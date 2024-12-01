//
// Created by 王少泽 on 2024/11/29.
//

#ifndef MYTINYSTL_QUEUE_H_
#define MYTINYSTL_QUEUE_H_

// 这个头文件包含了两个模板类 queue 和 priority_queue
// queue          : 队列
// priority_queue : 优先队列

#include "deque.h"
#include "vector.h"
#include "functional.h"
#include "heap_algo.h"

namespace mystl {
    //1.数据类型。2.底层容器类型,默认用deque
    template<class T,class Container=mystl::deque<T>>
    class queue {
    public:
        typedef Container container_type;
        typedef typename Container::value_type value_type;
        typedef typename Container::size_type size_type;
        typedef typename Container::reference reference;
        typedef typename Container::const_reference const_reference;

        static_assert(std::is_same<T,value_type>::value,"the value_type and T should be same");
    private:
        container_type c_;

    public:
        queue()=default;

        explicit queue(size_type n):c_(n){}

        queue(size_type n,const value_type& value):c_(n,value){}

        template<class IIter>
        queue(IIter first,IIter last):c_(first,last){}

        queue(std::initializer_list<T> ilist):c_(ilist.begin().ilist.end()){}

        queue(const Container& c):c_(c){}

        //std::is_nothrow_move_constructible<Container>::value 的值为 true，就意味着类型 Container 具有不抛出异常的移动构造函数
        /*
        *
        *class AnotherClass {
            public:
                AnotherClass() = default;
                AnotherClass(AnotherClass&& other) {
                    // 假设这里的移动构造函数没有使用noexcept声明，并且有可能抛出异常
                    // 比如在资源转移过程中进行了一些可能失败的操作（如调用外部函数可能返回错误等）
                    throw std::runtime_error("移动构造函数出现异常");
                }
            };
            std::is_nothrow_move_constructible<AnotherClass>返回false

        class MyClass {
        public:
            MyClass() = default;
            MyClass(MyClass&& other) noexcept {
                // 假设这里进行简单的资源转移操作，不会抛出异常
                // 比如只是复制一些简单的成员变量，或者移交一些指针资源等
                std::cout << "调用MyClass的移动构造函数" << std::endl;
            }
        };
        std::is_nothrow_move_constructible<AnotherClass>返回true
         *
         */
        queue(Container&& c) noexcept(std::is_nothrow_move_constructible<Container>::value)
   :c_(mystl::move(c))
        {
        }

        queue(const queue& rhs):c_(rhs.c_){}

        queue(queue&& rhs) noexcept(std::is_nothrow_move_constructible<Container>::value)
  :c_(mystl::move(rhs.c_))
        {
        }

        queue& operator=(const queue& rhs) {
            c_=rhs.c_;
            return *this;
        }
        queue& operator=(queue&& rhs) noexcept(std::is_nothrow_assignable<Container>::value) {
            c_=mystl::move(rhs.c_);
            return *this;
        }
        queue& operator=(std::initializer_list<T> ilist) {
            c_=ilist;
            return *this;
        }
        ~queue()=default;

        reference front() {
            return c_.front();
        }
        const_reference front() const{
            return c_.front();
        }
        reference back() {
            return c_.back();
        }
        const_reference back() const {
            return c_.back();
        }

        bool empty() const noexcept {
            return c_.empty();
        }
        size_type size() const noexcept {
            return c_.size();
        }

        //修改容器操作
        template<class ...Args>
        void emplace(Args&& ...args) {
            c_.emplace_back(mystl::forward<Args>(args)...);
        }

        void push(const value_type& value) {
            c_.push_back(value);
        }

        void push(value_type&& value) {
            c_.emplac_back(mystl::move(value));
        }

        void pop() {
            c_.pop_front();
        }
        void vlear() {
            while(!empty()) {
                pop();
            }
        }
        void swap(queue& rhs) noexcept(noexcept(mystl::swap(c_, rhs.c_)))
        { mystl::swap(c_, rhs.c_); }
    public:
        friend bool operator==(const queue& lhs,const queue& rhs){return lhs.c_==rhs.c_;}
        friend bool operator<(const queue& lhs,const queue& rhs){return lhs.c_<rhs.c_;}
    };

    //container不一定是deque
    template <class T,class Container>
    bool operator==(const queue<T,Container>& lhs,const queue<T,Container>& rhs) {
        return lhs==rhs;
    }

    template<class T,class Container>
    bool operator!=(const queue<T,Container>& lhs,const queue<T,Container>& rhs) {
        return !(lhs==rhs);
    }
    template<class T,class Container>
   bool operator<(const queue<T,Container>& lhs,const queue<T,Container>& rhs) {
        return lhs<rhs;
    }
    template<class T,class Container>
   bool operator>(const queue<T,Container>& lhs,const queue<T,Container>& rhs) {
        return lhs>rhs;
    }
    template<class T,class Container>
   bool operator<=(const queue<T,Container>& lhs,const queue<T,Container>& rhs) {
        return !(lhs>rhs);
    }
    template<class T,class Container>
  bool operator>=(const queue<T,Container>& lhs,const queue<T,Container>& rhs) {
        return !(lhs<rhs);
    }
    // 重载 mystl 的 swap
    template <class T, class Container>
    void swap(queue<T, Container>& lhs, queue<T, Container>& rhs) noexcept(noexcept(lhs.swap(rhs)))
    {
        lhs.swap(rhs);
    }


