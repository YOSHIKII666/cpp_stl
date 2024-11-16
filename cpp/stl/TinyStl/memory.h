//
// Created by 王少泽 on 2024/11/16.
//

#ifndef MYTINYSTL_MEMORY_H_
#define MYTINYSTL_MEMORY_H_

#include <cstddef>
#include <cstdlib>
#include <climits>
#include <new>

#include "algobase.h"
#include "allocator.h"
#include "construct.h"
#include "uninitialized.h"

namespace mystl {
    //获取对象地址
    template<class Tp>
    //constexpr修饰，可在编译期就得到值
    constexpr Tp* address_of(Tp& value) noexcept {
        return &value;
    }

    //获取，释放 临时缓冲区
    template<class T>
    //ptrdiff_t型变量用于表示两个指针之间的差值
    pair<T*,ptrdiff_t> get_buffer_helper(ptrdiff_t len,T*) {
        if(len>static_cast<ptrdiff_t>(INT_MAX/sizeof(T))) {
            len=INT_MAX/sizeof(T);
        }
        while (len>0) {
            T* tmp=static_cast<T*>(malloc(static_cast<size_t>(len)*sizeof(T)));
            if(tmp) {
                return pair<T*,ptrdiff_t>(tmp,len);
            }
            len/=2;//申请失败时，减少len的大小
        }
        return pair<T*,ptrdiff_t>(nullptr,0);
    }

    template<class T>
    pair<T*,ptrdiff_t> get_temporary_buffer(ptrdiff_t len) {
        return get_buffer_helper(len,static_cast<T*>(0));
    }

    template<class T>
    pair<T*,ptrdiff_t> get_temporary_buffer(ptrdiff_t len) {
        return get_buffer_helper(len,static_cast<T*>(0));
    }

    template<class T>
    void release_temporary_buffer(T* ptr) {
        free(ptr);
    }

    template<class ForwardIterator,class T>
    class temporary_buffer {
    private:
        ptrdiff_t original_len; //申请的大小
        ptrdiff_t len; //实际的大小
        T* buffer; //指向缓冲区的指针

    public:
        temporary_buffer(ForwardIterator first,ForwardIterator last);
        ~temporary_buffer() {
            mystl::destroy(buffer,buffer+len);//调用析构函数
            free(buffer);//释放内存
        }
    public:
        ptrdiff_t size() const noexcept{return len;}
        ptrdiff_t request_size() const noexcept{return original_len;}
        T* begin() noexcept{return buffer;}
        T* end() noexcept{return buffer+len;}
    private:
        void allocate_buffer();
        void initialize_buffer(const T&,std::true_type){}
        void initialize_buffer(const T& value,std::false_type) {
            mystl::uninitialized_fill_n(buffer,len,value);
        }
    private:
        temporary_buffer(const temporary_buffer&);
        void operator=(const temporary_buffer&);
    };

    template<class ForwardIterator, class T>
    temporary_buffer<ForwardIterator, T>::temporary_buffer(ForwardIterator first, ForwardIterator last) {
            try {
                len=mystl::distance(first,last);
                allocate_buffer();
                if(len>0) {
                    initialize_buffer(*first,std::is_trivially_default_constructible<T>());
                }
            }catch (...) {
                free(buffer);
                buffer=nullptr;
                len=0;
            }
    }

    //allocate_buffer函数的实现
    template<class ForwardIterator, class T>
    void temporary_buffer<ForwardIterator, T>::allocate_buffer() {
        original_len=len;
        if(len>static_cast<ptrdiff_t>(INT_MAX/sizeof(T))) {
            len=INT_MAX/sizeof(T);
        }
        while(len>0) {
            buffer=static_cast<ptrdiff_t>(malloc(len*sizeof(T)));
            if(buffer) break;
            len/=2;
        }
    }

    //小型智能指针
    template<class T>
    class auto_ptr {
    public:
        typedef T elem_type;
    private:
        T* m_ptr;//实际指针
    public:
        //构造，析构，复制函数
        explicit auto_ptr(T* p=nullptr):m_ptr(p){}
        auto_ptr(auto_ptr& rhs):m_ptr(rhs.realse()){}
        template<class U>
        auto_ptr(auto_ptr<U> rhs):m_ptr(rhs.realse()){}

        auto_ptr& operator=(auto_ptr& rhs) {
            if(this!=&rhs) {
                delete m_ptr;
                m_ptr=rhs.realse();
            }
            return *this;
        }
        template<class U>
        auto_ptr& operator=(auto_ptr<U>& rhs) {
            if(this->get()!=rhs.get()) {
                delete m_ptr;
                m_ptr=rhs.realse();
            }
            return *this;
        }
        ~auto_ptr(){delete m_ptr;}

    public:
        T& operator*() const{return *m_ptr;}
        T* operator->() const{return m_ptr;}

        //获取指针
        T* get() const{return m_ptr;}
        //释放指针
        T* realse() {
            T* tmp=m_ptr;
            m_ptr=nullptr;
            return tmp;
        }
        //重置指针
        void reset(T* p=nullptr) {
            if(m_ptr!=p) {
                delete m_ptr;
                m_ptr=p;
            }
        }
    };

}

#endif //MEMORY_H
