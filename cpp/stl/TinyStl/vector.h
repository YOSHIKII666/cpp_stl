#ifndef MYTINYSTL_VECTOR_H_
#define MYTINYSTL_VECTOR_H_

// 这个头文件包含一个模板类 vector
// vector : 向量

// notes:
//
// 异常保证：
// mystl::vecotr<T> 满足基本异常保证，部分函数无异常保证，并对以下函数做强异常安全保证：
//   * emplace
//   * emplace_back
//   * push_back
// 当 std::is_nothrow_move_assignable<T>::value == true 时，以下函数也满足强异常保证：
//   * reserve
//   * resize
//   * insert

#include <initializer_list>

#include "iterator.h"
#include "memory.h"
#include "util.h"
#include "exceptdef.h"

namespace mystl{
//#ifdef 是条件编译预处理指令的一种。它用于检查某个标识符（通常是宏名称）是否已经被定义过了。
//如果该标识符已经被定义，那么 #ifdef 和与之对应的 #endif 之间的代码块将会被编译处理；
//反之，如果该标识符未被定义，这段代码块则会被编译器忽略，不会参与到实际的编译过程中。
#ifdef max
#pragma message("#undefing marco max")
#undef max
#endif // max

#ifdef min
#pragma message("#undefing marco min")
#undef min
#endif // min

template <class T>
class vector{
public:
    typedef mystl::allocator<T>  allocator_type;
    typedef mystl::allocator<T> data_allocator;

    typedef typename allocator_type::value_type value_type;
    typedef typename allocator_type::pointer pointer;
    

};
}

#endif