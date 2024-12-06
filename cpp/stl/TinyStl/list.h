//
// Created by 王少泽 on 2024/12/4.
//

#ifndef MYTINYSTL_LIST_H_
#define MYTINYSTL_LIST_H_

// 这个头文件包含了一个模板类 list
// list : 双向链表

// notes:
//
// 异常保证：
// mystl::list<T> 满足基本异常保证，部分函数无异常保证，并对以下等函数做强异常安全保证：
//   * emplace_front
//   * emplace_back
//   * emplace
//   * push_front
//   * push_back
//   * insert

#include <initializer_list>

#include "iterator.h"
#include "memory.h"
#include "functional.h"
#include "util.h"
#include "exceptdef.h"

namespace mystl
{
    template<class T>
    struct list_node_base;

    template<class T>
    struct list_node;

    //node的特性，把两种结构包含在一个结构体中
    template<class T>
    struct node_traits {
        typedef list_node_base<T>* base_ptr;
        typedef list_node<T>* node_ptr;
    };

    template<class T>
    struct list_node_base {
        typedef typename node_traits<T>::base_ptr base_ptr;
        typedef typename node_traits<T>::node_ptr node_ptr;

        base_ptr prev; //前一个节点
        base_ptr next;//后面一个

        list_node_base()=default;

        node_ptr as_node() {
            return static_cast<node_ptr>(self());
        }

        void unlink() {
            prev=next=self();
        }

        base_ptr self() {
            return static_cast<base_ptr>(&*this);//&是取地址，返回一个指针
        }
    };

    template<class T>
    struct list_node:public list_node_base<T> {
        typedef typename node_traits<T>::base_ptr base_ptr;
        typedef typename node_traits<T>::node_ptr node_ptr;

        T value;//数据

        list_node()=default;

        list_node(const T& v):value(v){}

        list_node(T&& v):value(mystl::move(v)){}

        base_ptr as_base() {
            return static_cast<base_ptr>(&*this);
        }

        node_ptr self() {
            return static_cast<node_ptr>(&*this);
        }
    };

    //list的迭代器
    template<class T>
    struct list_iterator:public mystl::iterator<mystl::bidirectional_iterator_tag,T> {
        typedef T value_type;
        typedef T* pointer;
        typedef T& reference;
        typedef typename node_traits<T>::base_ptr base_ptr;
        typedef typename node_traits<T>::node_ptr node_ptr;
        typedef list_iterator<T> self;

        base_ptr node_; //list_node_base  <-o->  ; list_node o

        list_iterator()=default;
        list_iterator(base_ptr x):node_(x){}

        list_iterator(node_ptr x):node_(x->as_base()){}

        list_iterator(const list_iterator& rhs):node_(rhs.node_){}

        reference operator*() const {
            return node_->as_node()->value;//as_node返回list_node_base的节点本身
        }

        pointer operator->() const {
            return &(operator*());
        }

        self& operator++() {
            MYSTL_DEBUG(node_!=nullptr);
            node_=node_->next;
            return *this;
        }

        self operator++(int) {
            self temp=*this;
            ++*this;
            return temp;//pointer++(后置操作），先返回本身，再移动
        }

        self& operator--() {
            MYSTL_DEBUG(node!=nullptr);
            node_=node_->prev;
            return *this;
        }

        self operator--(int){
            self temp=*this;
            --*this;
            return temp;
        }

        bool operator==(const self& rhs) const {
            return node_==rhs.node_;
        }
        bool operator!=(const self& rhs) const {
            return node_!=rhs.node_;
        }
    };

    template <class T>
    struct list_const_iterator : public iterator<bidirectional_iterator_tag, T>
    {
        typedef T                                 value_type;
        typedef const T*                          pointer;
        typedef const T&                          reference;
        typedef typename node_traits<T>::base_ptr base_ptr;
        typedef typename node_traits<T>::node_ptr node_ptr;
        typedef list_const_iterator<T>            self;

        base_ptr node_;

        list_const_iterator() = default;
        list_const_iterator(base_ptr x)
          :node_(x) {}
        list_const_iterator(node_ptr x)
          :node_(x->as_base()) {}
        list_const_iterator(const list_iterator<T>& rhs)
          :node_(rhs.node_) {}
        list_const_iterator(const list_const_iterator& rhs)
          :node_(rhs.node_) {}

        reference operator*()  const { return node_->as_node()->value; }
        pointer   operator->() const { return &(operator*()); }

        self& operator++()
        {
            MYSTL_DEBUG(node_ != nullptr);
            node_ = node_->next;
            return *this;
        }
        self operator++(int)
        {
            self tmp = *this;
            ++*this;
            return tmp;
        }
        self& operator--()
        {
            MYSTL_DEBUG(node_ != nullptr);
            node_ = node_->prev;
            return *this;
        }
        self operator--(int)
        {
            self tmp = *this;
            --*this;
            return tmp;
        }

        // 重载比较操作符
        bool operator==(const self& rhs) const { return node_ == rhs.node_; }
        bool operator!=(const self& rhs) const { return node_ != rhs.node_; }
    };

    //list
    template<class T>
    class list {
        typedef mystl::allocator<T> allocator_type;
        typedef mystl::allocator<T> data_allocator;
        typedef mystl::allocator<list_node_base<T>> base_allocator;
        typedef mystl::allocator<list_node<T>>  node_allocator;

        typedef typename allocator_type::value_type value_type;
    };

}

#endif