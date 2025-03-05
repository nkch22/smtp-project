cmake_minimum_required(VERSION 3.30)

project(concurrency_lib)

add_library(concurrency STATIC
        src/Sync/WaitGroup.cpp
        src/ThreadPool/ThreadPool.cpp

        # To make IDE happy
        include/Concurrency/WaitGroup.h
        include/Concurrency/ThreadPool.h
        include/Concurrency/Task.h
        include/Concurrency/UnboundedBlockingMPMCQueue.h
)
target_include_directories(concurrency PUBLIC include)

# for some potential private headers
target_include_directories(concurrency PRIVATE src)

add_subdirectory(tests)