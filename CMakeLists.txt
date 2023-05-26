#cmake_minimum_required(VERSION 3.22)
#project(OS_EX3)
#
#set(CMAKE_CXX_STANDARD 11)
#SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")
#add_executable(OS_EX3 MapReduceFramework.cpp utilities.cpp JobContext.cpp ThreadContext.h SampleClient.cpp Barrier/Barrier.cpp)
#add_executable(OS_EX3 JobContext.cpp JobContext.h)
##add_executable(OS_EX3 "Barrier/barrierdemo.cpp" "Barrier/Barrier.cpp" JobContext.cpp utilities.cpp)
#add_executable(OS_EX3 "Atomic/atomicdemo_multivar.cpp" ThreadContext.h)
#
#-----------------------------------------------------
cmake_minimum_required(VERSION 3.1)


# NOTE: You can't have both ThreadSanitizer and AddressSanitizer enabled at the same time.

# Uncomment the following to enable ThreadSanitizer.
#set (CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -fno-omit-frame-pointer -fsanitize=thread")
#set (CMAKE_LINKER_FLAGS_DEBUG "${CMAKE_LINKER_FLAGS_DEBUG} -fno-omit-frame-pointer -fsanitize=thread")

# Uncomment the following to enable AddressSanitizer.
#set (CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -fno-omit-frame-pointer -fsanitize=address")
#set (CMAKE_LINKER_FLAGS_DEBUG "${CMAKE_LINKER_FLAGS_DEBUG} -fno-omit-frame-pointer -fsanitize=address")


# Project configuration
project(ex3 VERSION 1.0 LANGUAGES C CXX)


# Ensure system has pthreads
set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)

add_library(MapReduceFramework
        MapReduceClient.h
        MapReduceFramework.cpp MapReduceFramework.h
        # ------------- Add your own .h/.cpp files here -------------------
        utilities.cpp Barrier/Barrier.cpp JobContext.cpp)


set_property(TARGET MapReduceFramework PROPERTY CXX_STANDARD 11)
target_compile_options(MapReduceFramework PUBLIC -Wextra)
target_include_directories(MapReduceFramework PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
# link pthreads to your framework
target_link_libraries(MapReduceFramework PUBLIC Threads::Threads)

# Add tests
add_subdirectory(mattanTests)

