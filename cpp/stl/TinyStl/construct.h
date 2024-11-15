#ifndef MYTINYSTL_CONSTRACT_H_
#define MYTINYSTL_CONSTRACT_H_

#include<new>
#include"type_traits.h"
#include"iterator.h"

namespace mystl{

template<class Ty>//��ָ�� ptr ��ָ����ڴ��ַ�ϣ�ͨ������ȫ��������� new ��������ʹ������ Ty ��Ĭ�Ϲ��캯��������һ������ Ty �Ķ���
void construct(Ty* ptr){
     ::new((void*)ptr) Ty();//::new,ȫ���������new������
}

template<class Ty1,class Ty2>
void construct(Ty1* ptr,const Ty2& value){
     ::new((void*)ptr) Ty1(value);
}

template<class Ty,class... Args>
void construct(Ty* ptr,Args&&... args){
    ::new((void*)ptr) Ty(mystl::forward<Args>(args...));//Ty(&&T) ��ֵ���õĹ��캯��
}

//��������
template<class Ty>
void destroy_one(Ty*,std::true_type){}

//���Զ�����������ԣ�type traits������ʹ�ñ�׼���ṩ�������������ж�ĳ�������Ƿ�����ض�����ʱ��std::false_type ��������������������ʾ���߱������ԡ�
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
        destroy(&*first); //first��һ����������*first��ָ�����������&*first�ǻ�ȡ���ָ��ָ���Ԫ�صĵ�ַ
    }
}
template <class Ty>
// std::is_trivially_destructible<Ty>�����ڱ������ж�����Ty�Ƿ���ƽ���������ģ�
// һ�����ͱ���Ϊ��ƽ���������ģ���ζ�Ÿ����͵���������û���û��Զ����������Ϊ��
//Ҳ������Ҫôû����ʽ����������������ʱ��������Ϊ������һ��Ĭ�ϵ��������������Ĭ����������ͨ����ִ���κ�ʵ���Բ�����ֻ���ͷŶ���ռ�õ��ڴ�ռ�ȳ��������
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