#ifndef MYTINYSTL_VECTOR_H_
#define MYTINYSTL_VECTOR_H_

// 这个头文件包含一个模板类 vector
// vector : 向量

// notes:
//
// 异常保证：
// mystl::vecotr<T> 满足基本异常保证，部分函数无异常保证，并对以下函数做强异常安全保证：
//   * emplace
//   * emplace_back
//   * push_back
// 当 std::is_nothrow_move_assignable<T>::value == true 时，以下函数也满足强异常保证：
//   * reserve
//   * resize
//   * insert

#include <initializer_list>

#include "iterator.h"
#include "memory.h"
#include "util.h"
#include "exceptdef.h"

namespace mystl{
//#ifdef 是条件编译预处理指令的一种。它用于检查某个标识符（通常是宏名称）是否已经被定义过了。
//如果该标识符已经被定义，那么 #ifdef 和与之对应的 #endif 之间的代码块将会被编译处理；
//反之，如果该标识符未被定义，这段代码块则会被编译器忽略，不会参与到实际的编译过程中。
#ifdef max
#pragma message("#undefing marco max")
#undef max
#endif // max

#ifdef min
#pragma message("#undefing marco min")
#undef min
#endif // min

template <class T>
class vector{
    //try_init(),用于分配初始空间，默认大小为16
    //init_space(size,cap),默认分配cap个cap_,size个size_
    //fill_init(n,value),按照n和16的最大值初始化一片区域，并且给前n个值赋值为value
    //range_init(first,last),按照last-first和16的最大值初始化一片区域并且给first到last个值赋值为和first相同
    //destroyed_and_recover(first,last,n),调用析构函数，释放内存。
    //get_new_cap(add_size),如果1.5*old_size>max_size()的前提下，old_size+add_size>max-16那么返回old+add,反之返回old+add+16;如果old_size=0,返回16和add的最大值，反之返回1.5old_size和old+add的最大值
    //fill_assign(n,value)，如果n>capacity()，那么重新初始化一片区域，n个赋值为value;若n>size()把begin到end赋值为value,end_以及多出的赋值为value；如果n<capacity(),先用value填充n个位置后，清除到end_区间内的内容。
    //copy_assign(first,last,input_iterator_tag),给cur赋值为first,二者同时前进，直到first=last,然后erase多出的内容
    //copy_assign(first,last,forward_iterator_tag),计算first和last之间的距离len，如果len>capacity(),重新申请n个value，swap;如果size()>=len,把原vector中前len个内容以first赋值，然后摧毁后面的内容；如果len<capacity,复制操作
    //reallocate_emplace(pos,args),重新分配一片区域，并且在pos位置上插入args
    //reallocate_insert(pos,value),重新分配一片区域，并在pos处插入value
    //fill_insert(pos,n,value), ===>insert
    //copy_insert===>insert
public:
    typedef mystl::allocator<T>  allocator_type;
    typedef mystl::allocator<T> data_allocator;

    typedef typename allocator_type::value_type value_type;
    typedef typename allocator_type::pointer pointer;
    typedef typename allocator_type::const_pointer const_pointer;
    typedef typename allocator_type::reference reference;
    typedef typename allocator_type::const_reference const_reference;
    typedef typename allocator_type::size_type size_type;
    typedef typename allocator_type::difference_type difference_type;

    typedef value_type* iterator;
    typedef const value_type* const_iterator;
    typedef mystl::reverse_iterator<iterator> reverse_iterator;
    typedef mystl::reverse_iterator<const_iterator> const_reverse_iterator;

    allocator_type get_allocator(){return data_allocator();}

private:
        //begin_ ... end_ ... cap_
    iterator begin_;//表示目前使用空间的头部
    iterator end_;//表示目前使用空间的尾部
    iterator cap_;//表示目前存储的尾部

public:
    vector() noexcept {
        try_init();
    }
    explicit vector(size_type n)//explicit禁止构造函数发生隐式转换
    {
        fill_init(n,value_type());
    }
    vector(size_type n,const value_type& value) {
        fill_init(n,value);
    }
    template<class Iter,typename std::enable_if<mystl::is_input_iterator<Iter>::value,int>::type=0>
    vector(Iter first,Iter last) {
        MYSTL_DEBUG(!(last<first));
        range_init(first,last);
    }
    vector(const vector& rhs) {
        range_init(rhs.begin_,rhs.end_);
    }

