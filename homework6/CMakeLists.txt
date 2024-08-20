cmake_minimum_required(VERSION 3.2)

project(matrix)


add_executable(${PROJECT_NAME} main.cpp )

set_target_properties(${PROJECT_NAME} PROPERTIES
        CXX_STANDARD 14
        CXX_STANDARD_REQUIRED ON
)

install(TARGETS ${PROJECT_NAME} RUNTIME DESTINATION bin)

