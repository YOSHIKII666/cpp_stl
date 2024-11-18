//
// Created by 王少泽 on 2024/11/18.
//

#ifndef MYTINYSTL_HEAP_ALGO_H_
#define MYTINYSTL_HEAP_ALGO_H_
#include "iterator.h"

//push_heap,pop_heap,sort_heap,make_heap
namespace mystl {
    //push_heap
    template<class RandomIter,class Distance,class T>
    //topIndex表示根节点,value表示新插入节点的val,大根堆
    void push_heap_aux(RandomIter first,Distance holeIndex,Distance topIndex,T value) {
        auto parent=(holeIndex-1)/2;//表示新插入节点的父节点的坐标；
        while(holeIndex>topIndex && *(first+parent)<value) {
            *(first+holeIndex)=*(first+parent);
            holeIndex=parent;
            parent=(holeIndex-1)/2;
        }
        *(first+holeIndex)=value;//最终固定位置
    }
    template<class RandomIter,class Distance>
    void push_heap_d(RandomIter first,RandomIter last,Distance*) {
        mystl::push_heap_aux(first,(last-first)-1,static_cast<Distance>(0),*(last-1));
    }

    template<class RandomIter>
    void push_head(RandomIter first,RandomIter last) {
        mystl::push_heap_d(first,last,distance_type(first));
    }
    //自定义比较器的重载版本
    template<class RandomIter,class Distance,class T,class Compare>
    void push_heap_aux(RandomIter first,Distance holeIndex,Distance topIndex,T value,Compare comp) {
        auto parent=(holeIndex-1)/2;
        while(holeIndex>topIndex && comp(*(first+parent),value)) {
            *(first+holeIndex)=*(first+parent);
            holeIndex=parent;
            parent=(holeIndex-1)/2;
        }
        *(first+holeIndex)=value;
    }
    template<class RandomIter,class Compared,class Distance>
    void push_heap_d(RandomIter first,RandomIter last,Distance*,Compared comp) {
        mystl::push_heap_aux(first,(last-first)-1,static_cast<Distance>(0),*(last-1),comp);
    }

    template<class RandomIter,class Compared>
    void push_head(RandomIter first,RandomIter last,Compared comp) {
        mystl::push_heap_d(first,last,distance_type(first),comp);
    }

    template<class RandomIter,class T,class Distance>
    void adjust_heap(RandomIter first,Distance holeIndex,Distance len,T value) {
        auto topIndex=holeIndex;
        auto rchild=2*topIndex+2;
        while(rchild<len) {
            if(*(first+rchild)<*(first+rchild-1)) --rchild;
            *(first+holeIndex)=*(first+rchild);
            holeIndex=rchild;//下沉
            rchild=2*(rchild+1);
        }
        if(len==rchild) {
            *(first+holeIndex)=*(first+(rchild-1));
            holeIndex=rchild-1;
        }
        mystl::push_heap_aux(first,holeIndex,topIndex,value);//上浮
    }
    template<class RandomIter,class T,class Distance>
    void pop_heap_aux(RandomIter first,RandomIter last,RandomIter result,T value,Distance*) {
        *result=*first;
        mystl::adjust_heap(first,static_cast<Distance>(0),last-first,value);
    }
    template<class RandomIter>
    void pop_heap(RandomIter first,RandomIter last) {
        mystl::pop_heap_aux(first,last-1,last-1,*(last-1),distance_type(first));
    }
    // 重载版本使用函数对象 comp 代替比较操作
    template <class RandomIter, class T, class Distance, class Compared>
    void adjust_heap(RandomIter first, Distance holeIndex, Distance len, T value,
                     Compared comp)
    {
        // 先进行下溯(percolate down)过程
        auto topIndex = holeIndex;
        auto rchild = 2 * holeIndex + 2;
        while (rchild < len)
        {
            if (comp(*(first + rchild), *(first + rchild - 1)))  --rchild;
            *(first + holeIndex) = *(first + rchild);
            holeIndex = rchild;
            rchild = 2 * (rchild + 1);
        }
        if (rchild == len)
        {
            *(first + holeIndex) = *(first + (rchild - 1));
            holeIndex = rchild - 1;
        }
        // 再执行一次上溯(percolate up)过程
        mystl::push_heap_aux(first, holeIndex, topIndex, value, comp);
    }
    template <class RandomIter, class T, class Distance, class Compared>
    void pop_heap_aux(RandomIter first, RandomIter last, RandomIter result,
                      T value, Distance*, Compared comp)
    {
        *result = *first;  // 先将尾指设置成首值，即尾指为欲求结果
        mystl::adjust_heap(first, static_cast<Distance>(0), last - first, value, comp);
    }

    template <class RandomIter, class Compared>
    void pop_heap(RandomIter first, RandomIter last, Compared comp)
    {
        mystl::pop_heap_aux(first, last - 1, last - 1, *(last - 1),
                            distance_type(first), comp);
    }

    template<class RandomIter>
    void sort(RandomIter first,RandomIter last) {
        while(last-first>1) {//堆排序
            mystl::pop_heap(first,last--);
        }
    }

    template<class RandomIter,class Compared>
    void sort_heap(RandomIter first,RandomIter last,Compared comp) {
        while(last-first>1) {
            mystl::pop_heap(first,last--,comp);
        }
    }

    template <class RandomIter,class Distance>
    void make_heap_aux(RandomIter first,RandomIter last,Distance*) {
        if(last-first<2) return;
        auto len=last-first;
        auto holeIndex=(len-2)/2;
        while(true) {
            mystl::adjust_heap(first,holeIndex,len,*(first+holeIndex));
            if(holeIndex==0) return;
            holeIndex--;
        }
    }

    template<class RandomIter>
    void make_heap(RandomIter first,RandomIter last) {
        mystl::make_heap_aux(first,last,distance_type(first));
    }
    // 重载版本使用函数对象 comp 代替比较操作
    template <class RandomIter, class Distance, class Compared>
    void make_heap_aux(RandomIter first, RandomIter last, Distance*, Compared comp)
    {
        if (last - first < 2)
            return;
        auto len = last - first;
        auto holeIndex = (len - 2) / 2;
        while (true)
        {
            // 重排以 holeIndex 为首的子树
            mystl::adjust_heap(first, holeIndex, len, *(first + holeIndex), comp);
            if (holeIndex == 0)
                return;
            holeIndex--;
        }
    }

    template <class RandomIter, class Compared>
    void make_heap(RandomIter first, RandomIter last, Compared comp)
    {
        mystl::make_heap_aux(first, last, distance_type(first), comp);
    }
}


#endif //HEAP_ALGO_H
