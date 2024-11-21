//
// Created by 王少泽 on 2024/11/19.
//

#ifndef MYTINYSTL_DEQUE_H_
#define MYTINYSTL_DEQUE_H_

// 这个头文件包含了一个模板类 deque
// deque: 双端队列

// notes:
//
// 异常保证：
// mystl::deque<T> 满足基本异常保证，部分函数无异常保证，并对以下等函数做强异常安全保证：
//   * emplace_front
//   * emplace_back
//   * emplace
//   * push_front
//   * push_back
//   * insert

#include <initializer_list>

#include "iterator.h"
#include "memory.h"
#include "util.h"
#include "exceptdef.h"
namespace {
#ifdef max
#pragma message("#undefing marco max")
#undef max
#endif

#ifdef min
#pragma message("#undefing marco min")
#undef min
#endif
    //deque 的初始化大小
#ifndef DEQUE_MAP_INIT_SIZE
#define DEQUE_MAP_INIT_SIZE 8
#endif
    template<class T>
    struct deque_buf_size {
        static constexpr size_t value=sizeof(T)<256?4096/sizeof(T):16;
    };

    //deque的迭代器
    template<class T,class Ref,class Ptr>
    struct deque_iterator:public iterator<mystl::random_access_iterator_tag,T> {
        typedef deque_iterator<T,T&,T*>     iterator;
        typedef deque_iterator<T,const T&,const T*> const_iterator;
        typedef  deque_iterator self;

        typedef T value_type;
        typedef Ptr pointer;
        typedef Ref reference;
        typedef size_t size_type;
        typedef ptrdiff_t difference_type;
        typedef  T* value_pointer;//这里想象一级指针指向的是数组
        typedef T** map_pointer; //二级指针，每一个二级指针指向每一段数组，二级指针加一，代表一级指针移动bufer_size

        static const size_type buffer_size=deque_buf_size<T>::value;
        //迭代器成员
        value_pointer cur;//指向缓冲区的当前元素
        value_pointer first;
        value_pointer last;
        map_pointer node;//缓冲区所在的节点

        deque_iterator() noexcept:cur(nullptr),first(nullptr),last(nullptr),node(nullptr){};
        deque_iterator(value_pointer v,map_pointer n):cur(v),first(*n),last(*n+buffer_size),node(n){}
        deque_iterator(const iterator& rhs):cur(rhs.cur),first(rhs.first),last(rhs.last),node(rhs.node){}
        deque_iterator(iterator&& rhs)noexcept :cur(rhs.cur),first(rhs.first),last(rhs.last),node(rhs.node) {
            rhs.cur=nullptr;
            rhs.first=nullptr;
            rhs.last=nullptr;
            rhs.node=nullptr;
        }

        deque_iterator(const const_iterator& rhs):cur(rhs.cur),first(rhs.first),last(rhs.last),node(rhs.node){}
        self& operator=(const iterator& rhs) {
            if(this!=&rhs) {
                cur=rhs.cur;
                first=rhs.first;
                last=rhs.last;
                node=rhs.node;
            }
            return *this;
        }

        //转到另一片缓冲区
        void set_node(map_pointer new_node) {
            node=new_node;
            first=*node;
            last=first+buffer_size;
        }

        reference operator*() const {return *cur;}
        pointer operator->() const{return cur;}

        difference_type operator-(const self& x)const {
            return static_cast<difference_type>(buffer_size)*(node-x.node)+(cur-first)-(x.cur-x.first);
        }

        self& operator++() {
            ++cur;
            if(cur==last) {
                set_node(node+1);
                cur=first;
            }
            return *this;
        }

        self operator++(int) {
            self temp=*this;
            ++*this;
            return tmp;
        }
        self& operator--()
        {
            if (cur == first)
            { // 如果到达缓冲区的头
                set_node(node - 1);
                cur = last;
            }
            --cur;
            return *this;
        }
        self operator--(int)
        {
            self tmp = *this;
            --*this;
            return tmp;
        }
        self& operator+=(difference_type n) {
            const auto offset=n+(cur-first);
            if(offset>=0&&offset<static_cast<difference_type>(buffer_size)) {
                cur+=n;
            }else {
                //去别的缓冲区
                const auto node_offset=offset>0
                ?offset/static_cast<difference_type>(buffer_size)
                :-static_cast<difference_type>((-offset-1)/buffer_size)-1;
                set_node(node+node_offset);
                cur=first+(offset-node_offset * static_cast<difference_type>(buffer_size));
            }
            return *this;
        }
        self operator+(difference_type n) const {
            self temp=*this;
            return temp+=n;
        }
        self& operator-=(difference_type n) const {
            return *(this+= -n);
        }
        self operator-(difference_type n) const {
            self temp=*this;
            return temp-=n;
        }
        reference operator[](difference_type n) const{return *(*this+n);}

        //重载比较操作符
        bool operator==(const self& rhs) const {return cur==rhs.cur;}
        bool operator< (const self& rhs) const
        { return node == rhs.node ? (cur < rhs.cur) : (node < rhs.node); }
        bool operator!=(const self& rhs) const { return !(*this == rhs); }
        bool operator> (const self& rhs) const { return rhs < *this; }
        bool operator<=(const self& rhs) const { return !(rhs < *this); }
        bool operator>=(const self& rhs) const { return !(*this < rhs); }
    };
    //deque
    template<class T>
    class dequeue {
    public:
        typedef mystl::allocator<T> allocator_type;
        typedef mystl::allocator<T> data_allocator;
        typedef mystl::allocator<T*> map_allocator;

        typedef typename allocator_type::value_type value_type;
        typedef  typename allocator_type::pointer pointer;
        typedef typename allocator_type::const_pointer const_pointer;
        typedef typename allocator_type::reference reference;
        typedef typename allocator_type::const_reference const_reference;
        typedef typename allocator_type::size_type size_type;
        typedef typename allocator_type::difference_type difference_type;
        typedef pointer* map_pointer;
    };
}

#endif