    template<class T,class Container=mystl::vector<T>,
    class Compare=mystl::less<typename Container::value_type>>
    class priority_queue {
    public:
        typedef typename  Container container_type;
        typedef typename Compare value_compare;
        typedef typename Container::value_type value_type;
        typedef typename Container::size_type size_type;
        typedef typename Container::reference reference;
        typedef typename Container::const_reference reference;

        static_assert(std::is_same<T,value_type>::value,"T and value should be the same");

    private:
        container_type c_; //实现优先队列的底层容器
        value_compare comp_; //权值比较的标准
    public:
        priority_queue()=default;

        priority_queue(const Compare& c):c_(),comp_(c){};

        explicit priority_queue(size_type n):c_(n) {
            mystl::make_heap(c_.begin(),c_.end(),comp_);
        };
        priority_queue(size_type n,const value_type& value):c_(n,value) {
            mystl::make_heap(c_.begin(),c_.end(),comp_);
        }

        template<class Iter>
        priority_queue(Iter first,Iter last):c_(first,last) {
            mystl::make_heap(c_.begin(),c_.end(),comp_);
        }

        priority_queue(std::initializer_list<T> ilist):c_(ilist) {
            mystl::make_heap(c_.begin(),c_.end(),comp_);
        }
        priority_queue(const Container& s):c_(s) {
            mystl::make_heap(c_.begin(),c_.end(),comp_);
        }
        priority_queue(Container&& s):c_(mystl::move(s)) {
            mystl::make_heap(c_.begin(),c_.end(),comp_);
        }
        priority_queue(const priority_queue& rhs):c_(rhs.c_),comp_(rhs.comp_) {
            mystl::make_heap(c_.begin(),c_.end(),comp_);
        }
        priority_queue(priority_queue&& rhs):c_(mystl::move(rhs.c_)),comp_(mystl::move(rhs.comp_)) {
            mystl::make_heap(c_.begin(),c_.end(),comp);
        }

        priority_queue& operator=(const priority_queue& rhs) {
            c_=rhs.c_;
            comp_=rhs.comp_;
            mystl::make_heap(c_.begin(),c_.end(),comp_);
            return *this;
        }
        priority_queue& operator=(priority_queue&& rhs){
            c_=mystl::move(rhs.c_);
            comp_=mystl::move(rhs.comp_);
            mystl::make_heap(c_.begin(),c_.end(),comp_);
            return *this;
        }

        priority_queue& operator=(std::initializer_list<T> ilist) {
            c_=ilist;
            comp_=value_compare();
            mystl::make_heap(c_.begin(),c_.end(),comp_);
            return *this;
        }
        ~priority_queue();

    public:

        const_reference top() {
            return c_.front();
        }

        bool empty() const noexcept {
            return c_.empty();
        }
        size_type size() const noexcept {
            return c_.size();
        }

        template<class... Args>
        void emplace(Args&& ...args) {
            c_.emplace_back(mystl::forward<Args>(args));
            mystl::push_head(c_.begin(),c_.end(),comp_);
        }

        void push(const value_type& value) {
            c_.push_back(value);
            mystl::push_head(c_.begin(),c_.end(),comp_);
        }
        void pop() {
            mystl::pop_heap(c_.begin(),c_.end(),comp_);
            c_.pop_back();
        }
        void clear() {
            while(!empty()) {
                pop();
            }
        }
        void swap(priority_queue& rhs) noexcept(noexcept(mystl::swap(c_, rhs.c_)) &&
                                        noexcept(mystl::swap(comp_, rhs.comp_)))
        {
            mystl::swap(c_, rhs.c_);
            mystl::swap(comp_, rhs.comp_);
        }

    public:
        friend bool operator==(const priority_queue& lhs,const priority_queue& rhs) {
            return lhs.c_==rhs.c_;
        }
        friend bool operator!=(const priority_queue& lhs,const priority_queue& rhs) {
            return lhs.c_!=rhs.c_;
        }
    };

    // 重载比较操作符
    template <class T, class Container, class Compare>
    bool operator==(priority_queue<T, Container, Compare>& lhs,
                    priority_queue<T, Container, Compare>& rhs)
    {
        return lhs == rhs;
    }

    template <class T, class Container, class Compare>
    bool operator!=(priority_queue<T, Container, Compare>& lhs,
                    priority_queue<T, Container, Compare>& rhs)
    {
        return lhs != rhs;
    }

    // 重载 mystl 的 swap
    template <class T, class Container, class Compare>
    void swap(priority_queue<T, Container, Compare>& lhs,
              priority_queue<T, Container, Compare>& rhs) noexcept(noexcept(lhs.swap(rhs)))
    {
        lhs.swap(rhs);
    }

}
#endif //QUEUE_H
