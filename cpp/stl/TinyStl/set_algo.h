//
// Created by 王少泽 on 2024/12/20.
//

#ifndef MYTINYSTL_SET_ALGO_H_
#define MYTINYSTL_SET_ALGO_H_

// 这个头文件包含 set 的四种算法: union, intersection, difference, symmetric_difference
// 所有函数都要求序列有序

#include "algobase.h"
#include "iterator.h"

namespace mystl
{
    //求并集
    template<class InputIter1,class InputIter2,class OutputIter>
    OutputIter set_union(InputIter1 first1,InputIter1 last1,
        InputIter2 first2,InputIter2 last2,OutputIter result) {
        while(first1!=last1 && first2!=last2) {
            if(*first1<*first2) {
                *result=*first1;
                ++first1;
            }else if(*first2 < *first1) {
                *result=*first2;
                ++first2;
            }else {
                *result = *first1;
                ++first1;
                ++first2;
            }
            ++result;
        }
        //剩下的元素拷贝到result中
        return mystl::copy(first2,last2,mystl::copy(first1,last1,result));
    }

    //重载版本的函数使用对象comp代替比较操作

    template<class InputIter1,class InputIter2,class OutputIter,class Compare>
    OutputIter set_union(InputIter1 first1,InputIter1 last1,
                        InputIter2 first2,InputIter2 last2,
                        OutputIter result,Compare comp
    ) {
        while(first1!=last1&&first2!=last2) {
            if(comp(*first1,*first2)) {
                *result=*first1;
                ++first1;
            }
            else if(comp(*first2,*first1)) {
                *result=*first2;
                ++first2;
            }else {
                *result=*first1;
                ++first1;
                ++first2;
            }
            ++result;
        }
        return mystl::copy(first2,last2,mystl::copy(first1,last1,result));
    }

    //求交集
    template<class InputIter1,class InputIter2,class OutputIter>
    OutputIter set_intersection(InputIter1 first1,InputIter1 last1,
                                InputIter2 first2,InputIter2 last2,
                                OutputIter result
    ) {
        while (first1!=last1 && first2!=last2) {
            if(*first1<*first2) ++first1;
            else if(*first1>*first2) ++first2;
            else {
                *result=*first1;
                ++first1;
                ++first2;
                ++result;
            }
        }
        return result;
    }

    //重载，使用比较器的版本
    template<class InputIter1,class InputIter2,class OutputIter,class Compare>
    OutputIter set_intersection(InputIter1 first1,InputIter1 last1,
                                InputIter2 first2,InputIter2 last2,
                                OutputIter result,Compare comp) {
            if(comp(*first1,*first2)) ++first1;
            else if(comp(*first2,*first1)) ++first2;
            else {
                *result=*first1;
                ++first1;
                ++first2;
                ++result;
            }
        return result;
    }

    template <class InputIter1, class InputIter2, class OutputIter>
    OutputIter set_difference(InputIter1 first1, InputIter1 last1,
                              InputIter2 first2, InputIter2 last2,
                              OutputIter result)
    {
      while (first1 != last1 && first2 != last2)
      {
        if (*first1 < *first2)
        {
          *result = *first1;
          ++first1;
          ++result;
        }
        else if (*first2 < *first1)
        {
          ++first2;
        }
        else
        {
          ++first1;
          ++first2;
        }
      }
      return mystl::copy(first1, last1, result);
    }

    // 重载版本使用函数对象 comp 代替比较操作
    template <class InputIter1, class InputIter2, class OutputIter, class Compared>
    OutputIter set_difference(InputIter1 first1, InputIter1 last1,
                              InputIter2 first2, InputIter2 last2,
                              OutputIter result, Compared comp)
    {
      while (first1 != last1 && first2 != last2)
      {
        if (comp(*first1, *first2))
        {
          *result = *first1;
          ++first1;
          ++result;
        }
        else if (comp(*first2, *first1))
        {
          ++first2;
        }
        else
        {
          ++first1;
          ++first2;
        }
      }
      return mystl::copy(first1, last1, result);
    }

    /*****************************************************************************************/
    // set_symmetric_difference
    // 计算 (S1-S2)∪(S2-S1) 的结果并保存到 result 中，返回一个迭代器指向输出结果的尾部
    /*****************************************************************************************/
    template <class InputIter1, class InputIter2, class OutputIter>
    OutputIter set_symmetric_difference(InputIter1 first1, InputIter1 last1,
                                        InputIter2 first2, InputIter2 last2,
                                        OutputIter result)
    {
      while (first1 != last1 && first2 != last2)
      {
        if (*first1 < *first2)
        {
          *result = *first1;
          ++first1;
          ++result;
        }
        else if (*first2 < *first1)
        {
          *result = *first2;
          ++first2;
          ++result;
        }
        else
        {
          ++first1;
          ++first2;
        }
      }
      return mystl::copy(first2, last2, mystl::copy(first1, last1, result));
    }

    // 重载版本使用函数对象 comp 代替比较操作
    template <class InputIter1, class InputIter2, class OutputIter, class Compared>
    OutputIter set_symmetric_difference(InputIter1 first1, InputIter1 last1,
                                        InputIter2 first2, InputIter2 last2,
                                        OutputIter result, Compared comp)
    {
      while (first1 != last1 && first2 != last2)
      {
        if (comp(*first1, *first2))
        {
          *result = *first1;
          ++first1;
          ++result;
        }
        else if (comp(*first2, *first1))
        {
          *result = *first2;
          ++first2;
          ++result;
        }
        else
        {
          ++first1;
          ++first2;
        }
      }
      return mystl::copy(first2, last2, mystl::copy(first1, last1, result));
    }
}
#endif