    vector(vector&& rhs) noexcept:begin_(rhs.begin_),end_(rhs.end_),cap_(rhs.cap_) {
        rhs.begin_=nullptr;
        rhs.end_=nullptr;
        rhs.cap_=nullptr;
    }

    vector(std::initializer_list<value_type> ilist) {
        range_init(ilist.begin(),ilist.end());
    }

    vector& operator=(const vector& rhs);
    vector& operator=(vector&& rhs) noexcept;

    vector& operator=(std::initializer_list<value_type> ilist) {
        vector tmp(ilist.begin(),ilist.end());
        swap(tmp);
        return *this;
    }
    ~vector() {
        destroy_and_recover(begin_,end_,cap_-begin_);
        begin_=end_=cap_=nullptr;
    }

public:
    //vector中的迭代器的相关操作
    iterator begin() noexcept {//返回迭代器指针
        return begin_;
    }
    const_iterator begin() const noexcept {//返回常数指针
        return begin_;
    }
    iterator end() noexcept {
        return end_;
    }
    const_iterator end() const noexcept {
        return end_;
    }

    reverse_iterator rbegin() noexcept {
        return reverse_iterator(end());
    }
    const_reverse_iterator rbegin() const noexcept {
        return const_reverse_iterator(end());
    }
    reverse_iterator rend() noexcept {
        return reverse_iterator(begin());
    }
    const_reverse_iterator rend() const noexcept {
        return reverse_iterator(begin());
    }

    const_iterator cbegin() const noexcept {
        return begin();
    }
    const_iterator cend() const noexcept {
        return end();
    }
    const_reverse_iterator crbegin() const noexcept {
        return rbegin();
    }
    const_reverse_iterator crend() const noexcept {
        return rend();
    }

    //容量相关操作
    //判断vector是否为空
    bool empty() const noexcept {
        return begin_==end_;
    }
    //获取vector的长度
    size_type size() const noexcept {
        return static_cast<size_type>(end_-begin_);
    }
    //static_cast<size_type>(-1)可以取到size_type类型所能表示的最大值
    size_type max_size() const noexcept {
        return static_cast<size_type>(-1)/sizeof(T);
    }
    size_type capacity() const noexcept {
        return static_cast<size_type>(cap_-begin_);
    }
    void reverse(size_type n);
    void shrink_to_fit();

    //访问元素相关操作
    reference operator[](size_type n) {
        MYSTL_DEBUG(n<size());
        return *(begin_+n);
    }

    const_reference operator[](size_type n) const {
        MYSTL_DEBUG(n<size());
        return *(begin_+n);
    }
    reference at(size_type n) {
        THROW_OUT_OF_RANGE_IF(!(n<size()),"vector<T>::at() out of range");
        return (*this)[n];
    }
    const_reference at(size_type n) const {
        THROW_OUT_OF_RANGE_IF(!(n<size()),"vector<T>::at() out of range");
        return (*this)[n];
    }
    //取出第一个元素
    reference front() {
        MYSTL_DEBUG(!empty());
        return *begin_;
    }
    const_reference front() const {
        MYSTL_DEBUG(!empty());
        return *begin_;
    }
    //取出最后一个元素
    reference back() {
        MYSTL_DEBUG(!empty());
        return *(end_-1);
    }
    const_reference back() const {
        MYSTL_DEBUG(!empty());
        return *(end_-1);
    }

    pointer data() noexcept {
        return begin_;
    }
    const_pointer data() const noexcept {
        return begin_;
    }

