#ifndef MYTINYSTL_EXCEPTDEF_H_
#define MYTINYSTL_EXCEPTDEF_H_

#include<stdexcept>
#include<cassert>
namespace mystl{
    //�궨��
    #define MYSTL_DEBUG(expr) assert(expr)//���ڳ�������ʱ���ж��Լ�顣�������ǶԴ���� expr�����ʽ����ֵ��������ʽ��ֵΪ�棨�� C �� C++ �У�ͨ������ֵ��ʾ�棬��ֵ��ʾ�٣�����ô�����������ִ�У���������ʽ��ֵΪ�٣�assert ��ͻ�ʹ������ִֹ�У������һЩ�����Ϣ
    #define THROW_LENGTH_ERROR_IF(expr,what) if((expr)) throw std::length_error(what);
    #define THROW_OUT_OF_RANGE_IF(expr,what) if((expr)) throw std::out_of_range(what);
    #define THROW_RUNTIME_ERROR_IF(expr,what) if((expr)) throw std::runtime_error(what);
}

#endif