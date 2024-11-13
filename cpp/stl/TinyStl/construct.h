#ifndef MYTINYSTL_CONSTRACT_H_
#define MYTINYSTL_CONSTRACT_H_

#include<new>
#include"type_traits.h"
#include"iterator.h"

namespace mystl{

template<class Ty>//在指针 ptr 所指向的内存地址上，通过调用全局作用域的 new 操作符，使用类型 Ty 的默认构造函数来构造一个类型 Ty 的对象。
void construct(Ty* ptr){
    ::new((*void) ptr) Ty();//::new,全局作用域的new操作符
}

template<class Ty1,class Ty2>
void construct(Ty1* ptr,const Ty2& value){
    ::new((*void)ptr) Ty1(value);
}

template<class Ty,class... Args>
void construct(Ty* ptr,Args&&... args){
    ::new((void*)ptr) Ty(mystl::forward<Args>(args...));
}
}

#endif