    //修改容器的相关操作
    void assign(size_type n,const value_type& value) {
        fill_assign(n,value);
    }
    template<class Iter,typename std::enable_if<mystl::is_input_iterator<Iter>::value,int>::type=0>
    void assign(Iter first,Iter last) {
        MYSTL_DEBUG(!(last<first));
        copy_assign(first,last,iterator_category(first));
    }
    void assign(std::initializer_list<value_type> il) {
        copy_assign(il.begin(),il.end(),mystl::forward_iterator_tag{});
    }

    //emplace
    template<class... Args>
    void emplace(const_iterator pos,Args&& ...args);

    template<class... Args>
    void emplace_back(Args&& ...args);

    //push_back
    void push_back(const value_type& value);
    void push_back(value_type&& value) {
        emplace_back(mystl::move(value));
    }
    void pop_back();

    //insert
    iterator insert(const_iterator pos,const value_type& type);

    iterator insert(const_iterator pos,value_type&& value) {
        return emplace(pos,mystl::move(value));
    }
    iterator insert(const_iterator pos,size_type n,const value_type& value) {
        MYSTL_DEBUG(pos>=begin()&& pos<=end());
        return fill_insert(const_cast<iterator>(pos),n,value);
    }

    template <class Iter,typename std::enable_if<mystl::is_input_iterator<Iter>::value,int>::type=0>
    void insert(const_iterator pos,Iter first,Iter last) {
        MYSTL_DEBUG(pos>=begin()&&pos<=end()&&!(last<first));
        copy_insert(const_cast<iterator>(pos),first,last);
    }

    //erase
    iterator erase(const_iterator pos);
    iterator erase(const_iterator first,const_iterator last);
    void clear() {
        erase(begin(),end());
    }

    //resize
    void resize(size_type new_size) {
        return resize(new_size,value_type());
    }
    void resize(size_type new_size,const value_type& value);
    void reverse() {
        mystl::revese(begin(),end());//实现在algo.h中
    }
    void swap(vector& rhs) noexcept;
private:
    void try_init() noexcept;
    void init_space(size_type size,size_type cap);
    void fill_init(size_type n,const value_type& value);

    template<class Iter>
    void range_init(Iter first,Iter last);

    void destroy_and_recover(iterator first,iterator last,size_type n);

    //calculate the growth size
    size_type get_new_cap(size_type add_size);

    //assign
    void fill_assign(size_type n,const value_type& value);

    template<class IIter>
    void copy_assign(IIter first,IIter last,input_iterator_tag);

    template<class FIter>
    void copy_assign(FIter first,FIter last,forward_iterator_tag);

    //reallocate
    template<class... Args>
    void reallocate_emplace(iterator pos,Args&& ...args);
    void reallocate_insert(iterator pos,const value_type& value);

    //insert
    iterator fill_insert(iterator pos,size_type n,const value_type& value);
    template<class IIter>
    void copy_insert(iterator pos,IIter first,IIter last);

    void reinsert(size_type size);
};
    //各个函数的具体实现
    template<class T>
    vector<T>& vector<T>::operator=(const vector &rhs) {
        if(this!=&rhs) {
            const auto len=rhs.size();
            if(len>capacity()) {
                //vector1=vector2,vector2的长度大于vector1的长度
                vector tmp(rhs.begin(),rhs.end());
                swap(tmp);
            }else if(size()>=len) {
                //vector1=vector2,vector2的长度小于vector1的长度
                auto i=mystl::copy(rhs.begin(),rhs.end(),begin());
                mystl::uninitialized_copy(rhs.begin()+size(),rhs.end(),end_);
                cap_=end_=begin_+len;
            }else {
                //把rhs.begin()---rhs.end()复制，重新初始化一个rhs.begin()---rhs.end()
                mystl::copy(rhs.begin(),rhs.end()+size(),begin_);
                mystl::uninitialized_copy(rhs.begin()+size(),rhs.end(),end_);
                cap_=end_=begin_+len;
            }
        }
        return *this;
    }

