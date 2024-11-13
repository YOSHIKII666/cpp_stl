#ifndef MYTINYSTL_UTIL_H_
#define MYTINYSTL_UTIL_H_

#include<cstddef>
#include"type_traits.h"
namespace mystl{

    template<class T>
    typename std::remove_reference<T>::type&& move(T&& arg) noexcept
    {
        return static_cast<typename std::remove_reference<T>::type&&>(arg); //返回右值引用
    }

    template<class T>//将左值引用转换为右值引用传回去
    T&& forward(typename std::remove_reference<T>::type& arg)noexcept
    {
        return static_cast<T&&>(arg);
    }
    //通过 std::forward 转发参数就能确保根据原始参数是左值还是右值准确地调用对应的函数重载版本，
    //避免出现因错误的类型转换而导致调用不符合预期的函数重载情况。
    template<class T>
    T&& forward(typename std::remove_reference<T>::type&& arg)noexcept//
    {
        static_assert(!std::is_lvalue_reference<T>::value,"bad forward");//保证接收到的参数是右值引用
        return static_cast<T&&>(arg);
    }
}

#endif