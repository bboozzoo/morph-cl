# OpenCV morphology with OpenCL tester
# Copyright (C) 2015  Maciej Borzecki
#
# This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 2 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.

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
