//
// Created by 王少泽 on 2024/12/1.
//

#ifndef MYTINYSTL_STACK_H_
#define MYTINYSTL_STACK_H_

// 这个头文件包含了一个模板类 stack
// stack : 栈

#include "deque.h"

namespace mystl
{
    template<class T,class Container=mystl::deque<T>>
    class stack {
    public:
        typedef Container container_type;
        typedef typename Container::value_type value_type;
        typedef typename Container::size_type size_type;
        typedef typename Container::reference reference;
        typedef typename Container::const_reference const_reference;
        static_assert(std::is_same<T,value_type>,"the value and T should be the same");
    private:
        container_type c_;
    public:
        stack()=default;

        explicit stack(size_type n):c_(n){}
        stack(size_type n,value_type& value):c_(n,value){}

        template<class IIter>
        stack(IIter first,IIter last):c_(first,last){}

        stack(std::initializer_list<T> ilist):c_(ilist.begin(),ilist.end()){}

        stack(const Container& c):c_(c){}

        stack(Container&& c) noexcept(std::is_nothrow_constructible<Container>::value):c_(mystl::move(c)){}

        stack(const stack& rhs):c_(rhs.c_){}

        stack(stack&& rhs) noexcept(std::is_nothrow_constructible<Container>::value):c_(mystl::move(rhs.c_)) {}

        stack& operator=(const stack& rhs) {
            c_=rhs.c_;
            return *this;
        }
        stack& operator=(stack&& rhs) noexcept(std::is_nothrow_assignable<Container>::value){
            c_=mystl::move(rhs.c_);
            return *this;
        }

        stack& operator=(std::initializer_list<T> ilist) {
            c_=ilist;
            return *this;
        }

        ~stack()=default;

        //栈顶是最后一个进去的元素，所以要用back来返回
        reference top() {
            return c_.back();
        }

        const_reference top() const {
            return c_.back();
        }

        bool empty() const noexcept {
            return c_.empty();
        }
        size_type size() const noexcept {
            return c_.size();
        }

        template<class... Args>
        void emplace(Args&& ...args) {
            c_.emplace_back(mystl::forward<Args>(args...));
        }

        void push(const value_type& value) {
            c_.push_back(mystl::move(value));
        }

        void pop() {
            c_.pop_back();
        }

        void clear() {
            while(!empty) {
                pop();
            }
        }

        void swap(stack& rhs) noexcept(noexcept(mystl::swap(c_, rhs.c_)))
        { mystl::swap(c_, rhs.c_); }

    public:
        friend bool operator==(const stack& lhs,const stack& rhs){
            return lhs.c_==rhs.c_;
        }

        friend bool operator<(const stack& lhs,const stack& rhs) {
            return lhs.c_<rhs.c_;
        }
    };

    // 重载比较操作符
    template <class T, class Container>
    bool operator==(const stack<T, Container>& lhs, const stack<T, Container>& rhs)
    {
        return lhs == rhs;
    }

    template <class T, class Container>
    bool operator<(const stack<T, Container>& lhs, const stack<T, Container>& rhs)
    {
        return lhs < rhs;
    }

    template <class T, class Container>
    bool operator!=(const stack<T, Container>& lhs, const stack<T, Container>& rhs)
    {
        return !(lhs == rhs);
    }

    template <class T, class Container>
    bool operator>(const stack<T, Container>& lhs, const stack<T, Container>& rhs)
    {
        return rhs < lhs;
    }

    template <class T, class Container>
    bool operator<=(const stack<T, Container>& lhs, const stack<T, Container>& rhs)
    {
        return !(rhs < lhs);
    }

    template <class T, class Container>
    bool operator>=(const stack<T, Container>& lhs, const stack<T, Container>& rhs)
    {
        return !(lhs < rhs);
    }

    // 重载 mystl 的 swap
    template <class T, class Container>
    void swap(stack<T, Container>& lhs, stack<T, Container>& rhs) noexcept(noexcept(lhs.swap(rhs)))
    {
        lhs.swap(rhs);
    }

}
#endif