if(TARGET mbgl-vendor-gtest)
    return()
endif()

add_library(mbgl-vendor-gtest STATIC EXCLUDE_FROM_ALL
    ${CMAKE_CURRENT_LIST_DIR}/gtest/src/gtest-death-test.cc
    ${CMAKE_CURRENT_LIST_DIR}/gtest/src/gtest-filepath.cc
    ${CMAKE_CURRENT_LIST_DIR}/gtest/src/gtest-port.cc
    ${CMAKE_CURRENT_LIST_DIR}/gtest/src/gtest-printers.cc
    ${CMAKE_CURRENT_LIST_DIR}/gtest/src/gtest-test-part.cc
    ${CMAKE_CURRENT_LIST_DIR}/gtest/src/gtest-typed-test.cc
    ${CMAKE_CURRENT_LIST_DIR}/gtest/src/gtest.cc
)

target_include_directories(mbgl-vendor-gtest PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}/gtest/include
)
