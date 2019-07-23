if(TARGET mbgl-vendor-jni.hpp)
    return()
endif()

add_library(mbgl-vendor-jni.hpp INTERFACE)

target_include_directories(mbgl-vendor-jni.hpp SYSTEM INTERFACE
    ${CMAKE_CURRENT_LIST_DIR}/jni.hpp/include
)
