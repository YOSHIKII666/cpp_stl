#ifndef MYTINYSTL_CONSTRACT_H_
#define MYTINYSTL_CONSTRACT_H_

#include<new>
#include"type_traits.h"
#include"iterator.h"

namespace mystl{

template<class Ty>//��ָ�� ptr ��ָ����ڴ��ַ�ϣ�ͨ������ȫ��������� new ��������ʹ������ Ty ��Ĭ�Ϲ��캯��������һ������ Ty �Ķ���
void construct(Ty* ptr){
    ::new((*void) ptr) Ty();//::new,ȫ���������new������
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