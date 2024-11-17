#ifndef MYTINYSTL_VECTOR_H_
#define MYTINYSTL_VECTOR_H_

// ���ͷ�ļ�����һ��ģ���� vector
// vector : ����

// notes:
//
// �쳣��֤��
// mystl::vecotr<T> ��������쳣��֤�����ֺ������쳣��֤���������º�����ǿ�쳣��ȫ��֤��
//   * emplace
//   * emplace_back
//   * push_back
// �� std::is_nothrow_move_assignable<T>::value == true ʱ�����º���Ҳ����ǿ�쳣��֤��
//   * reserve
//   * resize
//   * insert

#include <initializer_list>

#include "iterator.h"
#include "memory.h"
#include "util.h"
#include "exceptdef.h"

namespace mystl{
//#ifdef ����������Ԥ����ָ���һ�֡������ڼ��ĳ����ʶ����ͨ���Ǻ����ƣ��Ƿ��Ѿ���������ˡ�
//����ñ�ʶ���Ѿ������壬��ô #ifdef ����֮��Ӧ�� #endif ֮��Ĵ���齫�ᱻ���봦��
//��֮������ñ�ʶ��δ�����壬��δ������ᱻ���������ԣ�������뵽ʵ�ʵı�������С�
#ifdef max
#pragma message("#undefing marco max")
#undef max
#endif // max

#ifdef min
#pragma message("#undefing marco min")
#undef min
#endif // min

template <class T>
class vector{
public:
    typedef mystl::allocator<T>  allocator_type;
    typedef mystl::allocator<T> data_allocator;

    typedef typename allocator_type::value_type value_type;
    typedef typename allocator_type::pointer pointer;
    

};
}

#endif