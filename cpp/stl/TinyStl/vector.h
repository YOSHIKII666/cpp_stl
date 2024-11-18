#ifndef MYTINYSTL_VECTOR_H_
#define MYTINYSTL_VECTOR_H_

// ���ͷ�ļ�����һ��ģ���� vector
// vector : ����

// notes:
//
// �쳣��֤��
// mystl::vecotr<T> ��������쳣��֤�����ֺ������쳣��֤���������º�����ǿ�쳣��ȫ��֤��
//   * emplace
//   * emplace_back
//   * push_back
// �� std::is_nothrow_move_assignable<T>::value == true ʱ�����º���Ҳ����ǿ�쳣��֤��
//   * reserve
//   * resize
//   * insert

#include <initializer_list>

#include "iterator.h"
#include "memory.h"
#include "util.h"
#include "exceptdef.h"

namespace mystl{
//#ifdef ����������Ԥ����ָ���һ�֡������ڼ��ĳ����ʶ����ͨ���Ǻ����ƣ��Ƿ��Ѿ���������ˡ�
//����ñ�ʶ���Ѿ������壬��ô #ifdef ����֮��Ӧ�� #endif ֮��Ĵ���齫�ᱻ���봦��
//��֮������ñ�ʶ��δ�����壬��δ������ᱻ���������ԣ�������뵽ʵ�ʵı�������С�
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
    //try_init(),���ڷ����ʼ�ռ䣬Ĭ�ϴ�СΪ16
    //init_space(size,cap),Ĭ�Ϸ���cap��cap_,size��size_
    //fill_init(n,value),����n��16�����ֵ��ʼ��һƬ���򣬲��Ҹ�ǰn��ֵ��ֵΪvalue
    //range_init(first,last),����last-first��16�����ֵ��ʼ��һƬ�����Ҹ�first��last��ֵ��ֵΪ��first��ͬ
    //destroyed_and_recover(first,last,n),���������������ͷ��ڴ档
    //get_new_cap(add_size),���1.5*old_size>max_size()��ǰ���£�old_size+add_size>max-16��ô����old+add,��֮����old+add+16;���old_size=0,����16��add�����ֵ����֮����1.5old_size��old+add�����ֵ
    //fill_assign(n,value)�����n>capacity()����ô���³�ʼ��һƬ����n����ֵΪvalue;��n>size()��begin��end��ֵΪvalue,end_�Լ�����ĸ�ֵΪvalue�����n<capacity(),����value���n��λ�ú������end_�����ڵ����ݡ�
    //copy_assign(first,last,input_iterator_tag),��cur��ֵΪfirst,����ͬʱǰ����ֱ��first=last,Ȼ��erase���������
    //copy_assign(first,last,forward_iterator_tag),����first��last֮��ľ���len�����len>capacity(),��������n��value��swap;���size()>=len,��ԭvector��ǰlen��������first��ֵ��Ȼ��ݻٺ�������ݣ����len<capacity,���Ʋ���
    //reallocate_emplace(pos,args),���·���һƬ���򣬲�����posλ���ϲ���args
    //reallocate_insert(pos,value),���·���һƬ���򣬲���pos������value
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
    iterator begin_;//��ʾĿǰʹ�ÿռ��ͷ��
    iterator end_;//��ʾĿǰʹ�ÿռ��β��
    iterator cap_;//��ʾĿǰ�洢��β��

public:
    vector() noexcept {
        try_init();
    }
    explicit vector(size_type n)//explicit��ֹ���캯��������ʽת��
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
    //vector�еĵ���������ز���
    iterator begin() noexcept {//���ص�����ָ��
        return begin_;
    }
    const_iterator begin() const noexcept {//���س���ָ��
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

    //������ز���
    //�ж�vector�Ƿ�Ϊ��
    bool empty() const noexcept {
        return begin_==end_;
    }
    //��ȡvector�ĳ���
    size_type size() const noexcept {
        return static_cast<size_type>(end_-begin_);
    }
    //static_cast<size_type>(-1)����ȡ��size_type�������ܱ�ʾ�����ֵ
    size_type max_size() const noexcept {
        return static_cast<size_type>(-1)/sizeof(T);
    }
    size_type capacity() const noexcept {
        return static_cast<size_type>(cap_-begin_);
    }
    void reverse(size_type n);
    void shrink_to_fit();

    //����Ԫ����ز���
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
    //ȡ����һ��Ԫ��
    reference front() {
        MYSTL_DEBUG(!empty());
        return *begin_;
    }
    const_reference front() const {
        MYSTL_DEBUG(!empty());
        return *begin_;
    }
    //ȡ�����һ��Ԫ��
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

    //�޸���������ز���
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
        mystl::revese(begin(),end());//ʵ����algo.h��
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
    //���������ľ���ʵ��
    template<class T>
    vector<T>& vector<T>::operator=(const vector &rhs) {
        if(this!=&rhs) {
            const auto len=rhs.size();
            if(len>capacity()) {
                //vector1=vector2,vector2�ĳ��ȴ���vector1�ĳ���
                vector tmp(rhs.begin(),rhs.end());
                swap(tmp);
            }else if(size()>=len) {
                //vector1=vector2,vector2�ĳ���С��vector1�ĳ���
                auto i=mystl::copy(rhs.begin(),rhs.end(),begin());
                mystl::uninitialized_copy(rhs.begin()+size(),rhs.end(),end_);
                cap_=end_=begin_+len;
            }else {
                //��rhs.begin()---rhs.end()���ƣ����³�ʼ��һ��rhs.begin()---rhs.end()
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

    //Ԥ���ռ䣬��ԭ����С��Ҫ���С���Ż����·���
    template<class T>
    void vector<T>::reserve(size_type n) {
        if(capacity()<n) {
            THROW_LENGTH_ERROR_IF(n>max_size(),"n can not larger than max_size()");
        }
        //���¿���һ�οռ䣬��ԭvector���䵽����ȥ
        const auto old_size=size();
        auto tmp=data_allocator::allocate(n);
        mystl::uninitialized_move(begin_,end_,tmp);
        data_allocator::deallocate(begin_,cap_-begin_);
        begin_=tmp;
        end_=tmp+old_size;
        cap_=begin_+n;
    }

    //�������������
    template<class T>
    void vector<T>::shrink_to_fit() {
        if(end_<cap_) {
            reinsert(size());
        }
    }

    //��posλ�þ͵ع���Ԫ�أ���ֹ���⿪��
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
            //��begin_��end_�м����emplace
            auto new_end=end_;
            //end_��ǰ����ָ�븳ֵΪend-1
            data_allocator::construct(mystl::address_of(*end_),*(end_-1));
            ++new_end;
            //�Ѵ�xpos��end_��ֵ���ƣ��൱�ڰ�pos)�ճ���������ָ��ǰ��һλ
            mystl::copy_backward(xpos,end_-1,end_);
            *xpos=value_type(mystl::forward<Args>(args)...);
            end_=new_end;
        }else {
            reallocate_emplace(xpos,mystl::forward<<Args>(args)...);
        }
        return begin()+n;
    }
    //emplace_back,��β���͵ع���
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
    //push_back,��β�����
    template<class T>
    void vector<T>::push_back(const value_type &value) {
        if(end_!=cap_) {
            data_allocator::construct(mystl::address_of(*end_),value);
            ++end_;
        }else {
            reallocate_insert(end_,value);
        }
    }
    //pop_back ����β��Ԫ��
    template<class T>
    void vector<T>::pop_back() {
        MYSTL_DEBUG(!empty());
        data_allocator::destroy(end_-1);
        --end_;
    }
    //��pos������Ԫ��
    template<class T>
    typename vector<T>::iterator vector<T>::insert(const_iterator pos, const value_type& value) {
        MYSTL_DEBUG(pos>=begin()&&pos<=end());
        iterator xpos=static_cast<iterator>(pos);
        const size_type n=pos-begin_;
        if(end_!=cap_&&end_==xpos) {
            data_allocator::construct(mystl::address_of(*end_),value);
            ++end_;
        }else if(end_!=cap_) {
            //���м����
            auto new_end=end_;
            data_allocator::construct(mystl::address_of(*end_),*(end_-1));
            ++new_end;
            auto value_copy=value;
            mystl::copy_backward(xpos,end_-1,end_);
            *pos=mystl::move(value_copy);
            end_=new_end;
        }else {//end_>=cap_,����洢�ռ䲻��ʱ
            reallocate_insert(xpos,value);
        }
        return begin_+n;
    }

    //ɾ��pos�ϵ�Ԫ��
    template<class T>
    typename vector<T>::iterator vector<T>::erase(const_iterator pos) {
        MYSTL_DEBUG(pos>=begin()&&pos<=end());
        iterator xpos=begin_+(pos-begin());//�������λ��
        mystl::move(xpos+1,end_,xpos);
        data_allocator::destroy(end_-1);
        --end_;
        return xpos;
    }

    //ɾ��[first,last)�ϵ�Ԫ��
    template<class T>
    typename vector<T>::iterator vector<T>::erase(const_iterator first, const_iterator last) {
        MYSTL_DEBUG(first>=begin()&&last<=end()&&!(last<first));
        const auto n=first-begin();
        iterator r=begin_+(first-begin());
        data_allocator::destroy(mystl::move(r+(last-first),end_,r),end_);
        end_=end_-(last-first);
        return begin_+n;
    }

    //����������С
    template<class T>
    void vector<T>::resize(size_type new_size, const value_type &value) {
        if(new_size<size()) {//ֻ����new_size�Ĵ�С
            erase(begin()+new_size,end());
        }else {
            insert(end(),new_size-size(),value);
        }
    }

    //����һ��vector����
    template<class T>
    void vector<T>::swap(vector &rhs) noexcept {
        if(this!=&rhs) {
            mystl::swap(begin_,rhs.begin_);
            mystl::swap(end_,rhs.end_);
            mystl::swap(cap_,rhs.cap_);
        }
    }

    //try_init����,������ʧ������ԣ����׳��쳣
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

    //init_space����,������ָ����size��cap�Ĵ�С
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

    //fill_init����
    template<class T>
    void vector<T>::fill_init(size_type n, const value_type &value) {
        const size_t init_size=mystl::max(static_cast<size_type>(16),n);
        init_space(n,init_size);
        mystl::uninitialized_fill_n(begin_,n,value);//��begin_��ʼ������n��λ�����value
    }

    //range_init����
    template<class T>
    template<class Iter>
    void vector<T>::range_init(Iter first, Iter last) {
        const size_type len=mystl::distance(first,last);//����ָ�����
        const size_type init_size=mystl::max(len,static_cast<size_type>(16));
        init_space(len,init_size);
        mystl::uninitialized_copy(first,last,begin_);
    }

    //destroy_and_recover
    template<class T>
    void vector<T>::destroy_and_recover(iterator first, iterator last, size_type n) {
        data_allocator::destroy(first,last);//������������
        data_allocator::deallocate(first,n);//�ͷſռ�
    }

    //get_new_cap
    template<class T>
    typename vector<T>::size_type vector<T>::get_new_cap(size_type add_size) {
        const auto old_size=capacity();
        THROW_LENGTH_ERROR_IF(old_size>max_size()-add_size,"vector size is too big");
        if(old_size>max_size()-old_size/2) {
            return old_size+add_size>max_size()-16?old_size+add_size:old_size+add_size+16;
        }
        //��capΪ0����ʼʱ�����ó���Ϊ16����������ʱ������1.5
        const size_type new_size=old_size==0?mystl::max(add_size,static_cast<size_type>(16)):mystl::max(old_size+old_size/2,old_size+add_size);
        return new_size;
    }

    //fill_assign
    template<class T>
    void vector<T>::fill_assign(size_type n, const value_type &value) {
        if(n>capacity()) {//���³�ʼ��һ��vector,���͵�ǰvector����swap
            vector tmp(n,value);
            swap(tmp);
        }
        else if(n>size()) {
            mystl::fill(begin(),end(),value);
            end_=mystl::uninitialized_fill_n(end_,n-size(),value);
        }else {
            //��begin_���n����ֵΪn��Ȼ���[begin_+n,end_]erase
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
            erase(cur,end_);//��cur�����ǲ���ȥ��
        }else {//ԭvector����������end_��������һ��
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
            mystl::advance(mid,size());//��midǰ��size���룬Ҳ����ǰ����û�����ݵ�λ��
            mystl::copy(first,last,begin_);
            auto new_end=mystl::uninitialized_copy(mid,last,end_);
            end_=new_end;
        }
    }

    //���·���ռ䲢��pos������Ԫ��
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
            if(static_cast<size_type>(cap_-end_)>=n) {//vector���������Գ���n����λ
                const size_type after_elems=end_-pos;
                auto old_end=end_;
                if(after_elems>n) {//[pos,end_]�Ŀռ����n
                    mystl::uninitialized_copy(end_-n,end_,end_);//[end_-n,end)����nλ
                    end_+=n;
                    mystl::move_backward(pos,old_end-n,old_end);
                    mystl::uninitialized_fill_n(pos,n,value);
                }else {
                    end_=mystl::uninitialized_fill_n(end_,n-after_elems,value_copy);
                    end_=mystl::uninitialized_move(pos,old_end,end_);
                    mystl::uninitialized_fill_n(pos,after_elems,value_copy);
                }
            }else {//���ÿռ䲻��
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
        { // ���ÿռ䲻��
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

    //���ز�����
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