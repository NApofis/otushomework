cmake_minimum_required(VERSION 3.2)

set(PATCH_VERSION "1" CACHE INTERNAL "Patch version")
set(PROJECT_VESRION 0.0.${PATCH_VERSION})

project(homework VERSION ${PROJECT_VESRION})

configure_file(homework1/info.h.in info.h)

add_executable(helloworld_cli homework1/main.cpp)
add_executable(ip_filter homework2/main.cpp)

add_library(helloworld_lib homework1/lib.cpp)
add_library(ip_filter_lib homework2/ip_filter.cpp)

add_subdirectory(./googletest)
include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR} ${CMAKE_BINARY_DIR})

add_executable(tests homework1/test.cpp homework2/test.cpp)

find_package(OpenSSL REQUIRED)

set_target_properties(helloworld_cli ip_filter helloworld_lib ip_filter_lib tests PROPERTIES
    CXX_STANDARD 14
    CXX_STANDARD_REQUIRED ON
)
target_include_directories(helloworld_lib
    PRIVATE "${CMAKE_BINARY_DIR}"
)
target_link_libraries(helloworld_cli PRIVATE
        helloworld_lib
)

target_link_libraries(ip_filter PRIVATE
        ip_filter_lib
)

target_link_libraries(tests
        gtest
        gtest_main
        helloworld_lib
        ip_filter_lib
        OpenSSL::SSL
)

if (MSVC)

    target_compile_options(helloworld_cli PRIVATE
        /W4
    )
    target_compile_options(ip_filter PRIVATE
        /W4
    )
    target_compile_options(helloworld_lib PRIVATE
        /W4
    )
    target_compile_options(tests PRIVATE
        /W4
    )
endif()


install(TARGETS helloworld_cli RUNTIME DESTINATION bin)
install(TARGETS ip_filter RUNTIME DESTINATION bin)

set(CPACK_GENERATOR DEB)

set(CPACK_PACKAGE_VERSION_MAJOR "${PROJECT_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${PROJECT_VERSION_MINOR}")
set(CPACK_PACKAGE_VERSION_PATCH "${PROJECT_VERSION_PATCH}")

set(CPACK_PACKAGE_CONTACT tihonovn.a@yandex.ru)

include(CPack)

enable_testing()

add_test(tests tests)
