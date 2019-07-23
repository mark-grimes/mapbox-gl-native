if(TARGET mbgl-vendor-variant)
    return()
endif()

add_library(mbgl-vendor-variant INTERFACE)

target_include_directories(mbgl-vendor-variant SYSTEM INTERFACE
    ${CMAKE_CURRENT_LIST_DIR}/variant/include
)
