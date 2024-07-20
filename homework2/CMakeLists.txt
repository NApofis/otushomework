cmake_minimum_required(VERSION 3.2)

set(PATCH_VERSION "1" CACHE INTERNAL "Patch version")
set(PROJECT_VESRION 0.0.${PATCH_VERSION})

project(ip_filter VERSION ${PROJECT_VESRION})

set(TESTS_LIST homework2/test.cpp PARENT_SCOPE)

add_executable(ip_filter main.cpp)

add_library(ip_filter_lib ip_filter.cpp)

set_target_properties( ip_filter ip_filter_lib PROPERTIES
        CXX_STANDARD 14
        CXX_STANDARD_REQUIRED ON
)
target_link_libraries(ip_filter PRIVATE
        ip_filter_lib
)


install(TARGETS ip_filter RUNTIME DESTINATION bin)

