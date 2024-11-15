#ifndef MYTINYSTL_CONSTRACT_H_
#define MYTINYSTL_CONSTRACT_H_

#include<new>
#include"type_traits.h"
#include"iterator.h"

namespace mystl{

template<class Ty>//在指针 ptr 所指向的内存地址上，通过调用全局作用域的 new 操作符，使用类型 Ty 的默认构造函数来构造一个类型 Ty 的对象。
void construct(Ty* ptr){
     ::new((void*)ptr) Ty();//::new,全局作用域的new操作符
}

template<class Ty1,class Ty2>
void construct(Ty1* ptr,const Ty2& value){
     ::new((void*)ptr) Ty1(value);
}

template<class Ty,class... Args>
void construct(Ty* ptr,Args&&... args){
    ::new((void*)ptr) Ty(mystl::forward<Args>(args...));//Ty(&&T) 右值引用的构造函数
}

//析构函数
template<class Ty>
void destroy_one(Ty*,std::true_type){}

//在自定义的类型特性（type traits）或者使用标准库提供的类型特性来判断某个类型是否具有特定属性时，std::false_type 常被用作返回类型来表示不具备该属性。
template<class Ty>
void destroy_one(Ty* pointer,std::false_type){
    if(pointer!=nullptr){
        pointer->~Ty();
    }
}
template<class ForwardIter>
void destroy_cat(FowardIter,ForwardIter,std::true_type){}

template<class ForwardIter>
void destroy_cat(Forward first,Forward last,std::false_type){
    for(;first!=last;++first){
        destroy(&*first); //first是一个迭代器，*first是指向这个对象本身，&*first是获取这个指针指向的元素的地址
    }
}
template <class Ty>
// std::is_trivially_destructible<Ty>用于在编译期判断类型Ty是否是平凡可析构的，
// 一个类型被认为是平凡可析构的，意味着该类型的析构函数没有用户自定义的特殊行为，
//也就是它要么没有显式定义析构函数（此时编译器会为其生成一个默认的析构函数，这个默认析构函数通常不执行任何实质性操作，只是释放对象占用的内存空间等常规操作）
void destroy(Ty* pointer)
{
  destroy_one(pointer, std::is_trivially_destructible<Ty>{});
}

template <class ForwardIter>
void destroy(ForwardIter first, ForwardIter last)
{
  destroy_cat(first, last, std::is_trivially_destructible<
              typename iterator_traits<ForwardIter>::value_type>{});
};

}

#endif