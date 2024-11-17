//
// Created by 王少泽 on 2024/11/15.
//

#ifndef MYTINYSTL_UNINITIALIZED_H_
#define MYTINYSTL_UNINITIALIZED_H_

//用于对未初始化的空间构造元素
#include "algobase.h"
#include "construct.h"
#include "iterator.h"
#include "type_traits.h"
#include "util.h"

namespace mystl {
    //uninitialized_copy，把[first,last)上的内容复制到result处
    template<class InputIter,class ForwardIter>
    ForwardIter unchecked_uninit_copy(InputIter first,InputIter last,ForwardIter result,std::true_type) {
        return mystl::copy(first,last,result);
    }

    template<class InputIter,class ForwardIter>
    ForwardIter unckecked_uninit_copy(InputIter first,InputIter last,ForwardIter result,std::false_type) {
        auto cur=result;
        try {
            for(;first!=last;++first,++cur) {
                mystl::construct(&*cur,*first);
            }
        }catch(...) {
            for (;first!=last;++first,++cur) {
                mystl::destroy(&*cur);
            }
        }
        return cur;
    }

    template<class InputIter,class ForwardIter>
    //std::is_trivially_copy_assignable，判断类是否是平凡可复制的
    ForwardIter uninitialized_copy(InputIter first,InputIter last,ForwardIter result) {
        return mystl::unckecked_uninit_copy(first,last,result,std::is_trivially_copy_assignable<typename iterator_traits<ForwardIter>::value_type>{});
    }

    //复制[first,first+n)区间到result
    template<class InputIter,class Size,class ForwardIter>
    ForwardIter unchecked_uninit_copy_n(InputIter first,Size n,ForwardIter result,std::true_type) {
        return mystl::copy_n(first,n,result).second;
    }

    template<class InputIter,class Size,class ForwardIter>
    ForwardIter unchecked_uninit_copy_n(InputIter first,Size n,ForwardIter result,std::false_type) {
        auto cur=result;
        try {
            for(;n>0;--n,++cur,++first) {
                mystl::construct(&*cur,*first);
            }
        }catch (...) {
            for(;result!=cur;--cur) {
                mystl::destroy(&*cur);
            }
        }
        return cur;
    }

    template<class InputIter,class Size,class ForwardIter>
    ForwardIter uninitialized_copy_n(InputIter first,Size n,ForwardIter result) {
        return mystl::unchecked_uninit_copy_n(first,n,result,std::is_trivially_copy_assignable<typename iterator_traits<InputIter>::value_type>{});
    }

    //uninitialized_fill
    template<class ForwardIter,class T>
    void unchecked_uninit_fill(ForwardIter first,ForwardIter last,const T& value,std::true_type) {
        mystl::fill(first,last,value);
    }

    template<class ForwardIter,class T>
    void unchecked_uninit_fill(ForwardIter first,ForwardIter last,Const T& value,std::false_type) {
        auto cur=first;
        try {
            for(;cur!=last;++cur) {
                mystl::construct(&*cur,value);
            }
        }catch (...) {
            for(;first!=cur;++first) {
                mystl::destroy(&*first);
            }
        }
    }
     template<class ForwardIter,class T>
    void uninitialized_fill(ForwardIter first,ForwardIter last,const T& value) {
        mystl::unchecked_uninit_fill(first,last,value,std::is_trivially_copy_assignable<typename iterator_traits<ForwardIter>::value_type>{});
    }

    //有返回值的填充
    template<class ForwardIter,class Size,class T>
    ForwardIter unchecked_uninit_fill_n(ForwardIter first,Size n,const T& value,std::true_type) {
        return mystl::fill_n(first,n,value);
    }

    template<class ForwardIter,class Size,class T>
    ForwardIter uninitialized_fill_n(ForwardIter first,Size n,const T& value,std::false_type) {
        auto cur=first;
        try {
            for(;n>0;--n,++cur) {
                mystl::construct(&*cur,value);
            }
        }catch (...) {
            for(;first!=cur;++first) {
                mystl::destroy(&*first);
            }
        }
        return cur;
    }

    template<class ForwardIter,class Size,class T>
    ForwardIter uninitialized_fill_n(ForwardIter first,Size n,const T& value) {
        return mystl::unchecked_uninit_fill_n(std::is_trivially_copy_assignable<typename iterator_traits<ForwardIter>::value_type>{});
    }

    //move,把[first,last)上的内容移动到result起始的空间处
    template<class InputIter,class ForwardIter>
    ForwardIter unchecked_uninit_move(InputIter first,InputIter last,ForwardIter result,std::true_type) {
        return mystl::move(first,last,result);
    }

    template<class InputIter,class ForwardIter>
    ForwardIter unchcecked_uninit_move(InputIter first,InputIter last,ForwardIter result,std::false_type) {
        auto cur=result;
        try {
            for(;first!=last;++first,++cur) {
                mystl::construct(&*cur,mystl::move(*first));
            }
        }catch (...) {
            mystl::destroy(result,cur);
        }
        return cur;
    }

    template<class InputIter,class ForwardIter>
    ForwardIter uninitialized_move(InputIter first,InputIter last,ForwardIter result) {
        return mystl::unchcecked_uninit_move(first,last,result,std::is_trivially_copy_assignable<typename iterator_traits<ForwardIter>::value_type>{});
    }

    /*****************************************************************************************/
    // uninitialized_move_n
    // 把[first, first + n)上的内容移动到以 result 为起始处的空间，返回移动结束的位置
    /*****************************************************************************************/
    template <class InputIter, class Size, class ForwardIter>
    ForwardIter
    unchecked_uninit_move_n(InputIter first, Size n, ForwardIter result, std::true_type)
    {
        return mystl::move(first, first + n, result);
    }

    template <class InputIter, class Size, class ForwardIter>
    ForwardIter
    unchecked_uninit_move_n(InputIter first, Size n, ForwardIter result, std::false_type)
    {
        auto cur = result;
        try
        {
            for (; n > 0; --n, ++first, ++cur)
            {
                mystl::construct(&*cur, mystl::move(*first));
            }
        }
        catch (...)
        {
            for (; result != cur; ++result)
                mystl::destroy(&*result);
            throw;
        }
        return cur;
    }

    template <class InputIter, class Size, class ForwardIter>
    ForwardIter uninitialized_move_n(InputIter first, Size n, ForwardIter result)
    {
        return mystl::unchecked_uninit_move_n(first, n, result,
                                              std::is_trivially_move_assignable<
                                              typename iterator_traits<InputIter>::
                                              value_type>{});
    }
}


#endif //UNINITIALIZED_H
