cmake_minimum_required(VERSION 3.2)

project(gui_app)

include_directories(
        ${CMAKE_CURRENT_SOURCE_DIR}/.
)


file (GLOB FILES
        main.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/*/*.h
)

add_executable(${PROJECT_NAME} ${FILES})

set_target_properties(${PROJECT_NAME} PROPERTIES
        CXX_STANDARD 14
        CXX_STANDARD_REQUIRED ON
)

install(TARGETS ${PROJECT_NAME} RUNTIME DESTINATION bin)

