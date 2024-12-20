//
// Created by 王少泽 on 2024/12/20.
//
#ifndef MYTINYSTL_NUMERIC_H_
#define MYTINYSTL_NUMERIC_H_

// 这个头文件包含了 mystl 的数值算法

#include "iterator.h"

namespace mystl
{
    //accumulate
    //以初值init对每个元素进行累加
    template<class InputIter,class T>
    T accmulate(InputIter first,InputIter last,T init) {
        for(;first!=last;++first) {
            init += *first;
        }
        return init;
    }

    template<class InputIter,class T,class BinaryOp>
    T accmulate(InputIter first,InputIter last,T init,BinaryOp binary_op) {
        for(;first!=last;++first) {
            init=binary_op(init,*first);
        }
        return init;
    }

    //计算相邻两个元素的差值，保存到result为起始的区间上
    template<class InputIter,class OutputIter>
    OutputIter adjacent_difference(InputIter first,InputIter last,OutputIter result) {
        if(first==last) return result;
        *result=*first;
        auto value=*first;
        while(++first!=last) {
            auto tmp=*first;
            *++result=tmp-value;
            value=tmp;
        }
        return ++result;
    }

    template<class InputIter,class OutputIter,class BinaryOp>
    OutputIter adjancent_difference(InputIter first,InputIter last,OutputIter result,BinaryOp binary_op) {
        if (first == last)  return result;
        *result = *first;  // 记录第一个元素
        auto value = *first;
        while (++first != last)
        {
            auto tmp = *first;
            *++result = binary_op(tmp, value);
            value = tmp;
        }
        return ++result;
    }

    template<class InputIter1,class InputIter2,class T>
    T inner_product(InputIter1 first1,InputIter1 last1,InputIter2 first2,T init) {
        for(;first1!=last1;++first1,++first2) {
            init+=(*first1 * *first2);
        }
        return init;
    }

    template<class InputIter1,class InputIter2,class T,class BinaryOp1,class BinaryOp2>
    T inner_product(InputIter1 first1,InputIter1 last1,InputIter2 first2,T init,BinaryOp1 binary_op1,BinaryOp2 binary_op2) {
        for(;first1!=last1;++first1,++first2) {
            init=binary_op1(init,binary_op2(*first1,*first2));
        }
        return init;
    }

   template<class ForwardIter,class T>
    void iota(ForwardIter first,ForwardIter last,T value) {
        while(first!=last) {
            *first++=value;
            ++value;
        }
    }


    template <class InputIter, class OutputIter>
    OutputIter partial_sum(InputIter first, InputIter last, OutputIter result)
    {
        if (first == last)  return result;
        *result = *first;  // 记录第一个元素
        auto value = *first;
        while (++first != last)
        {
            value = value + *first;
            *++result = value;
        }
        return ++result;
    }

    // 版本2
    template <class InputIter, class OutputIter, class BinaryOp>
    OutputIter partial_sum(InputIter first, InputIter last, OutputIter result,
                           BinaryOp binary_op)
    {
        if (first == last)  return result;
        *result = *first;  //记录第一个元素
        auto value = *first;
        while (++first != last)
        {
            value = binary_op(value, *first);
            *++result = value;
        }
        return ++result;
    }

}
#endif //NUMERIC_H
