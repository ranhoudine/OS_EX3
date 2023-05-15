cmake_minimum_required(VERSION 3.22)
project(OS_EX3)

set(CMAKE_CXX_STANDARD 11)
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")
add_executable(OS_EX3 MapReduceFramework.cpp utilities.cpp JobContext.cpp ThreadContext.h)
#add_executable(OS_EX3 JobContext.cpp JobContext.h)
add_executable(OS_EX3 "Barrier/barrierdemo.cpp" "Barrier/Barrier.cpp")