    template<class T>
    vector<T>& vector<T>::operator=(vector&& rhs) noexcept {
        destroy_and_recover(begin_,end_,cap_-begin_);
        begin_=rhs.begin_;
        end_=rhs.end_;
        cap_=rhs.cap_;
        rhs.begin_=nullptr;
        rhs.end_=nullptr;
        rhs.cap_=nullptr;
        return *this;
    }

    //预留空间，当原容量小于要求大小，才会重新分配
    template<class T>
    void vector<T>::reserve(size_type n) {
        if(capacity()<n) {
            THROW_LENGTH_ERROR_IF(n>max_size(),"n can not larger than max_size()");
        }
        //重新开辟一段空间，把原vector分配到里面去
        const auto old_size=size();
        auto tmp=data_allocator::allocate(n);
        mystl::uninitialized_move(begin_,end_,tmp);
        data_allocator::deallocate(begin_,cap_-begin_);
        begin_=tmp;
        end_=tmp+old_size;
        cap_=begin_+n;
    }

    //放弃多余的容量
    template<class T>
    void vector<T>::shrink_to_fit() {
        if(end_<cap_) {
            reinsert(size());
        }
    }

    //在pos位置就地构造元素，防止额外开销
    template<class T>
    template<class... Args>
    void vector<T>::emplace(const_iterator pos, Args &&... args) {
        MYSTL_DEBUG(pos>=begin()&&pos<=end());
        iterator xpos=const_cast<iterator>(pos);
        const size_type n=xpos-begin_;
        if(end_!=cap_&&xpos==end_) {
            data_allocator::construct(mystl::address_of(*end_),mystl::forward<Args>(args)...);
            ++end_;
        }
        else if(end_!=cap_) {
            //在begin_和end_中间进行emplace
            auto new_end=end_;
            //end_再前进，指针赋值为end-1
            data_allocator::construct(mystl::address_of(*end_),*(end_-1));
            ++new_end;
            //把从xpos到end_的值复制（相当于把pos)空出来，其余指针前进一位
            mystl::copy_backward(xpos,end_-1,end_);
            *xpos=value_type(mystl::forward<Args>(args)...);
            end_=new_end;
        }else {
            reallocate_emplace(xpos,mystl::forward<<Args>(args)...);
        }
        return begin()+n;
    }
    //emplace_back,在尾部就地构造
    template<class T>
    template<class... Args>
    void vector<T>::emplace_back(Args &&... args) {
        if(end_< cap_) {
            data_allocator::construct(mystl::address_of(*end_),mystl::forward<Args>(args)...);
            ++end_;
        }else {
            reallocate_emplace(end_,mystl::forward<Args>(args)...);
        }
    }
    //push_back,从尾部添加
    template<class T>
    void vector<T>::push_back(const value_type &value) {
        if(end_!=cap_) {
            data_allocator::construct(mystl::address_of(*end_),value);
            ++end_;
        }else {
            reallocate_insert(end_,value);
        }
    }
    //pop_back 弹出尾部元素
    template<class T>
    void vector<T>::pop_back() {
        MYSTL_DEBUG(!empty());
        data_allocator::destroy(end_-1);
        --end_;
    }
    //在pos处加入元素
    template<class T>
    typename vector<T>::iterator vector<T>::insert(const_iterator pos, const value_type& value) {
        MYSTL_DEBUG(pos>=begin()&&pos<=end());
        iterator xpos=static_cast<iterator>(pos);
        const size_type n=pos-begin_;
        if(end_!=cap_&&end_==xpos) {
            data_allocator::construct(mystl::address_of(*end_),value);
            ++end_;
        }else if(end_!=cap_) {
            //从中间插入
            auto new_end=end_;
            data_allocator::construct(mystl::address_of(*end_),*(end_-1));
            ++new_end;
            auto value_copy=value;
            mystl::copy_backward(xpos,end_-1,end_);
            *pos=mystl::move(value_copy);
            end_=new_end;
        }else {//end_>=cap_,申请存储空间不够时
            reallocate_insert(xpos,value);
        }
        return begin_+n;
    }

