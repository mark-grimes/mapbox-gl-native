if(TARGET mbgl-vendor-geometry.hpp)
    return()
endif()

add_library(mbgl-vendor-geometry.hpp INTERFACE)

target_include_directories(mbgl-vendor-geometry.hpp SYSTEM INTERFACE
    ${CMAKE_CURRENT_LIST_DIR}/geometry.hpp/include
)
