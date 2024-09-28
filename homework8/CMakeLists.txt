cmake_minimum_required(VERSION 3.2)

project(bayan)

#FIND_PACKAGE( Boost REQUIRED COMPONENTS
#        program_options
#        filesystem
#        system
#        serialization
#        regex
#        REQUIRED )

#INCLUDE_DIRECTORIES( ${Boost_INCLUDE_DIR} )
#https://github.com/boostorg/boost/
#https://github.com/boost-cmake/boost/tree/master

include(FetchContent)

FetchContent_Declare(
    Boost
    URL https://github.com/boostorg/boost/releases/download/boost-1.84.0/boost-1.84.0.tar.xz
    URL_MD5 893b5203b862eb9bbd08553e24ff146a
    DOWNLOAD_EXTRACT_TIMESTAMP ON
)

FetchContent_MakeAvailable(Boost)

file (GLOB FILES
        main.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp
)
add_executable(${PROJECT_NAME} ${FILES} )

target_include_directories(${PROJECT_NAME}
        PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/include
#        ${Boost_INCLUDE_DIRS}
)
target_link_libraries( ${PROJECT_NAME} PRIVATE
        Boost::filesystem
        Boost::system
        Boost::program_options
        Boost::detail
        Boost::mpl
        Boost::flyweight
        Boost::regex
)

set_target_properties(${PROJECT_NAME} PROPERTIES
        CXX_STANDARD 14
        CXX_STANDARD_REQUIRED ON
)

install(TARGETS ${PROJECT_NAME} RUNTIME DESTINATION bin)

