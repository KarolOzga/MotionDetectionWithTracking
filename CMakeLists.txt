cmake_minimum_required(VERSION 3.16)
project(motion_detector)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package(OpenCV REQUIRED)

add_executable(motion_detector main.cpp)
target_include_directories(motion_detector PRIVATE ${OpenCV_INCLUDE_DIRS})
target_link_libraries(motion_detector PRIVATE ${OpenCV_LIBS})