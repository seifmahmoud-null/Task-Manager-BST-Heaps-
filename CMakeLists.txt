cmake_minimum_required(VERSION 3.29)
project(task_manager)

set(CMAKE_CXX_STANDARD 20)

add_executable(task_manager main.cpp
        heap.h
        BST.h)
