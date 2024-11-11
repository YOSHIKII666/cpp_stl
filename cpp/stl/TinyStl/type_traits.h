#ifndef MYSTL_TYPE_TRAITS_H_
#define MYSTL_TYPE_TRAITRS_H_

#include<type_traits>
namespace mystl{

//定义的这个结构体，可将不同的数据组合成一个新整体
template<class T,T v>
struct m_integral_constant
{
    static constexpr T value=v;//constexpr指定一个表达式或函数在编译时就可以求值得到常量结果   
};

template<bool b>
using m_bool_constant=m_integral_constant<bool,b>; //m_bool_constant为一个别名
typedef m_bool_constant<true>  m_true_type;
typedef m_bool_constant<false> m_false_type;


template <class T1,class T2>
struct pair;
template<class T>
struct is_pair:mystl::m_false_type{};//is_pair继承自mystl::my_false_type

template<class T1,class T2>
struct is_pair<mystl::pair<T1,T2>>:mystl::m_true_type{};
}

#endif