cmake_minimum_required(VERSION 3.2)
project(join_server)

set(CMAKE_CXX_STANDARD 17)

file (GLOB FILES
        ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp
        main.cpp
)
add_executable(${PROJECT_NAME} ${FILES} )

include(FetchContent)

FetchContent_Declare(
        Boost
        URL https://github.com/boostorg/boost/releases/download/boost-1.84.0/boost-1.84.0.tar.xz
        URL_MD5 893b5203b862eb9bbd08553e24ff146a
        DOWNLOAD_EXTRACT_TIMESTAMP ON
)

FetchContent_MakeAvailable(Boost)
target_link_libraries( ${PROJECT_NAME} PRIVATE
        Boost::asio
)

file (GLOB FILES_INCLUDE
        ${CMAKE_CURRENT_SOURCE_DIR}/src/*.hpp
)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

install(TARGETS ${PROJECT_NAME} RUNTIME DESTINATION bin)
