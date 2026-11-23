cmake_minimum_required(VERSION 3.16)
project(motion_detector)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package(OpenCV REQUIRED)

# Biblioteka z logiką (używana przez program i przez testy)
add_library(motion_lib
        src/BackgroundModel.cpp
        include/BackgroundModel.h
        src/ConnectedComponents.cpp
        include/ConnectedComponents.h)
target_include_directories(motion_lib PUBLIC
        include
        ${OpenCV_INCLUDE_DIRS})
target_link_libraries(motion_lib PUBLIC ${OpenCV_LIBS})

# Program
add_executable(motion_detector src/main.cpp
)
target_link_libraries(motion_detector PRIVATE motion_lib)

# Testy (GoogleTest)
include(FetchContent)
FetchContent_Declare(googletest
        URL https://github.com/google/googletest/archive/refs/tags/v1.14.0.zip
        DOWNLOAD_EXTRACT_TIMESTAMP TRUE)
FetchContent_MakeAvailable(googletest)

enable_testing()
add_executable(tests tests/test_connected_components.cpp)
target_link_libraries(tests PRIVATE motion_lib GTest::gtest_main)

include(GoogleTest)
gtest_discover_tests(tests)