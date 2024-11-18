#ifndef MYTINYSTL_ALGOBASE_H_
#define MYTINYSTL_ALGOBASE_H_

#include<cstring>
#include "iterator.h"
#include "util.h"

namespace mystl {
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

    //两个迭代器所指向的对象对�?
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
    //只有当经�? std::remove_const 处理后的 Tp 类型�? Up 类型相同�?
    //并且 Up 类型是可以平凡赋值的情况下，平凡赋值表示这个类中没有重载运算符
    //这个 std::enable_if 结构才会定义出类型为 Up*（指�? Up 类型的指针）
    //�? type，否则（如果条件不满足），这�? type 就不存在
    typename std::enable_if<std::is_same<typename std::remove_const<Tp>::type,Up>::value &&
        std::is_trivially_move_assignable<Up>::value,Up*>::type
    unchecked_copy(Tp* first,Tp* last,Up* result) {
        const auto n = static_cast<size_t>(last - first);
        if(n!=0) {
            //std::memmove(void* dest,const void* src,size_t count)
            //�? count 个字节的数据�? src 所指向的内存区域复制到 dest 所指向的内存区域�?
            std::memmove(result,first,n*sizeof(Up));
        }
        return result+n;
    }

    template <class InputIter,class OutputIter>
    OutputIter copy(InputIter first,InputIter last,OutputIter result) {
        return unchecked_copy(first,last,result);
    }

    template<class BidirectionalIter1,class BidirectionalIter2>
    BidirectionalIter2 unchecked_copy_backward_cat(BidirectionalIter1 first,BidirectionalIter1 last,BidirectionalIter2 result,mystl::bidirectional_iterator_tag) {
        while(first!=last) {
            *(--result)=*(--last);
        }
        return result;
    }

    template<class RandomIter1,class Bidirectional2>
    Bidirectional2 unchecked_copy_backward_cat(RandomIter1 first,RandomIter1 last,Bidirectional2 result,mystl::random_access_iterator_tag) {
        for(auto n=last-first;n>0;n--) {
            *(--result)=*(--last);
        }
        return result;
    }
    template<class BidirectionalIter1,class BidirectionalIter2>
    BidirectionalIter2 unchecked_copy_backward(BidirectionalIter1 first,BidirectionalIter1 last,BidirectionalIter2 result) {
        unchecked_copy_backward_cat(first,last,result,iterator_category(first));
    }

    template<class Tp,class Up>
    typename std::enable_if<std::is_same<typename std::remove_const<Tp>::type ,Up>::value && std::is_trivially_move_assignable<Up>::value,Up*>::type
    unchecked_copy_backward(Tp* first,Tp *last,Up *result) {
        const auto n=static_cast<size_t>(last-first);
        if(n!=0) {
            result-=n;
            std::memmove(result,first,n*sizeof(Up));
        }
        return result;
    }

    template<class BidirectionalIter1,class BidirectionalIter2>
    BidirectionalIter2 copy_backward(BidirectionalIter1 first,BidirectionalIter1 last,BidirectionalIter2 result) {
        return unchecked_copy_backward(first,last,result);
    }

    //copy_if,把[first,last)中满足UnaryPredicate关系的元素拷贝到result为起点的位置�?
    template<class InputIter,class OutputIter,class UnaryPredicate>
    OutputIter copy_if(InputIter first,InputIter last,OutputIter result,UnaryPredicate unary_pred) {
        for(;first!=last;++first) {
            if(unary_pred(first)) {
                *result++=*first;
            }
        }
        return result;
    }

    //copy_n,[first,first+n]拷贝到[result,result+n]上，返回一对指向尾部的指针
    template<class InputIter,class Size,class OutputIter>
    mystl::pair<InputIter,OutputIter> unchecked_copy_n(InputIter first,Size n,OutputIter result,mystl::input_iterator_tag) {
        for(;n>0;--n,++first,++result) {
            *result=*first;
        }
        return mystl::pair<InputIter,OutputIter>(first,result);
    }

