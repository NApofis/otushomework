cmake_minimum_required(VERSION 3.2)
project(async)

set(CMAKE_CXX_STANDARD 17)

file (GLOB FILES
        ${CMAKE_CURRENT_SOURCE_DIR}/async/async.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/async/src/*.cpp
)

add_library(async SHARED ${FILES})


target_include_directories(async
        PRIVATE
        ${PROJECT_SOURCE_DIR}/async/include
        PUBLIC
        ${PROJECT_SOURCE_DIR}/include
)

add_executable(async_example example.cpp)
target_link_libraries(async_example PRIVATE async)