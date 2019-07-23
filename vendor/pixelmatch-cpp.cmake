if(TARGET mbgl-vendor-pixelmatch-cpp)
    return()
endif()

add_library(mbgl-vendor-pixelmatch-cpp INTERFACE)

target_include_directories(mbgl-vendor-pixelmatch-cpp SYSTEM INTERFACE
    ${CMAKE_CURRENT_LIST_DIR}/pixelmatch-cpp/include
)
