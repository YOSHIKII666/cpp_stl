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

    //�ṹ��ģ�棺pair
    //first��second�ֱ��ʾȡ����������
    template<class Ty1,class Ty2>
    struct pair {
        typedef Ty1 first_type;
        typedef Ty2 second_type;
        first_type first;
        second_type second;

        //Ĭ�Ϲ��캯��
        template<class Other1=Ty1,class Other2=Ty2,typename = typename std::enable_if<std::is_default_constructible<Other1>::value&&std::is_default_constructible<Other2>::value,void>::type>
        constexpr pair():first(),second(){}
        //��ʽ���캯��
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
        //��ʽ���캯��
        template<class U1=Ty1,class U2=Ty2,typename std::enable_if<std::is_copy_constructible<U1>::value&&std::is_copy_constructible<U2>::value&&(!std::is_convertible<const U1&,Ty1>::value||!std::is_convertible<const U2&,Ty2>::value),int>::type=0>
        explicit constexpr pair(const Ty1& a,const Ty2& b):first(a),second(b){};

        pair(const pair& rhs)=default;
        pair(pair&& rhs)=default;

        //����������ʽ���캯������ֵ���ð汾
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

        //����������ʽ���캯������ֵ���ð�
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

    // ���رȽϲ�����
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

    // ���� mystl �� swap
    template <class Ty1, class Ty2>
    void swap(pair<Ty1, Ty2>& lhs, pair<Ty1, Ty2>& rhs)
    {
        lhs.swap(rhs);
    }

    // ȫ�ֺ��������������ݳ�Ϊһ�� pair
    template <class Ty1, class Ty2>
    pair<Ty1, Ty2> make_pair(Ty1&& first, Ty2&& second)
    {
        return pair<Ty1, Ty2>(mystl::forward<Ty1>(first), mystl::forward<Ty2>(second));
    }

    // ȫ�ֺ��������������ݳ�Ϊһ�� pair
    template <class Ty1, class Ty2>
    pair<Ty1, Ty2> make_pair(Ty1&& first, Ty2&& second)
    {
        return pair<Ty1, Ty2>(mystl::forward<Ty1>(first), mystl::forward<Ty2>(second));
    }

}

#endif