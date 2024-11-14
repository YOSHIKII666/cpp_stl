#ifndef MYTINYSTL_UTIL_H_
#define MYTINYSTL_UTIL_H_

#include<cstddef>
#include"type_traits.h"
namespace mystl{
    /*
        forward��move������
            move�ǰѽ��յ��Ĳ���ǿ��תΪ��ֵ���ô��ݻ�ȥ,��������ֵ������ֵ���ö�����ȥ��
            forward���ڽ��ղ���ʱ����תΪ��ֵ���ã���ô��static_cast<T&&>�����У����ص���T && &,�൱��T&������ı�ԭ���Ľṹ�����³��ִ���������
    */

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

    template<class T>
    void swap(T& lhs,T& rhs) {
        //��ֵ���ݸ�ֵ����ʡ�ռ�
        auto tmp(mystl::move(lhs));
        lhs=mystl::move(rhs);
        rhs=mystl::move(tmp);
    }
}

#endif