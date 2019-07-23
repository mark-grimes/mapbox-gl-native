if(TARGET mbgl-vendor-kdbush.hpp)
    return()
endif()

add_library(mbgl-vendor-kdbush.hpp INTERFACE)

target_include_directories(mbgl-vendor-kdbush.hpp SYSTEM INTERFACE
    ${CMAKE_CURRENT_LIST_DIR}/kdbush.hpp/include
)