    //删除pos上的元素
    template<class T>
    typename vector<T>::iterator vector<T>::erase(const_iterator pos) {
        MYSTL_DEBUG(pos>=begin()&&pos<=end());
        iterator xpos=begin_+(pos-begin());//计算相对位置
        mystl::move(xpos+1,end_,xpos);
        data_allocator::destroy(end_-1);
        --end_;
        return xpos;
    }

    //删除[first,last)上的元素
    template<class T>
    typename vector<T>::iterator vector<T>::erase(const_iterator first, const_iterator last) {
        MYSTL_DEBUG(first>=begin()&&last<=end()&&!(last<first));
        const auto n=first-begin();
        iterator r=begin_+(first-begin());
        data_allocator::destroy(mystl::move(r+(last-first),end_,r),end_);
        end_=end_-(last-first);
        return begin_+n;
    }

    //重置容器大小
    template<class T>
    void vector<T>::resize(size_type new_size, const value_type &value) {
        if(new_size<size()) {//只留下new_size的大小
            erase(begin()+new_size,end());
        }else {
            insert(end(),new_size-size(),value);
        }
    }

    //与另一个vector交换
    template<class T>
    void vector<T>::swap(vector &rhs) noexcept {
        if(this!=&rhs) {
            mystl::swap(begin_,rhs.begin_);
            mystl::swap(end_,rhs.end_);
            mystl::swap(cap_,rhs.cap_);
        }
    }

    //try_init函数,若分配失败则忽略，不抛出异常
    template<class T>
    void vector<T>::try_init() noexcept {
        try {
            begin_=data_allocator::allocate(16);
            end_=begin_;
            cap_=begin_+16;
        }
        catch (...) {
            begin_=nullptr;
            end_=nullptr;
            cap_=nullptr;
        }
    }

    //init_space函数,参数中指定了size和cap的大小
    template<class T>
    void vector<T>::init_space(size_type size, size_type cap) {
        try {
            begin_data_allocator::allocate(cap);
            end_=begin_+size;
            cap_=begin_+cap;
        }catch (...) {
            begin_=nullptr;
            end_=nullptr;
            cap_=nullptr;
            throw;
        }
    }

    //fill_init函数
    template<class T>
    void vector<T>::fill_init(size_type n, const value_type &value) {
        const size_t init_size=mystl::max(static_cast<size_type>(16),n);
        init_space(n,init_size);
        mystl::uninitialized_fill_n(begin_,n,value);//从begin_开始往后面n个位置填充value
    }

    //range_init函数
    template<class T>
    template<class Iter>
    void vector<T>::range_init(Iter first, Iter last) {
        const size_type len=mystl::distance(first,last);//计算指针距离
        const size_type init_size=mystl::max(len,static_cast<size_type>(16));
        init_space(len,init_size);
        mystl::uninitialized_copy(first,last,begin_);
    }

    //destroy_and_recover
    template<class T>
    void vector<T>::destroy_and_recover(iterator first, iterator last, size_type n) {
        data_allocator::destroy(first,last);//调用析构函数
        data_allocator::deallocate(first,n);//释放空间
    }

    //get_new_cap
    template<class T>
    typename vector<T>::size_type vector<T>::get_new_cap(size_type add_size) {
        const auto old_size=capacity();
        THROW_LENGTH_ERROR_IF(old_size>max_size()-add_size,"vector size is too big");
        if(old_size>max_size()-old_size/2) {
            return old_size+add_size>max_size()-16?old_size+add_size:old_size+add_size+16;
        }
        //当cap为0，初始时，设置长度为16；当有内容时，扩容1.5
        const size_type new_size=old_size==0?mystl::max(add_size,static_cast<size_type>(16)):mystl::max(old_size+old_size/2,old_size+add_size);
        return new_size;
    }

