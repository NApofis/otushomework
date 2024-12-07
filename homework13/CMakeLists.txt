cmake_minimum_required(VERSION 3.2)

project(fashion_mnist)

add_executable(${PROJECT_NAME} main.cpp)

set_target_properties(${PROJECT_NAME} PROPERTIES
        CXX_STANDARD 14
        CXX_STANDARD_REQUIRED ON
)

include(FetchContent)

FetchContent_Declare(
        eigen
        URL https://gitlab.com/libeigen/eigen/-/archive/3.4.0/eigen-3.4.0.tar.gz
)

FetchContent_MakeAvailable(eigen)

target_link_libraries( ${PROJECT_NAME} PRIVATE
        eigen
)
install(TARGETS ${PROJECT_NAME} RUNTIME DESTINATION bin)

