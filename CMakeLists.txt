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
        cpp/stl/TinyStl/functional.h
        cpp/stl/TinyStl/queue.h
        cpp/stl/TinyStl/stack.h
        cpp/stl/TinyStl/basic_string.h
        cpp/stl/TinyStl/list.h
        cpp/stl/TinyStl/rb_tree.h
        cpp/stl/TinyStl/map.h
        cpp/stl/TinyStl/hashtable.h
        cpp/stl/TinyStl/unordered_map.h
        cpp/stl/TinyStl/unordered_set.h
        cpp/stl/TinyStl/set_algo.h
        cpp/stl/TinyStl/numeric.h
        cpp/stl/Test/Lib/redbud/platform.h
        cpp/stl/Test/Lib/redbud/io/color.h
        cpp/stl/TinyStl/algorithm.h
        cpp/stl/Test/test.h
        cpp/stl/Test/map_test.h
)
