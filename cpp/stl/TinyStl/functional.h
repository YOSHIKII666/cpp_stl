//
// Created by 王少泽 on 2024/11/29.
//

#ifndef MYTINYSTL_FUNCTIONAL_H_
#define MYTINYSTL_FUNCTIONAL_H_

// 这个头文件包含了 mystl 的函数对象与哈希函数

#include <cstddef>
namespace mystl {
    //一个参数的函数和返回
    template<class Arg,class Result>
    struct unarg_function {
        typedef Arg argument_types;
        typedef Result result_type;
    };
    //两个参数的函数和返回
    template<class Arg1,class Arg2,class Result>
    struct binary_function {
        typedef Arg1 first_argument_type;
        typedef Arg2 second_argument_type;
        typedef Result result_type;
    };
    //加法
    template<class T>
    struct plus:public binary_function<T,T,T> {
        T operator()(const T& x,const T& y) {
            return x+y;
        }
    };
    //减法
    template<class T>
    struct minus:public binary_function<T,T,T> {
        T operator()(const T& x,const T& y) {
            return x-y;
        }
    };
    //乘法
    template<class T>
    struct multiplies:public binary_function<T,T,T> {
        T operator()(const T& x,const T& y) {
            return x*y;
        }
    };
    //除法
    template<class T>
    struct divides:public binary_function<T,T,T> {
        T operator()(const T& x,const T& y) {
            return x/y;
        }
    };
    //取模
    template<class T>
    struct negate:public binary_function<T,T,T> {
        T operator()(const T& x,const T& y) {
            return x%y;
        }
    };
    //否定
    template<class T>
    struct negate:public unarg_function<T,T> {
        T operator()(const T& x) {
            return -x;
        }
    };
        // 加法的证同元素
    template <class T>
    T identity_element(plus<T>) { return T(0); }

    // 乘法的证同元素
    template <class T>
    T identity_element(multiplies<T>) { return T(1); }

    // 函数对象：等于
    template <class T>
    struct equal_to :public binary_function<T, T, bool>
    {
      bool operator()(const T& x, const T& y) const { return x == y; }
    };

    // 函数对象：不等于
    template <class T>
    struct not_equal_to :public binary_function<T, T, bool>
    {
      bool operator()(const T& x, const T& y) const { return x != y; }
    };

    // 函数对象：大于
    template <class T>
    struct greater :public binary_function<T, T, bool>
    {
      bool operator()(const T& x, const T& y) const { return x > y; }
    };

    // 函数对象：小于
    template <class T>
    struct less :public binary_function<T, T, bool>
    {
      bool operator()(const T& x, const T& y) const { return x < y; }
    };

    // 函数对象：大于等于
    template <class T>
    struct greater_equal :public binary_function<T, T, bool>
    {
      bool operator()(const T& x, const T& y) const { return x >= y; }
    };

    // 函数对象：小于等于
    template <class T>
    struct less_equal :public binary_function<T, T, bool>
    {
      bool operator()(const T& x, const T& y) const { return x <= y; }
    };

    // 函数对象：逻辑与
    template <class T>
    struct logical_and :public binary_function<T, T, bool>
    {
      bool operator()(const T& x, const T& y) const { return x && y; }
    };

    // 函数对象：逻辑或
    template <class T>
    struct logical_or :public binary_function<T, T, bool>
    {
      bool operator()(const T& x, const T& y) const { return x || y; }
    };

    // 函数对象：逻辑非
    template <class T>
    struct logical_not :public unarg_function<T, bool>
    {
      bool operator()(const T& x) const { return !x; }
    };

    // 证同函数：不会改变元素，返回本身
    template <class T>
    struct identity :public unarg_function<T, bool>
    {
      const T& operator()(const T& x) const { return x; }
    };

    // 选择函数：接受一个 pair，返回第一个元素
    template <class Pair>
    struct selectfirst :public unarg_function<Pair, typename Pair::first_type>
    {
      const typename Pair::first_type& operator()(const Pair& x) const
      {
        return x.first;
      }
    };

    // 选择函数：接受一个 pair，返回第二个元素
    template <class Pair>
    struct selectsecond :public unarg_function<Pair, typename Pair::second_type>
    {
      const typename Pair::second_type& operator()(const Pair& x) const
      {
        return x.second;
      }
    };

    // 投射函数：返回第一参数
    template <class Arg1, class Arg2>
    struct projectfirst :public binary_function<Arg1, Arg2, Arg1>
    {
      Arg1 operator()(const Arg1& x, const Arg2&) const { return x; }
    };

    // 投射函数：返回第二参数
    template <class Arg1, class Arg2>
    struct projectsecond :public binary_function<Arg1, Arg2, Arg1>
    {
      Arg2 operator()(const Arg1&, const Arg2& y) const { return y; }
    };



    //哈希函数
    template<class T>
    struct hash<T*> {
        size_t operator()(T* p) const noexcept {
            return reinterpret_cast<size_t>(p);
        }
    };

    // 对于整型类型，只是返回原值
#define MYSTL_TRIVIAL_HASH_FCN(Type)         \
template <> struct hash<Type>                \
{                                            \
size_t operator()(Type val) const noexcept \
{ return static_cast<size_t>(val); }       \
};
}
#endif
