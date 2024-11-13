#ifndef MYTINYSTL_UTIL_H_
#define MYTINYSTL_UTIL_H_

#include<cstddef>
#include"type_traits.h"
namespace mystl{

    template<class T>
    typename std::remove_reference<T>::type&& move(T&& arg) noexcept
    {
        return static_cast<typename std::remove_reference<T>::type&&>(arg); //������ֵ����
    }

    template<class T>//����ֵ����ת��Ϊ��ֵ���ô���ȥ
    T&& forward(typename std::remove_reference<T>::type& arg)noexcept
    {
        return static_cast<T&&>(arg);
    }
    //ͨ�� std::forward ת����������ȷ������ԭʼ��������ֵ������ֵ׼ȷ�ص��ö�Ӧ�ĺ������ذ汾��
    //�����������������ת�������µ��ò�����Ԥ�ڵĺ������������
    template<class T>
    T&& forward(typename std::remove_reference<T>::type&& arg)noexcept//
    {
        static_assert(!std::is_lvalue_reference<T>::value,"bad forward");//��֤���յ��Ĳ�������ֵ����
        return static_cast<T&&>(arg);
    }
}

#endif