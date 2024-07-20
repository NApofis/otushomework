cmake_minimum_required(VERSION 3.2)

project(helloworld VERSION ${PROJECT_VESRION})

add_executable(helloworld_cli main.cpp)

set(TESTS_LIST homework1/test.cpp PARENT_SCOPE)

add_library(helloworld_lib lib.cpp)

set_target_properties(helloworld_cli helloworld_lib PROPERTIES
    CXX_STANDARD 14
    CXX_STANDARD_REQUIRED ON
)

target_include_directories(helloworld_lib
    PRIVATE "${CMAKE_BINARY_DIR}"
)

target_link_libraries(helloworld_cli PRIVATE
        helloworld_lib
)

install(TARGETS helloworld_cli RUNTIME DESTINATION bin)