    template<class RandomIter,class Size,class OutputIter>
    mystl::pair<RandomIter,OutputIter> unchecked_copy_n(RandomIter first,Size n,OutputIter result,mystl::random_access_iterator_tag) {
        auto last=first+n;
        return mystl::pair<RandomIter,OutputIter>(last,mystl::copy(first,last,result));
    }

    template<class InputIter,class Size,class OutputIter>
    mystl::pair<InputIter,OutputIter> copy_n(InputIter first,Size n,OutputIter result) {
        return unchecked_copy_n(first,n,iterator_category(first));
    }

    //move,把[first,last)区间内元素移动到[result,result+(last-first))�?
    template<class InputIter,class OutputIter>
    OutputIter unchecked_move_cat(InputIter first,InputIter last,OutputIter result,mystl::input_iterator_tag) {
        for (;first!=last;++first,++result) {
            *result=mystl::move(*first);
        }
        return result;
    }

    //randomIter的move版本
    template<class RandomIter,class OutputIter>
    OutputIter unchecked_move_cat(RandomIter first,RandomIter last,OutputIter result,mystl::random_access_iterator_tag) {
        for(auto n=last-first;n>0;--n,++first,++result) {
            *result=mystl::move(*first);
        }
        return result;
    }

    template<class InputIter,class OutputIter>
    OutputIter unchecked_move(InputIter first,InputIter last,OutputIter result) {
        return unchecked_move_cat(first,last,result,iterator_category(first));
    }

    //为trivially_copy_assignable类型提供特殊版本
    //Tp版本转为Up版本后再进行移动
    template<class Tp,class Up>
    typename std::enable_if<std::is_same
    <typename std::remove_const<Tp>::type,Up>::value
    &&std::is_trivially_move_assignable<Up>::value,Up*>::type
    unchecked_move(Tp* first,Tp* last,Up* result) {
        const size_t n=static_cast<size_t>(last-last);
        if(n!=0) {
            std::memmove(result,first,n*sizeof(Up));
        }
        return result+n;
    }

    template<class InputIter,class OutputIter>
    OutputIter move(InputIter first,InputIter last,OutputIter result) {
        return unchecked_move(first,last,result);
    }

    //move_backward,将[first,last)区间内元素移动到[result-(last-first),result]区间�?
    template<class BidirectionalIter1,class BidirectionalIter2>
    BidirectionalIter2 unchecked_move_backward_cat(BidirectionalIter1 first,BidirectionalIter1 last,BidirectionalIter2 result,mystl::bidirectional_iterator_tag) {
        while(first!=last) {
            *--result=mystl::move(*--last);
        }
        return result;
    }

    template<class RandomIter1,class RandomIter2>
    RandomIter2 unchecked_move_backward_cat(RandomIter1 first,RandomIter1 last,RandomIter2 result,mystl::random_access_iterator_tag) {
        for(auto n=last-first;n>0;--n) {
            *--result=mystl::move(*--last);
        }
        result result;
    }

    template<class BidirectionalIter1,class BidirectionalIter2>
    BidirectionalIter2 unchecked_move_backward(BidirectionalIter1 first,BidirectionalIter1 last,BidirectionalIter2 result) {
        return unchecked_move_backward_cat(first,last,result,iterator_category(first));
    }

    template<class Tp,class Up>
    typename std::enable_if<std::is_same<typename std::remove_const<Tp>::type,Up>::value && std::is_trivially_move_assignable<Up>::value,Up*>::type
    unchecked_move_backward(Tp* first,Tp* last,Up* result) {
        const size_t n=static_cast<size_t>(last-first);
        if(n!=0) {
            result-=n;
            std::memmove(result,first,n*sizeof(Up));
        }
        return result;
    }

    template<class BidirectionalIter1 ,class BidirectionalIter2>
    BidirectionalIter2 move_backward(BidirectionalIter1 first,BidirectionalIter1 last,BidirectionalIter2 result) {
        return unchecked_move_backward(first,last,result);
    }

    //equal
    template<class InputIter1,class InputIter2>
    bool equal(InputIter1 first1,InputIter1 last1,InputIter2 first2) {
        for(;first1!=last1;++first1,++first2) {
            if(*first1!=*first2) {
                return false;
            }
        }
        return true;
    }

