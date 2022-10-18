cmake_minimum_required(VERSION 3.1)

project(sfmlp)
find_package(SFML COMPONENTS graphics REQUIRED)
find_package(OpenCV REQUIRED)
add_executable(test main.cpp)

include_directories( ${OpenCV_INCLUDE_DIRS} )
target_link_libraries(test sfml-graphics ${OpenCV_LIBS} )