    //fill_assign
    template<class T>
    void vector<T>::fill_assign(size_type n, const value_type &value) {
        if(n>capacity()) {//重新初始化一个vector,并和当前vector进行swap
            vector tmp(n,value);
            swap(tmp);
        }
        else if(n>size()) {
            mystl::fill(begin(),end(),value);
            end_=mystl::uninitialized_fill_n(end_,n-size(),value);
        }else {
            //把begin_后的n个赋值为n，然后把[begin_+n,end_]erase
            erase(mystl::fill_n(begin_,n,value),end_);
        }
    }

    //copy_assign
    template<class T>
    template<class IIter>
    void vector<T>::copy_assign(IIter first, IIter last, input_iterator_tag) {
        auto cur=begin_;
        for(;first!=last&&cur!=end_;++first,++cur) {
            *cur=*first;
        }
        if(first==last) {
            erase(cur,end_);//把cur后面那部分去掉
        }else {//原vector不够长，在end_后面新增一段
            insert(end_,first,last);
        }
    }

    //copy_assign2
    template<class T>
    template<class FIter>
    void vector<T>::copy_assign(FIter first, FIter last, forward_iterator_tag) {
        const size_type len=mystl::distance(first,last);
        if(len>capacity()) {
            vector tmp(first,last);
            swap(tmp);
        }else if(size()>=len) {
            auto new_end=mystl::copy(first,last,begin_);
            data_allocator::destroy(new_end,end_);
            end_=new_end;
        }else {
            //first=begin_ ; last=end_
            auto mid=first;
            mystl::advance(mid,size());//让mid前进size距离，也就是前进到没有内容的位置
            mystl::copy(first,last,begin_);
            auto new_end=mystl::uninitialized_copy(mid,last,end_);
            end_=new_end;
        }
    }

    //重新分配空间并在pos处构造元素
    template<class T>
    template<class... Args>
    void vector<T>::reallocate_emplace(iterator pos, Args &&... args) {
            const auto new_size=get_new_cap(1);
            auto new_begin=data_allocator::allocate(new_size);
            auto new_end=new_begin;
        try {
            new_end=mystl::uninitialized_move(begin_,pos,new_begin);
            data_allocator::construct(mystl::address_of(*new_end),mystl::forward<Args>(args)...);
            ++new_end;
            new_end=mystl::uninitialized_move(pos,end_,new_end);
        }catch (...) {
            data_allocator::deallocate(new_begin,new_size);
            throw;
        }
        destroy_and_recover(begin_,end_,cap_-begin_);
        begin_=new_begin;
        end_=new_end;
        cap_=new_begin+new_size;
    }

    template<class T>
    void vector<T>::reallocate_insert(iterator pos, const value_type &value) {
        const auto new_size=get_new_cap(1);
        auto new_begin=data_allocator::allocate(new_size);
        auto new_end=new_begin;
        const value_type& value_copy=value;
        try {
            new_end=mystl::uninitialized_move(begin_,pos,new_begin);
            data_allocator::construct(mystl::address_of(*new_end),value_copy);
            ++new_end;
            new_end=mystl::uninitialized_move(pos,end_,new_end);
        }catch (...) {
            data_allocator::deallocate(new_begin,new_size);
            throw;
        }
        destroy_and_recover(begin_,end_,cap_-begin_);
        begin_=new_begin;
        end_=new_end;
        cap_=new_begin+new_size;
    }

