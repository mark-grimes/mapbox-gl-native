if(TARGET mbgl-vendor-supercluster.hpp)
    return()
endif()

add_library(mbgl-vendor-supercluster.hpp INTERFACE)

target_include_directories(mbgl-vendor-supercluster.hpp SYSTEM INTERFACE
    ${CMAKE_CURRENT_LIST_DIR}/supercluster.hpp/include
)
