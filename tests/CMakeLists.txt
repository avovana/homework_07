cmake_minimum_required(VERSION 3.2)

project(test)

find_package(Boost COMPONENTS unit_test_framework REQUIRED)

add_executable(test_main_cli test_main.cpp)

target_include_directories(test_main_cli PRIVATE ../include)
target_include_directories(test_main_cli PRIVATE ${Boost_INCLUDE_DIR})

set_target_properties(test_main_cli PROPERTIES
  CXX_STANDARD 14
  CXX_STANDARD_REQUIRED ON
  COMPILE_OPTIONS -Wpedantic -Wall -Wextra
  COMPILE_DEFINITIONS BOOST_TEST_DYN_LINK
)

target_link_libraries(test_main_cli
  ${Boost_LIBRARIES}
)

enable_testing()

add_test(test_suite_main test_main_cli)