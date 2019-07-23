add_executable(mbgl-render-test
    ${CMAKE_CURRENT_LIST_DIR}/filesystem.hpp
    ${CMAKE_CURRENT_LIST_DIR}/main.cpp
    ${CMAKE_CURRENT_LIST_DIR}/metadata.hpp
    ${CMAKE_CURRENT_LIST_DIR}/parser.cpp
    ${CMAKE_CURRENT_LIST_DIR}/parser.hpp
    ${CMAKE_CURRENT_LIST_DIR}/runner.cpp
    ${CMAKE_CURRENT_LIST_DIR}/runner.hpp
)

target_compile_definitions(mbgl-render-test PRIVATE
    TEST_RUNNER_ROOT_PATH="${PROJECT_SOURCE_DIR}"
)

# FIXME: Should not use core private interface
target_include_directories(mbgl-render-test PRIVATE
    ${PROJECT_SOURCE_DIR}/src
)

include(${PROJECT_SOURCE_DIR}/vendor/args.cmake)
include(${PROJECT_SOURCE_DIR}/vendor/boost.cmake)
include(${PROJECT_SOURCE_DIR}/vendor/filesystem.cmake)
include(${PROJECT_SOURCE_DIR}/vendor/pixelmatch-cpp.cmake)

target_link_libraries(mbgl-render-test PRIVATE
    mbgl-core
    mbgl-vendor-args
    mbgl-vendor-boost
    mbgl-vendor-filesystem
    mbgl-vendor-pixelmatch-cpp
)

set_property(TARGET mbgl-render-test PROPERTY FOLDER Executables)

add_test(NAME mbgl-render-test COMMAND mbgl-render-test)
