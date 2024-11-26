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
#include <iterator>
#include <utility>

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
        value_pointer first;//指向所在缓冲区头部
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
    class deque {
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
        typedef const_pointer* const_map_pointer;

        typedef deque_iterator<T,T&,T*> iterator;
        typedef dequeue_iterator<T,const T&,const T*> const_iterator;
        typedef mystl::reverse_iterator<iterator> reverse_iterator;
        typedef mystl::reverse_iterator<const_iterator> const_reverse_iterator;

        allocator_type get_allocator(){return allocator_type();}

        static const size_type buffer_size=deque_buf_size<T>::value;
    private:
        //迭代器的四个位置
        iterator begin_;
        iterator end_;
        map_pointer map_;// 每个map都指向一个缓冲区
        size_type map_size_;//map中的指针数量
    public:
        deque(){fill_init(0,value_type());}

        explicit deque(size_type n){fill_init(n,value_type());}
        deque(size_type n,const value_type& value){fill_init(n,value);}

        template<class IIter,typename std::enable_if<mystl::is_input_iterator<IIter>::value,int>::type=0>
        deque(IIter first,IIter,last){copy(first,last,mystl::iterator_category(first));}

        deque(std::initializer_list<value_type> ilist) {
            copy_init(ilist.begin(),ilist.end(),mystl::forward_iterator_tag());
        }

        deque(const deque& rhs) {
            copy_init(ilist.begin(),rhs.end(),mystl::forward_iterator_tag());
        }

        deque(deque&& rhs) noexcept
           :begin_(mystl::move(rhs.begin_)),
           end_(mystl::move(rhs.end_)),
           map_(rhs.map_),
           map_size_(rhs.map_size_)
        {
            rhs.map_ = nullptr;
            rhs.map_size_ = 0;
        }

        deque& operator=(const deque& rhs);
        deque& operator=(deque&& rhs);

        deque& operator=(std::initializer_list<value_type> ilist) {
            deque tmp(ilist);
            swap(tmp);
            return *this;
        }

        ~deque() {
            if(map_!=nullptr) {
                clear();
                data_allocator::deallocate(*begin_.node,buffer_size);
                *begin_.node=nullptr;
                map_allocator::deallocate(map_,map_size_);
                map_=nullptr;
            }
        }

    public: //迭代器相关
        iterator begin() noexcept{return begin_;}
        const_iterator begin() const noexcept{return begin_;}
        iterator end() noexcept{return end_;}
        const_iterator end() const noexcept{return end_;}

        reverse_iterator rbegin() noexcept{return reverse_iterator(end());}
        const_reverse_iterator rbegin() const noexcept{return reverse_iterator(end());}
        reverse_iterator rend() noexcept{return reverse_iterator(begin());}
        const_reverse_iterator rend() const noexcept{return reverse_iterator(begin());}

        const_iterator cbegin() const noexcept{return begin();}
        const_iterator cend() const noexcept{return end();}
        const_reverse_iterator crbegin() const noexcept{return rbegin();}
        const_reverse_iterator crend() const noexcept{return rend();}

        //容量相关操作
        bool empty() const noexcept {return begin()==end();}
        size_type size() const noexcept{return end_-begin_;}
        size_type max_size() const noexcept{return static_cast<size_type>(-1);}
        void resize(size_type new_size){resize(new_size,value_type());}
        void resize(size_type new_size,const value_type& value);
        void shrink_to_fit() noexcept;

        reference operator[](size_type n) {
            MYSTL_DEBUG(n<size());
            return begin_[n];
        }
        const_reference operator[](size_type n) const {
            MYSTL_DEBUG(n<size());
            return begin_[n];
        }
        reference at(size_type n) {
            THROW_OUT_OF_RANGE_IF(!(n<size()),"out of range!!!");
            return (*this)[n];
        }
        reference at(size_type n) const {
            THROW_OUT_OF_RANGE_IF(!(n<size()),"out of range!!");
            return (*this)[n];
        }
        reference front() {
            MYSTL_DEBUG(!empty());
            return *begin();
        }
        const_reference front() const {
            MYSTL_DEBUG(!empty());
            return *begin();
        }
        reference back() {
            MYSTL_DEBUG(!empty());
            return *(end()-1);
        }
        const_reference back() const {
            MYSTL_DEBUG(!empty());
            return *(end()-1);
        }

        //修改容器
        void assign(size_type n,const value_type& value)
        {fill_assign(n,value);}

        template<class IIter,typename std::enable_if<
            mystl::is_input_iterator<IIter>::value,int>::type=0>
        void assign(IIter first,IIter last) {
            copy_assign(first,last,mystl::iterator_category(first));
        }

        void assign(std::initializer_list<value_type> ilist) {
            copy_assign(ilist.begin(),ilist.end(),mystl::forward_iterator_tag{});
        }

        template<class ...Args>
        void emplace_front(Args&& ...args);
        template <class ...Args>
        void emplace_back(Args&& ...args);
        template<class ...Args>
        iterator emplace(iterator pos,Args&& ...args);

        // push_front / push_back

        void     push_front(const value_type& value);
        void     push_back(const value_type& value);

        void     push_front(value_type&& value) { emplace_front(mystl::move(value)); }
        void     push_back(value_type&& value)  { emplace_back(mystl::move(value)); }

        // pop_back / pop_front

        void     pop_front();
        void     pop_back();

        // insert

        iterator insert(iterator position, const value_type& value);
        iterator insert(iterator position, value_type&& value);
        void     insert(iterator position, size_type n, const value_type& value);
        template <class IIter, typename std::enable_if<
          mystl::is_input_iterator<IIter>::value, int>::type = 0>
        void     insert(iterator position, IIter first, IIter last)
        { insert_dispatch(position, first, last, iterator_category(first)); }

        // erase /clear

        iterator erase(iterator position);
        iterator erase(iterator first, iterator last);
        void     clear();

        // swap

        void     swap(deque& rhs) noexcept;

    private:
        // helper functions

        // create node / destroy node
        map_pointer create_map(size_type size);
        void        create_buffer(map_pointer nstart, map_pointer nfinish);
        void        destroy_buffer(map_pointer nstart, map_pointer nfinish);

        // initialize
        void        map_init(size_type nelem);
        void        fill_init(size_type n, const value_type& value);
        template <class IIter>
        void        copy_init(IIter, IIter, input_iterator_tag);
        template <class FIter>
        void        copy_init(FIter, FIter, forward_iterator_tag);

        // assign
        void        fill_assign(size_type n, const value_type& value);
        template <class IIter>
        void        copy_assign(IIter first, IIter last, input_iterator_tag);
        template <class FIter>
        void        copy_assign(FIter first, FIter last, forward_iterator_tag);

        // insert
        template <class... Args>
        iterator    insert_aux(iterator position, Args&& ...args);
        void        fill_insert(iterator position, size_type n, const value_type& x);
        template <class FIter>
        void        copy_insert(iterator, FIter, FIter, size_type);
        template <class IIter>
        void        insert_dispatch(iterator, IIter, IIter, input_iterator_tag);
        template <class FIter>
        void        insert_dispatch(iterator, FIter, FIter, forward_iterator_tag);

        // reallocate
        void        require_capacity(size_type n, bool front);
        void        reallocate_map_at_front(size_type need);
        void        reallocate_map_at_back(size_type need);
    };
    //A=rhs
    template<class T>
       deque<T>& deque<T>::operator=(const deque& rhs) {
        if(this!=&rhs) {
            const auto len=size();
            if(len>=rhs.size()) {
                erase(mystl::copy(rhs.begin_,rhs.end_,begin_),end_);
            }
            else {
                iterator mid=rhs.begin()+static_cast<difference_type>(len);
                mystl::copy(rhs.begin_,mid,begin_);
                insert(end_,mid,rhs.end_);
            }
        }
        return *this;
    }
    //A=rhs;
    template<class T>
    deque<T>& deque<T>::operator=(deque&& rhs) {
        clear();
        begin_=mystl::move(rhs.begin_);
        end_=mystl::move(rhs.end_);
        map_=rhs.map_;
        map_size_=rhs.map_size_;
        rhs.map_=nullptr;
        rhs.map_size_=0;
        return *this;
    }
    //重置容器大小
    template<class T>
    void deque<T>::resize(size_type new_size,const value_type& value) {
        const auto len=size();
        if(new_size<len) {
            erase(begin_+new_size,end_);//删除这一段
        }
        else {
            insert(end_,new_size-len,value);
        }
    }

    //减小容器容量
    template<class T>
    void deque<T>::shrink_to_fit() noexcept {
        //map_这段数组中，begin_不一定是开头，end_也不一定是结尾
        for(auto cur=map_;cur<begin_.node;cur++) {
            data_allocator::deallocate(*cur,buffer_size);
            *cur=nullptr;
        }
        for(auto cur=end_.node;cur<map_+map_size_;cur++) {
            data_allocator::deallocate(*cur,buffer_size);
            *cur=nullptr;
        }
    }

    //在头部就地构建元素
    template<class T>
    template<class ...Args>
    void deque<T>::emplace_front(Args&& ...args) {
        if(begin_.cur!=begin_.first) {
            data_allocator::construct(begin_.cur-1,mystl::forward<Args>(args)...);
            --begin_.cur;
        }
        else {
            require_capacity(1,true);
            try {
                --begin_;
                data_allocator::construct(begin_.cur,mystl::forward<Args>(args)...);
            }catch(...){
                ++begin_;
                throw;
            }
        }
    }
    //在尾部构建元素
    template<class T>
    template<class ...Args>
    void deque<T>::emplace_back(Args&& ... args) {
        if(end_.cur!=end_.last-1) {
            data_allocator::construct(end_.cur,mystl::forward<Args>(args)...);
            ++end_.cur;
        }
        else {
            require_capacity(1,false);
            data_allocator::construct(end_.cur,mystl::forward<Args>(args)...);
            ++end_;
        }
    }

    //在pos处就地构造
    template<class T>
    template<class ...Args>
    typename deque<T>::iterator deque<T>::emplace(iterator pos,Args&& ...args) {
        if(pos.cur==begin_.cur) {
            emplace_front(mystl::forward<Args>(args)...);
            return begin_;
        }
        else if(pos.cur==end.cur) {
            emplace_back(mystl::forward<Args>(args)...);
            return end_-1;
        }
        return insert_aux(pos,mystl::forward<Args>(args)...);
    }
    //在头部插入元素
    template<class T>
    void deque<T>::push_front(const value_type& value) {
        if(begin_.cur!=begin_.first) {
            data_allocator::construct(begin_.cur-1,value);
            --begin_.cur;
        }else {
            require_capacity(1,true);
            try {
                --begin_;
                data_allocator::construct(begin_.cur,value);
            }
            catch(...) {
                ++begin_;
                throw;
            }
        }
    }

    //在尾部插入元素
    template<class T>
    void deque<T>::push_back(const value_type& value) {
        if(end_.last-1!=end_.cur) {
            data_allocator::construct(end_.cur,value);
            ++end_.cur;
        }else {
            require_capacity(1,false);
            data_allocator::construct(end_.cur,value);
            ++end_;
        }
    }

    //弹出头部元素
    template<class T>
    void deque<T>::pop_front() {
        MYSTL_DEBUG(!empty());
        if(begin_.cur!=begin_.last-1) {
            data_allocator::destroy(begin_.cur);
            ++begin_.cur;
        }else {
            data_allocator::destroy(begin_.cur);
            ++begin_;
            destroy_buffer(begin_.node-1,begin_.node-1);
        }
    }

    //弹出尾部元素
    template<class T>
    void deque<T>::pop_back() {
        if(end_.cur!=end_.last) {
            --end_.cur;
            data_allocator::destroy(end_.cur);
        }else {
            --end_;
            data_allocator::destroy(end_.cur);
            destroy_buffer(end_.node+1,end_.node+1);
        }
    }

    //在position处插入
    template<class T>
    typename deque<T>::iterator deque<T>::insert(iterator position,const value_type& value) {
        if(position.cur==begin_.cur) {
            push_front(value);
            return begin_;
        }
        else if(position.cur==end_.cur) {
            push_back(value);
            auto tmp=end_;
            --tmp;
            return tmp;
        }else {
            return insert_aux(position,value);
        }
    }
    template<class T>
    typename deque<T>::iterator deque<T>::insert(iterator position,value_type&& value) {
        if(position.cur=begin_.cur) {
            emplace_front(mystl::move(value));
            return begin_;
        }else if(position.cur==end_.cur) {
            emplace_back(mystl::move(value));
            auto temp=end_;
            --tmp;
            return tmp;
        }else {
            return insert_aux(position,mystl::move(value));
        }
    }

    //在position位置插入n个元素
    template<class T>
    void deque<T>::insert(iterator position,size_type n,const value_type& value) {
        if(position.cur==begin_.cur) {
            require_capacity(n,true);
            auto new_begin=begin_-n;
            mystl::uninitialized_fill_n(new_begin,n,value);
            begin_=new_begin;
        }
        else if(position.cur==end_.cur) {
            require_capacity(n,false);
            auto new_end=end_+n;
            mystl::uninitialized_fill_n(end_,n,value);
            end_=new_end;
        }else {
            fill_insert(position,n,value);
        }
    }

    //删除position处的元素
    template<class T>
    typename deque<T>::iterator deque<T>::erase(iterator position) {
        auto next=position;
        ++next;
        const size_type elems_before=position-begin_;
        if(elems_before<(size()/2)) {
            mystl::copy_backward(begin_,position,next);//--position=--next
            pop_front();
        }else {
            mystl::copy(next,end_,position);
            pop_back();
        }
        return begin_+elems_before;
    }

    //删除[first,last)中的元素
    template<class T>
    typename deque<T>::iterator deque<T>::erase(iterator first,iterator last) {
        if(first==begin_&&last==end_) {
            clear();
            return end_;
        }
        else {
            const size_type len=last-first;
            const size_type elems_before=first-begin_;
            if(elems_before<((size()-len)/2)) {
                mystl::copy_backward(begin_,first,last);
                auto new_begin=begin_+len;
                data_allocator::destroy(begin_.cur,new_begin.cur);
                begin_=new_begin;
            }else {
                mystl::copy(last,end_,first);
                auto new_end=end_-len;
                data_allocator::destroy(new_end.cur,end_.cur);
                end_=new_end;
            }
        }
    }
    template<class T>
    void deque<T>::clear() {
        for(map_pointer cur=begin_.node+1;cur<end_.node;++cur) {
            data_allocator::destroy(*cur,*cur+buffer_size);
        }
        if(begin_.node!=end_.node) {
            //有两个以上的缓冲区
            mystl::destroy(begin_.cur,begin_.last);
            mystl::destroy(end_.first,end_.cur);
        }else {
            mystl::destroy(begin_.cur,end_.cur);
        }
        shrink_to_fit();
        end_=begin_;
    }

    template<class T>
    void deque<T>::swap(deque& rhs) noexcept {
        mystl::swap(begin_, rhs.begin_);
        mystl::swap(end_, rhs.end_);
        mystl::swap(map_, rhs.map_);
        mystl::swap(map_size_, rhs.map_size_);
    }

    template<class T>
    typename deque<T>::map_pointer deque<T>::create_map(size_type size) {
        map_pointer mp=nullptr;
        mp=map_allocator::allocate(size);
        for(size_type i=0;i<size;++i) {
            *(mp+i)=nullptr;
        }
        return mp;
    }

    template<class T>
    void deque<T>::create_buffer(map_pointer nstart,map_pointer nfinish) {
        map_pointer cur;
        try {
            for(cur=nstart;cur<=nfinish;++cur) {
                *cur=data_allocator::allocate(buffer_size);//申请数组空间
            }
        }catch (...) {
            while(cur!=nstart) {
                --cur;
                data_allocator::deallocate(*cur,buffer_size);
                *cur=nullptr;
            }
            throw;
        }
    }

    template<class T>
    void deque<T>::destroy_buffer(map_pointer nstart,map_pointer nfinish) {
        for(map_pointer n=nstart;n<=nfinish;++n) {
            data_allocator::deallocate(*n,buffer_size);
            *n=nullptr;
        }
    }

    template<class T>
    void deque<T>::map_init(size_type nElem) {
        const size_type nNode=nElem/buffer_size+1;//需要分配的缓冲区个数
        map_size_=mystl::max(static_cast<size_type>(DEQUE_MAP_INIT_SIZE),nNode+2);
        try {
            map_=create_map(map_size_);
        }catch(...) {
            map_=nullptr;
            map_size_=0;
            throw;
        }

        map_pointer nstart=map_+(map_size_-nNode)/2;
        map_pointer nfinish=nstart+nNode-1;
        try {
            create_buffer(nstart,nfinish);
        }catch (...) {
            map_allocator::deallocate(map_,map_size_);
            map_=nullptr;
            map_size_=0;
            throw;
        }
        begin_.set_node(nstart);
        end_.set_node(nfinish);
        begin_.cur=begin_.first;
        end_.cur=end.first+(nElem%buffer_size);
    }

    template<class T>
    void deque<T>::fill_init(size_type n,const value_type& value) {
        map_init(n);
        if(n!=0) {
            for(auto cur=begin_.node;cur<end_.node;++cur) {
                mystl::uninitialized_fill(*cur,*cur+buffer_size,value);
            }
            mystl::uninitialized_fill(end_.first,end_.cur,value);
        }
    }

    template<class T>
    template<class IIter>
    void deque<T>::copy_init(IIter first,IIter last,mystl::input_iterator_tag) {
        const size_type n=mystl::distance(first,last);
        map_init(n);
        for(;first!=last;++first) {
            emplace_back(*first);
        }
    }

    //把[first,last]中的内容复制过来
    template<class T>
    template<class FIter>
    void deque<T>::copy_init(FIter first,FIter last,mystl::forward_iterator_tag) {
        const size_type n=mystl::distance(first,last);
        map_init(n);
        for(auto cur=begin_.node;cur<end_.node;++cur) {
            auto next=first;
            mystl::advance(next,buffer_size);
            mystl::uninitialized_copy(first,next,*cur);
            first=next;
        }
        mystl::uninitialized_copy(first,last,end_.first);
    }

    template<class T>
    void deque<T>:: fill_assign(size_type n,const value_type& value) {
        if(n>size()) {
            mystl::fill(begin(),end(),value);
            insert(end(),n-size(),value);
        }else {
            erase(begin()+n,end());
            mystl::fill(begin(),end(),value);
        }
    }

    //将first和last这部分内容复制到当前调用的deque
    //input型迭代器
    template<class T>
    template<class IIter>
    void deque<T>::copy_assign(IIter first,IIter last,mystl::input_iterator_tag) {
        auto first1=begin();
        auto last1=end();
        for(;first!=last&&first1!=last1;++first,++first1) {
            *first1=*first;
        }
        if(first1!=last1) {
            erase(first1,last1);//清除掉多余的这部分
        }
        else {
            insert_dispatch(end_,first,last,mystl::input_iterator_tag{});
        }
    }

    //forward型迭代器
    template<class T>
    template<class FIter>
    void deque<T>::copy_assign(FIter first,FIter last,mystl::forward_iterator_tag) {
        const size_type len1=size();
        const size_type len2=mystl::distance(first,last);
        if(len1<len2) {
            auto next=first;
            mystl::advance(next,len1);
            mystl::copy(first,next,begin_);//先把原deque撑满
            insert_dispatch(end_,next,last,mystl::forward_iterator_tag{});
        }else {
            erase(mystl::copy(first,last,begin_),end_);
        }
    }

    template<class T>
    template<class... Args>
    typename deque<T>::iterator deque<T>::insert_aux(iterator position,Args&& ...args) {
        const size_type elems_before=position-begin_;
        value_type value_copy=value_type(mystl::forward<Args>(args)...);
        if(elems_before<(size()/2)){//在前半段插入
            emplace_front(front());
            auto front1=begin_;
            ++front1;
            auto front2=front1;
            ++front2;
            position =begin_+elems_before;
            auto pos=position;
            ++pos;
            mystl::copy(front2,pos,front1);
        }else {
            emplace_back(back());
            auto end1=end_;
            --end1;
            auto end2=end1;
            --end2;
            position=begin_+elems_before;
            mystl::copy_backward(pos,end2,end1);
        }
        *position=mystl::move(value_copy);
        return position;
    }
}



#endif