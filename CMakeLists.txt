cmake_minimum_required(VERSION 3.29)
project(cpp_stl)

set(CMAKE_CXX_STANDARD 14)


add_executable(cpp_stl
    cpp/stl/TinyStl/iterator.h
    cpp/stl/TinyStl/type_traits.h
        cpp/stl/TinyStl/test.cpp
        cpp/stl/TinyStl/util.h
        cpp/stl/TinyStl/construct.h
        cpp/stl/TinyStl/allocator.h
        cpp/stl/TinyStl/algobase.h
        cpp/stl/TinyStl/uninitialized.h
        cpp/stl/TinyStl/memory.h
        cpp/stl/TinyStl/vector.h
        cpp/stl/TinyStl/heap_algo.h
        cpp/stl/TinyStl/deque.h
)
