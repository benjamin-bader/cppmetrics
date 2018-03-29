cmake_minimum_required(VERSION 3.2.2)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

project(cppmetrics)

option(BUILD_TESTS "Build unit tests" OFF)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake")

include(MetricHelpers)

if(BUILD_TESTS)
  include(GTest)
  enable_testing()
endif(BUILD_TESTS)

add_subdirectory(metrics)
