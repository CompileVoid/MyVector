cmake_minimum_required(VERSION 3.21)
project(MyVectorProject LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(MyVector INTERFACE)
target_include_directories(MyVector INTERFACE
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
)

find_package(GTest CONFIG REQUIRED)

enable_testing()

add_executable(run_tests tests/test_my_vector.cpp)
target_link_libraries(run_tests PRIVATE MyVector GTest::gtest_main)

include(GoogleTest)
gtest_discover_tests(run_tests)