    //自定义比较器的equal
    template<class InputIter1,class InputIter2,class Compare>
    bool equal(InputIter1 first1,InputIter1 last1,InputIter2 first2,Compare comp) {
        for(;first1!=last1;++first1,++first2) {
            if(!comp(*first1,*first2)) {
                return false;
            }
        }
        return true;
    }

    //从first 位置开始填充n个位�?
    template<class OutputIter,class Size,class T>
    OutputIter unchecked_fill_n(OutputIter first,Size n,const T& value) {
        for(;n>0;--n,++first) {
            *first=*value;
        }
        return first;
    }

    //为one-byte类型提供的版�?
    template <class Tp,class Size,class Up>
    typename std::enable_if<std::is_integral<Tp>::value && sizeof(Tp)==1 &&
        !std::is_same<Tp,bool>::value&&
            std::is_integral<Up>::value&&sizeof(Up)==1,Tp*>::type
    uncheked_fill_n(Tp* first,Size n,Up value) {
        if(n>0) {//std::memset指定起始指针，指定的value以及需要填充的长度n
            std::memset(first,(unsigned char)value,(size_t)n);
        }
        return first+n;
    }

    template<class OutputIter,class Size,class T>
    OutputIter fill_n(OutputIter first,Size n,const T& value) {
        return unchecked_fill_n(first,n,value);
    }

    //fill,为[first,last)区间中所有的元素填充新�?
    template<class ForwardIter,class T>
    void fill_cat(ForwardIter first,ForwardIter last,const T& value,mystl::forward_iterator_tag) {
        for(;first!=last;++first) {
            *first=value;
        }
    }

    template<class RandomIter,class T>
    void fill_cat(RandomIter first,RandomIter last,const T& value,mystl::random_access_iterator_tag) {
        fill_n(first,last-first,value);
    }

    template<class ForwardIter,class T>
    void fill(ForwardIter first,ForwardIter last,const T& value) {
        fill_cat(first,last,value,iterator_category(first));
    }

    //lexicographical_compare,以字典序对两个序列进行比�?
    template <class InputIter1,class InputIter2>
    bool lexicographical_compare(InputIter1 first1,InputIter1 last1, InputIter2 first2,InputIter2 last2) {
        for(;first1!=last1&&first2!=last2;++first1,++first2) {
            if(*first1<*first2) {
                return true;
            }
            if(*first2<*first1) {
                return false;
            }
        }
        return first1==last1&&first2!=last2;
    }

    //重载，实现自定义比较
    template<class InputIter1,class InputIter2,class Compared>
    bool lexicographical_compare(InputIter1 first1,InputIter1 last1,InputIter2 first2,InputIter2 last2,Compared comp) {
        for(;first1!=last1&&first2!=last2;++first1,++first2) {
            if(comp(*first1,first2)) {
                return true;
            }
            if(comp(*first2,first1)) {
                return false;
            }
        }
        return first1==last1&&first2!=last2;
    }

    //实现const unsigned char*的版�?
    bool lexicographical_compare(const unsigned char* first1,const unsigned char* last1,const unsigned char* first2,const unsigned char* last2) {
        const auto len1=last1-first1;
        const auto len2=last2-first2;
        const auto result=std::memcmp(first1,first2,mystl::min(len1,len2));
        return result!=0?result<0:len1<len2;
    }

    //mismatch ，比较两个序列，返回第一处不匹配的元�?
    template<class InputIter1,class InputIter2>
    mystl::pair<InputIter1,InputIter2> mismatch(InputIter1 first1,InputIter1 last1,InputIter2 first2) {
        while(first1!=last1&&*first1==*first2) {
            ++first1;
            ++first2;
        }
        return mystl::pair<InputIter1,InputIter2>(first1,first2);
    }
    //重载使用自定义的比较�?
    template<class InputIter1,class InputIter2,class Compare>
    mystl::pair<InputIter1,InputIter2> mismatch(InputIter1 first1,InputIter1 last1,InputIter2 first2,Compare comp) {
        while(first1!=last1&&comp(*first1,*first2)) {
            ++first1;
            ++first2;
        }
        return mystl::pair<InputIter1,InputIter2>(first1,first2);
    }
}


#endif