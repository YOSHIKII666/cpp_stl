#ifndef MYTINYSTL_ALGOBASE_H_
#define MYTINYSTL_ALGOBASE_H_

#include<cstring>
#include "iterator.h"
#include "util.h"

namespace mystl{
    template<class T>
    const T& max(const T& lhs,const T& rhs){
        return lhs>rhs?lhs:rhs;
    }

    template<class T,class Compare>
    const T& max(const T& lhs,const T& rhs,Compare comp) {
        return comp(lhs,rhs)?rhs:lhs;
    }

    template<class T>
    const T& min(const T& lhs,const T& rhs) {
        return lhs<rhs?lhs:rhs;
    }
    template<class T,class Compare>
    const T& min(const T& lhs,const T& rhs,Compare comp) {
        return comp(rhs,lhs)?rhs:lhs;
    }

    //两个迭代器所指向的对象对调
    template<class Iter1,class Iter2>
    void iter_swap(Iter1 lhs,Iter2 rhs) {
        mystl::swap(*lhs,*rhs);
    }

    //把[first,last]拷贝到[result,result+(last-first)]
    template<class InputIter,class OutputIter>
    OutputIter unchecked_copy_cat(InputIter first,InputIter last,OutputIter result,mystl::input_iterator_tag) {
        for(;first!=last;++first,++result) {
            *result=*first;
        }
        return result;
    }

    template<class RandomIter,class OutputIter>
    OutputIter unchecked_copy_cat(RandomIter first,RandomIter last,OutputIter result,mystl::random_access_iterator_tag) {
        for(;first!=last;++first,++result) {
                *result=*first;
        }
        return result;
    }

    template<class InputIter,class OutputIter>
    OutputIter unchecked_copy(InputIter first,InputIter last,OutputIter result) {
        return unchecked_copy_cat(first,last,result,iterator_category(first));
    }

    template<class Tp,class Up>
    //只有当经过 std::remove_const 处理后的 Tp 类型和 Up 类型相同，
    //并且 Up 类型是可以平凡赋值的情况下，
    //这个 std::enable_if 结构才会定义出类型为 Up*（指向 Up 类型的指针）
    //的 type，否则（如果条件不满足），这个 type 就不存在
    typename std::enable_if<std::is_same<typename std::remove_const<Tp>::type,Up>::value &&
        std::is_trivially_assignable<Up>::value,Up*>::type
    unchecked_copy(Tp* first,Tp* last,Up* result) {
        const auto n = static_cast<size_t>(last - first);
        if(n!=0) {
            //std::memmove(void* dest,const void* src,size_t count)
            //将 count 个字节的数据从 src 所指向的内存区域复制到 dest 所指向的内存区域。
            std::memmove(result,first,n*sizeof(Up));
        }
        return result+n;
    }
}

#endif