    template<class T>
    typename vector<T>::iterator vector<T>::fill_insert(iterator pos, size_type n, const value_type &value) {
            if(n==0) return pos;
            const size_type xpos=pos-begin_;
            const value_type value_copy=value;
            if(static_cast<size_type>(cap_-end_)>=n) {//vector的容量可以承受n个单位
                const size_type after_elems=end_-pos;
                auto old_end=end_;
                if(after_elems>n) {//[pos,end_]的空间大于n
                    mystl::uninitialized_copy(end_-n,end_,end_);//[end_-n,end)后移n位
                    end_+=n;
                    mystl::move_backward(pos,old_end-n,old_end);
                    mystl::uninitialized_fill_n(pos,n,value);
                }else {
                    end_=mystl::uninitialized_fill_n(end_,n-after_elems,value_copy);
                    end_=mystl::uninitialized_move(pos,old_end,end_);
                    mystl::uninitialized_fill_n(pos,after_elems,value_copy);
                }
            }else {//备用空间不够
                const auto new_size=get_new_cap(n);
                auto new_begin=data_allocator::allocate(new_size);
                auto new_end=new_begin;
                try {
                    new_end=mystl::uninitialized_move(begin_,pos,new_begin);
                    new_end=uninitialized_fill_n(new_end,n,value);
                    new_end=mystl::uninitialized_move(pos,end_,new_end);
                }catch (...) {
                    destroy_and_recover(new_begin,new_end,new_size);
                    throw;
                }
                data_allocator::deallocate(begin_,cap_-begin_);
                begin_=new_begin;
                end_=new_end;
                cap_=begin_+new_size;
            }
        return begin_+xpos;
    }

    template<class T>
    template<class IIter>
    void vector<T>::copy_insert(iterator pos, IIter first, IIter last) {
        if(first==last) return;
        const auto n=mystl::distance(first,last);
        if((cap_-end_)>=n) {
            const auto after_elems=end_-pos;
            auto old_end=end_;
            if(after_elems>n) {
                end_=mystl::uninitialized_copy(end_-n,end_,end_);
                mystl::move_backward(pos,old_end-n,old_end);
                mystl::uninitialized_copy(first, last, pos);
            }
            else
            {
                auto mid = first;
                mystl::advance(mid, after_elems);
                end_ = mystl::uninitialized_copy(mid, last, end_);
                end_ = mystl::uninitialized_move(pos, old_end, end_);
                mystl::uninitialized_copy(first, mid, pos);
            }
        }
        else
        { // 备用空间不足
            const auto new_size = get_new_cap(n);
            auto new_begin = data_allocator::allocate(new_size);
            auto new_end = new_begin;
            try
            {
                new_end = mystl::uninitialized_move(begin_, pos, new_begin);
                new_end = mystl::uninitialized_copy(first, last, new_end);
                new_end = mystl::uninitialized_move(pos, end_, new_end);
            }
            catch (...)
            {
                destroy_and_recover(new_begin, new_end, new_size);
                throw;
            }
            data_allocator::deallocate(begin_, cap_ - begin_);
            begin_ = new_begin;
            end_ = new_end;
            cap_ = begin_ + new_size;
        }
    }

    template<class T>
    void vector<T>::reinsert(size_type size) {
        auto new_begin=data_allocator::allocate(size);
        try {
            mystl::uninitialized_move(begin_,end_,new_begin);
        }catch (...) {
            data_allocator::deallocate(new_begin,size);
            throw;
        }
        data_allocator::deallocate(begin_,cap_-begin_);
        begin_=new_begin;
        end_=begin_+size;
        cap_=begin_+size;
    }

    //重载操作符
    template<class T>
    bool operator==(const vector<T>& lhs,vector<T>& rhs) {
        return lhs.size()==rhs.size()&&mystl::equal(lhs.begin(),lhs.end(),rhs.begin());
    }

    template<class T>
    bool operator<(const vector<T>& lhs,const vector<T>& rhs) {
        return mystl::lexicographical_compare(lhs.begin(),lhs.end(),rhs.begin(),rhs.end());
    }

    template<class T>
    bool operator!=(const vector<T>& lhs,vector<T>& rhs) {
        return !(lhs==rhs);
    }

    template<class T>
    bool operator>(const vector<T>& lhs,vector<T>& rhs) {
        return rhs<lhs;
    }

    template <class T>
    bool operator<=(const vector<T>& lhs, const vector<T>& rhs)
    {
        return !(rhs < lhs);
    }

    template <class T>
    bool operator>=(const vector<T>& lhs, const vector<T>& rhs)
    {
        return !(lhs < rhs);
    }

    template<class T>
    void swap(vector<T>& lhs,vector<T>& rhs) {
        lhs.swap(rhs);
    }

}

#endif