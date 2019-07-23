if(TARGET mbgl-vendor-rapidjson)
    return()
endif()

add_library(mbgl-vendor-rapidjson INTERFACE)

target_include_directories(mbgl-vendor-rapidjson SYSTEM INTERFACE
    ${CMAKE_CURRENT_LIST_DIR}/rapidjson/include
)

target_compile_definitions(mbgl-vendor-rapidjson INTERFACE
    RAPIDJSON_HAS_STDSTRING=1
)

if(WIN32)
    target_compile_definitions(mbgl-vendor-rapidjson INTERFACE
        RAPIDJSON_HAS_CXX11_RVALUE_REFS
    )
endif()
