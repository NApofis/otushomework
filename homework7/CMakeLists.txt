cmake_minimum_required(VERSION 3.2)

project(bulk)

file (GLOB FILES
        main.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp
)

add_executable(${PROJECT_NAME} ${FILES} )

target_include_directories(${PROJECT_NAME}
        PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

set_target_properties(${PROJECT_NAME} PROPERTIES
        CXX_STANDARD 22
        CXX_STANDARD_REQUIRED ON
)

install(TARGETS ${PROJECT_NAME} RUNTIME DESTINATION bin)

