if(TARGET mbgl-vendor-geojson.hpp)
    return()
endif()

add_library(mbgl-vendor-geojson.hpp INTERFACE)

target_include_directories(mbgl-vendor-geojson.hpp SYSTEM INTERFACE
    ${CMAKE_CURRENT_LIST_DIR}/geojson.hpp/include
)
