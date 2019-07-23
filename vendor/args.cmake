if(TARGET mbgl-vendor-args)
    return()
endif()

add_library(mbgl-vendor-args INTERFACE)

target_include_directories(mbgl-vendor-args SYSTEM INTERFACE
    ${CMAKE_CURRENT_LIST_DIR}/args
)
