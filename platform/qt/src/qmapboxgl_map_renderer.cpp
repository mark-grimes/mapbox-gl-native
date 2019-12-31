#include "qmapboxgl_map_renderer.hpp"
#include "qmapboxgl_scheduler.hpp"

#include <mbgl/gfx/backend_scope.hpp>

#include <QThreadStorage>
#include <QtGlobal>

static bool needsToForceScheduler() {
    static QThreadStorage<bool> force;

    if (!force.hasLocalData()) {
        force.setLocalData(mbgl::Scheduler::GetCurrent() == nullptr);
    }

    return force.localData();
};

static auto *getScheduler() {
    static QThreadStorage<std::shared_ptr<QMapboxGLScheduler>> scheduler;

    if (!scheduler.hasLocalData()) {
        scheduler.setLocalData(std::make_shared<QMapboxGLScheduler>());
    }

    return scheduler.localData().get();
};

QMapboxGLMapRenderer::QMapboxGLMapRenderer(qreal pixelRatio, QMapboxGLSettings::GLContextMode mode, const QString &localFontFamily)
    : m_backend(static_cast<mbgl::gfx::ContextMode>(mode)),
      m_renderer(std::make_unique<mbgl::Renderer>(m_backend, pixelRatio, mbgl::optional<std::string> {},
                 localFontFamily.isEmpty() ? mbgl::nullopt : mbgl::optional<std::string> { localFontFamily.toStdString() }))
    , m_forceScheduler(needsToForceScheduler())
{
    // If we don't have a Scheduler on this thread, which
    // is usually the case for render threads, use a shared
    // dummy scheduler that needs to be explicitly forced to
    // process events.
    if (m_forceScheduler) {
        auto scheduler = getScheduler();

        if (mbgl::Scheduler::GetCurrent() == nullptr) {
            mbgl::Scheduler::SetCurrent(scheduler);
        }

        connect(scheduler, SIGNAL(needsProcessing()), this, SIGNAL(needsRendering()));
    }
}

QMapboxGLMapRenderer::~QMapboxGLMapRenderer()
{
    MBGL_VERIFY_THREAD(tid);
}

void QMapboxGLMapRenderer::updateParameters(std::shared_ptr<mbgl::UpdateParameters> newParameters)
{
    std::lock_guard<std::mutex> lock(m_updateMutex);
    m_updateParameters = std::move(newParameters);
}

std::vector<QMapbox::Feature> QMapboxGLMapRenderer::queryRenderedFeatures(const QPointF& point) const
{
    const mbgl::ScreenCoordinate mapboxPoint( point.x(), point.y() );
    const auto mapboxFeatures=m_renderer->queryRenderedFeatures( mapboxPoint );

    std::vector<QMapbox::Feature> result;
    for( const auto& mapboxFeature : mapboxFeatures )
    {
        QMapbox::Feature qtFeature;
        if( mapboxFeature.id.is<mapbox::feature::null_value_t>() ) qtFeature.id=QVariant();
        else if( mapboxFeature.id.is<uint64_t>() ) qtFeature.id=QVariant::fromValue( mapboxFeature.id.get<uint64_t>() );
        else if( mapboxFeature.id.is<int64_t>() ) qtFeature.id=QVariant::fromValue( mapboxFeature.id.get<int64_t>() );
        else if( mapboxFeature.id.is<double>() ) qtFeature.id=QVariant::fromValue( mapboxFeature.id.get<double>() );
        else if( mapboxFeature.id.is<std::string>() )
        {
            qtFeature.id=QVariant::fromValue<QString>( mapboxFeature.id.get<std::string>().c_str() );
            std::string value=mapboxFeature.id.get<std::string>();
        }

        if( mapboxFeature.geometry.is<mapbox::geometry::point<double>>() )
        {
            qtFeature.type=QMapbox::Feature::Type::PointType;
            // TODO - convert mapboxFeature.geometry
        }
        else if( mapboxFeature.geometry.is<mapbox::geometry::line_string<double>>() )
        {
            qtFeature.type=QMapbox::Feature::Type::LineStringType;
            // TODO - convert mapboxFeature.geometry
        }
        else if( mapboxFeature.geometry.is<mapbox::geometry::polygon<double>>() )
        {
            qtFeature.type=QMapbox::Feature::Type::PolygonType;
            // TODO - convert mapboxFeature.geometry
        }

        for( const auto& nameValuePair : mapboxFeature.properties )
        {
            // using value_base = mapbox::util::variant<null_value_t, bool, uint64_t, int64_t, double, std::string,
            //                                          mapbox::util::recursive_wrapper<std::vector<value>>,
            //                                          mapbox::util::recursive_wrapper<std::unordered_map<std::string, value>>>;

            if( nameValuePair.second.is<mapbox::feature::null_value_t>() ) qtFeature.properties[nameValuePair.first.c_str()]=QVariant();
            else if( nameValuePair.second.is<bool>() ) qtFeature.properties[nameValuePair.first.c_str()]=nameValuePair.second.get<bool>();
            else if( nameValuePair.second.is<uint64_t>() ) qtFeature.properties[nameValuePair.first.c_str()]=nameValuePair.second.get<uint64_t>();
            else if( nameValuePair.second.is<int64_t>() ) qtFeature.properties[nameValuePair.first.c_str()]=nameValuePair.second.get<int64_t>();
            else if( nameValuePair.second.is<double>() ) qtFeature.properties[nameValuePair.first.c_str()]=nameValuePair.second.get<double>();
            else if( nameValuePair.second.is<std::string>() ) qtFeature.properties[nameValuePair.first.c_str()]=QVariant::fromValue<QString>( nameValuePair.second.get<std::string>().c_str() );
            // TODO - add conversions for vectors and maps. For now just add an empty entry so at least we get the name
            else qtFeature.properties[nameValuePair.first.c_str()]=QVariant();
        }

        result.push_back( std::move(qtFeature) );
    }

    return result;
}

void QMapboxGLMapRenderer::updateFramebuffer(quint32 fbo, const mbgl::Size &size)
{
    MBGL_VERIFY_THREAD(tid);

    m_backend.updateFramebuffer(fbo, size);
}

void QMapboxGLMapRenderer::render()
{
    MBGL_VERIFY_THREAD(tid);

    std::shared_ptr<mbgl::UpdateParameters> params;
    {
        // Lock on the parameters
        std::lock_guard<std::mutex> lock(m_updateMutex);

        // UpdateParameters should always be available when rendering.
        assert(m_updateParameters);

        // Hold on to the update parameters during render
        params = m_updateParameters;
    }

    // The OpenGL implementation automatically enables the OpenGL context for us.
    mbgl::gfx::BackendScope scope(m_backend, mbgl::gfx::BackendScope::ScopeType::Implicit);

    m_renderer->render(*params);

    if (m_forceScheduler) {
        getScheduler()->processEvents();
    }
}

void QMapboxGLMapRenderer::setObserver(std::shared_ptr<mbgl::RendererObserver> observer)
{
    m_renderer->setObserver(observer.get());
}
