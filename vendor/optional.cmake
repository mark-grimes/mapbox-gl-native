if(TARGET mbgl-vendor-optional)
    return()
endif()

add_library(mbgl-vendor-optional INTERFACE)

target_include_directories(mbgl-vendor-optional SYSTEM INTERFACE
    ${CMAKE_CURRENT_LIST_DIR}/optional
)
