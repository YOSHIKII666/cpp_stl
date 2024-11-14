#ifndef MYTINYSTL_UTIL_H_
#define MYTINYSTL_UTIL_H_

#include<cstddef>
#include"type_traits.h"
namespace mystl{
    /*
        forward和move的区别：
            move是把接收到的参数强制转为右值引用传递回去,无论是左值还是右值引用都传回去。
            forward是在接收参数时，会转为左值引用，那么在static_cast<T&&>过程中，返回的是T && &,相当于T&，不会改变原来的结构，导致出现错误的情况。
    */

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

    template<class T>
    void swap(T& lhs,T& rhs) {
        //右值传递赋值，节省空间
        auto tmp(mystl::move(lhs));
        lhs=mystl::move(rhs);
        rhs=mystl::move(tmp);
    }
}

#endif