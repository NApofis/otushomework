cmake_minimum_required(VERSION 3.2)

project(fashion_mnist)


add_executable(${PROJECT_NAME} main.cpp mlp_classifier.h classifier.h)

set_target_properties(${PROJECT_NAME} PROPERTIES
        CXX_STANDARD 20
        CXX_STANDARD_REQUIRED ON
)

include(FetchContent)

FetchContent_Declare(
        eigen
        URL https://gitlab.com/libeigen/eigen/-/archive/3.4.0/eigen-3.4.0.tar.gz
        URL_MD5 4c527a9171d71a72a9d4186e65bea559
        DOWNLOAD_EXTRACT_TIMESTAMP ON
)

FetchContent_MakeAvailable(eigen)
target_link_libraries( ${PROJECT_NAME} PRIVATE
        eigen
)
install(TARGETS ${PROJECT_NAME} RUNTIME DESTINATION bin)

