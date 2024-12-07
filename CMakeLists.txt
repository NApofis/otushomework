cmake_minimum_required(VERSION 3.2)

set(PATCH_VERSION "1" CACHE INTERNAL "Patch version")
set(PROJECT_VESRION 0.0.${PATCH_VERSION})

project(homework VERSION ${PROJECT_VESRION})
configure_file(homework1/info.h.in info.h)

set(TESTS_LIST ""
        homework11/src/server.cpp
        homework11/src/server.h)

add_subdirectory(homework1)
add_subdirectory(homework2)
add_subdirectory(homework3)
#add_subdirectory(homework3_1)
add_subdirectory(homework4)
add_subdirectory(homework5)
add_subdirectory(homework6)
add_subdirectory(homework7)
add_subdirectory(homework8)
add_subdirectory(homework9)
add_subdirectory(homework10)
add_subdirectory(homework11)
add_subdirectory(homework13)

find_package(OpenSSL REQUIRED)

add_executable(tests ${TESTS_LIST})

add_subdirectory(./googletest)
include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR} ${CMAKE_BINARY_DIR})

set_target_properties(tests PROPERTIES
    CXX_STANDARD 14
    CXX_STANDARD_REQUIRED ON
)

target_link_libraries(tests
    gtest
    gtest_main
    helloworld_lib
    ip_filter_lib
    OpenSSL::SSL
)

set(CPACK_GENERATOR DEB)

set(CPACK_PACKAGE_VERSION_MAJOR "${PROJECT_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${PROJECT_VERSION_MINOR}")
set(CPACK_PACKAGE_VERSION_PATCH "${PROJECT_VERSION_PATCH}")

set(CPACK_PACKAGE_CONTACT tihonovn.a@yandex.ru)

include(CPack)

enable_testing()

add_test(tests tests)
