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

    //结构体模版：pair
    //first和second分别表示取出来的数据
    template<class Ty1,class Ty2>
    struct pair {
        typedef Ty1 first_type;
        typedef Ty2 second_type;
        first_type first;
        second_type second;

        //默认构造函数
        template<class Other1=Ty1,class Other2=Ty2,typename = typename std::enable_if<std::is_default_constructible<Other1>::value&&std::is_default_constructible<Other2>::value,void>::type>
        constexpr pair():first(),second(){}
        //隐式构造函数
        template <class U1 = Ty1, class U2 = Ty2,
        typename std::enable_if<
        std::is_copy_constructible<U1>::value &&
        std::is_copy_constructible<U2>::value &&
        std::is_convertible<const U1&, Ty1>::value &&
        std::is_convertible<const U2&, Ty2>::value, int>::type = 0>
        constexpr pair(const Ty1& a, const Ty2& b)
        : first(a), second(b)
            {
            }
        //显式构造函数
        template<class U1=Ty1,class U2=Ty2,typename std::enable_if<std::is_copy_constructible<U1>::value&&std::is_copy_constructible<U2>::value&&(!std::is_convertible<const U1&,Ty1>::value||!std::is_convertible<const U2&,Ty2>::value),int>::type=0>
        explicit constexpr pair(const Ty1& a,const Ty2& b):first(a),second(b){};

        pair(const pair& rhs)=default;
        pair(pair&& rhs)=default;

        //其他类别的隐式构造函数，右值引用版本
        template <class Other1, class Other2,
           typename std::enable_if<
           std::is_constructible<Ty1, Other1>::value &&
           std::is_constructible<Ty2, Other2>::value &&
           std::is_convertible<Other1&&, Ty1>::value &&
           std::is_convertible<Other2&&, Ty2>::value, int>::type = 0>
           constexpr pair(Other1&& a, Other2&& b)
           : first(mystl::forward<Other1>(a)),
           second(mystl::forward<Other2>(b))
        {
        }

        // explicit constructiable for other type
        template <class Other1, class Other2,
          typename std::enable_if<
          std::is_constructible<Ty1, Other1>::value &&
          std::is_constructible<Ty2, Other2>::value &&
          (!std::is_convertible<Other1, Ty1>::value ||
           !std::is_convertible<Other2, Ty2>::value), int>::type = 0>
          explicit constexpr pair(Other1&& a, Other2&& b)
          : first(mystl::forward<Other1>(a)),
          second(mystl::forward<Other2>(b))
        {
        }

        //其他类别的隐式构造函数，左值引用版
        template<class Other1 ,class Other2,typename std::enable_if<std::is_constructible<Ty1,const Other1&>::value&&std::is_constructible<Ty2,const Other2&>::value&&(!std::is_convertible<const Other1&,Ty1>||!std::is_convertible<const Other2,Ty2>),int>::type=0>
        explicit constexpr pair(const pair<Other1,Other2>& other):first(other.first),second(other.second){};

        template<class Other1,class Other2,typename std::enable_if<std::is_constructible<Ty1,Other1>::value&&std::is_convertible<Ty2,Other2>::value&&std::is_convertible<Other1,Ty1>::value&&std::is_convertible<Other2,Ty2>::value,int>::type=0>
        constexpr pair(pair<Other1,Other2>&& other):first(mystl::forward<Other1>(other.first)),second(mystl::forward<Other2>(other.second)){};

        template<class Other1,class Other2,typename std::enable_if<std::is_constructible<Ty1,Other1>::value&&std::is_constructible<Ty2,Other2>::value&&(!std::is_convertible<Other1,Ty1>::value||!std::is_convertible<Other2,Ty2>::Value),int>::type=0>
        explicit constexpr pair(pair<Other1,Other2>&& other):first(mystl::forward<Other1>(other.first)),second(mystl::forward<Other2>(other.second)){}

        pair& operator=(const pair& rhs) {
            if(this!=&rhs) {
                first=rhs.first;
                second=rhs.second;
            }
            return *this;
        }

        template<class Other1 ,class Other2>
        pair& operator=(const pair<Other1,Other2>& other) {
            first=other.first;
            second=other.second;
            return *this;
        }

        template<class Other1,class Other2>
        pair operator=(pair<Other1,Other2>&& other) {
            first=mystl::forward<Other1>(other.first);
            second=mystl::forward<Other2>(other.second);
            return *this;
        }
        ~pair()=default;
        void swap(pair& other) {
            if(this!=&other) {
                mystl::swap(first,other.first);
                mystl::swap(second,other.second);
            }
        }
    };

    // 重载比较操作符
    template <class Ty1, class Ty2>
    bool operator==(const pair<Ty1, Ty2>& lhs, const pair<Ty1, Ty2>& rhs)
    {
        return lhs.first == rhs.first && lhs.second == rhs.second;
    }

    template <class Ty1, class Ty2>
    bool operator<(const pair<Ty1, Ty2>& lhs, const pair<Ty1, Ty2>& rhs)
    {
        return lhs.first < rhs.first || (lhs.first == rhs.first && lhs.second < rhs.second);
    }

    template <class Ty1, class Ty2>
    bool operator!=(const pair<Ty1, Ty2>& lhs, const pair<Ty1, Ty2>& rhs)
    {
        return !(lhs == rhs);
    }

    template <class Ty1, class Ty2>
    bool operator>(const pair<Ty1, Ty2>& lhs, const pair<Ty1, Ty2>& rhs)
    {
        return rhs < lhs;
    }

    template <class Ty1, class Ty2>
    bool operator<=(const pair<Ty1, Ty2>& lhs, const pair<Ty1, Ty2>& rhs)
    {
        return !(rhs < lhs);
    }

    template <class Ty1, class Ty2>
    bool operator>=(const pair<Ty1, Ty2>& lhs, const pair<Ty1, Ty2>& rhs)
    {
        return !(lhs < rhs);
    }

    // 重载 mystl 的 swap
    template <class Ty1, class Ty2>
    void swap(pair<Ty1, Ty2>& lhs, pair<Ty1, Ty2>& rhs)
    {
        lhs.swap(rhs);
    }

    // 全局函数，让两个数据成为一个 pair
    template <class Ty1, class Ty2>
    pair<Ty1, Ty2> make_pair(Ty1&& first, Ty2&& second)
    {
        return pair<Ty1, Ty2>(mystl::forward<Ty1>(first), mystl::forward<Ty2>(second));
    }

    // 全局函数，让两个数据成为一个 pair
    template <class Ty1, class Ty2>
    pair<Ty1, Ty2> make_pair(Ty1&& first, Ty2&& second)
    {
        return pair<Ty1, Ty2>(mystl::forward<Ty1>(first), mystl::forward<Ty2>(second));
    }

}

#endif