#ifndef MYTINYSTL_ITERATOR_H_
#define MYTINYSTL_ITERATOR_H_

// 这个头文件用于迭代器设计，包含了一些模板结构体与全局函数，

#include <cstddef>

#include "type_traits.h"
namespace mystl{

//五种迭代器
struct input_iterator_tag{};
struct output_iterator_tag{};
struct forward_iterator_tag:public input_iterator_tag{};
struct bidirectional_iterator_tag:public input_iterator_tag{};//双向迭代器
struct random_access_iterator_tag:public bidirectional_iterator_tag{};

//iterator结构体的定义
template<class Category,class T,class Distance=ptrdiff_t,class Pointer=T*,class Reference=T&>
struct iterator
{
    typedef Category iterator_category;
    typedef T value_type;
    typedef Pointer pointer;
    typedef Reference reference;
    typedef Distance difference_type;
};

//判断迭代器类别
template<class T>
struct has_iterator_cat
{
    private:
        struct two
        {
            char a;
            char b;
        }
        template<class U>
        static two test(...);
        template<class U>
        static char test(typename U::iterator_category* =0);
    public:
        static const bool value=sizeof(test<T>(0))==sizeof(char);
};

template<class Iterator,bool>
struct iterator_traits_impl{};

//typename明确指出某个名字是类型名而不是其他
template<class Iterator,bool>
struct iterator_traits_impl<Iterator,true>
{
    typedef typename Iterator::iterator_category iterator_category;
    typedef typename Iterator::value_type value_type;
    typedef typename Iterator::pointer pointer;
    typedef typename Iterator::reference reference;
    typedef typename Iterator:: difference_type difference_type;
};

template <class Iterator,bool>
struct iterator_traits_helper{};


template<class Iterator>
struct iterator_traits_helper<Iterator,true>:public iterator_traits_impl<Iterator,
    std::is_convertible<typename Iterator::iterator_category,input_iterator_tag>::value||
    std::is_convertible<typename Iterator::iterator_category,output_iterator_tag>::value>{};


template<class Iterator>
struct iterator_traits:public iterator_traits_helper<Iterator,has_iterator_cat<Iterator>::value>{};

template<class T>
struct iterator_traits<T*>
{
    typedef random_access_iterator_tag iterator_tag;
    typedef T value_type;
    typedef T* pointer;
    typedef T& reference;
    typedef ptrdiff_t difference_type;
};



}


#endif