if(TARGET mbgl-vendor-filesystem)
    return()
endif()

add_library(mbgl-vendor-filesystem INTERFACE)

target_include_directories(mbgl-vendor-filesystem SYSTEM INTERFACE
    ${CMAKE_SOURCE_DIR}/vendor/filesystem/include
)
