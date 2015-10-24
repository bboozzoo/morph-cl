cmake_minimum_required(VERSION 2.8.0 FATAL_ERROR)

project(morph-cl)

find_package(OpenCV)
find_package(Boost COMPONENTS program_options REQUIRED)

add_executable(morph-cl
  morph-cl.cpp)

include_directories(${OpenCV_INCLUDE_DIRS})
include_directories(${Boost_INCLUDE_DIRS})
target_link_libraries(morph-cl ${OpenCV_LIBS} ${Boost_LIBRARIES})

set_property(TARGET morph-cl PROPERTY CXX_STANDARD 14)
set_property(TARGET morph-cl PROPERTY CXX_STANDARD_REQUIRED ON)
