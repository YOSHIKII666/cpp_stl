//
// Created by 王少泽 on 2024/12/1.
//

#ifndef MYTINYSTL_BASIC_STRING_H_
#define MYTINYSTL_BASIC_STRING_H_

// 这个头文件包含一个模板类 basic_string
// 用于表示字符串类型

#include <iostream>

#include "iterator.h"
#include "memory.h"
#include "functional.h"
#include "exceptdef.h"

namespace mystl
{
    template <class CharType>
    struct char_traits {
        //CharType可以是不同的类型
        //char_type(0)用于表示字符串的结尾
        typedef CharType char_type;

        static size_t length(const char_type* str) {
            size_t len=0;
            for(; *str!=char_type(0);++str) {
                ++len;
            }
            return len;
        }

        static int compare(const char_type* s1,const char_type* s2,size_t n) {
            for(;n!=0;--n,++s1,++s2) {
                if(*s1<*s2) {
                    return -1;
                }
                if(*s1>*s2) {
                    return 1;
                }
            }
            return 0;
        }

        static char_type* copy(char_type* dst,const char_type* src,size_t n) {
            MYSTL_DEBUG(src+n<=dst||dst+n<=src);
            char_type* r=dst;
            for(;n!=0;--n,++dst,++src) {
                *dst=*src;
            }
            return r;
        }
        static char_type* move(char_type* dst,const char_type* src,size_t n) {
            char_type* r=dst;
            if(dst<src) {
                for(;n!=0;--n,++dst,++src) {
                    *dst=*src;
                }
            }else if(src<dst) {//避免数据覆盖
                dst+=n;
                src+=n;
                for(;n!=0;--n) {
                    *--dst=*--src;
                }
                return r;
            }
        }

        static char_type* fill(char_type* dst,char_type ch,size_t count) {
            char_type* r=dst;
            for(;count>0;--count,++dst) {
                *dst=ch;
            }
            return r;
        }
    };

    //CharType是char时
    template<>
    struct char_traits<char> {
        typedef char char_type;

        static size_t length(const char_type* str) noexcept {
            return std::strlen(str);
        }

        static int compare(const char_type* s1,const char_type* s2,size_t n) noexcept {
            return std::memcmp(s1,s2,n);
        }

        static char_type* copy(char_type* dst,const char_type* src,size_t n) noexcept {
            MYSTL_DEBUG(src+n<=dst||dst+n<=src);//不会出现重叠
            return static_cast<char_type*>(std::memcmp(dst,src,n));
        }

        static char_type* move(char_type* dst, const char_type* src, size_t n) noexcept
        {
            return static_cast<char_type*>(std::memmove(dst, src, n));
        }

        static char_type* fill(char_type* dst, char_type ch, size_t count) noexcept
        {
            return static_cast<char_type*>(std::memset(dst, ch, count));
        }
    };

    // Partialized. char_traits<wchar_t>
    template <>
    struct char_traits<wchar_t>
    {
        typedef wchar_t char_type;

        static size_t length(const char_type* str) noexcept
        {
            return std::wcslen(str);
        }

        static int compare(const char_type* s1, const char_type* s2, size_t n) noexcept
        {
            return std::wmemcmp(s1, s2, n);
        }

        static char_type* copy(char_type* dst, const char_type* src, size_t n) noexcept
        {
            MYSTL_DEBUG(src + n <= dst || dst + n <= src);
            return static_cast<char_type*>(std::wmemcpy(dst, src, n));
        }

        static char_type* move(char_type* dst, const char_type* src, size_t n) noexcept
        {
            return static_cast<char_type*>(std::wmemmove(dst, src, n));
        }

        static char_type* fill(char_type* dst, char_type ch, size_t count) noexcept
        {
            return static_cast<char_type*>(std::wmemset(dst, ch, count));
        }
    };

    template<>
    struct char_traits<char16_t> {
        typedef char16_t char_type;

        static size_t length(const char_type* str) noexcept {
            size_t len=0;
            for(;*str!=char_type(0);++str) {
                len++;
            }
            return len;
        }

        static int compare(const char_type* s1,const char_type* s2,size_t n) noexcept {
            for(;n!=0;--n,++s1,++s2) {
                if(*s1<*s2) {
                    return -1;
                }
                if(*s2<*s1) {
                    return 1;
                }
            }
            return 0;
        }

  static char_type* copy(char_type* dst, const char_type* src, size_t n) noexcept
  {
    MYSTL_DEBUG(src + n <= dst || dst + n <= src);
    char_type* r = dst;
    for (; n != 0; --n, ++dst, ++src)
      *dst = *src;
    return r;
  }

  static char_type* move(char_type* dst, const char_type* src, size_t n) noexcept
  {
    char_type* r = dst;
    if (dst < src)
    {
      for (; n != 0; --n, ++dst, ++src)
        *dst = *src;
    }
    else if (src < dst)
    {
      dst += n;
      src += n;
      for (; n != 0; --n)
        *--dst = *--src;
    }
    return r;
  }

  static char_type* fill(char_type* dst, char_type ch, size_t count) noexcept
  {
    char_type* r = dst;
    for (; count > 0; --count, ++dst)
      *dst = ch;
    return r;
  }
};

// Partialized. char_traits<char32_t>
template <>
struct char_traits<char32_t>
{
  typedef char32_t char_type;

  static size_t length(const char_type* str) noexcept
  {
    size_t len = 0;
    for (; *str != char_type(0); ++str)
      ++len;
    return len;
  }

  static int compare(const char_type* s1, const char_type* s2, size_t n) noexcept
  {
    for (; n != 0; --n, ++s1, ++s2)
    {
      if (*s1 < *s2)
        return -1;
      if (*s2 < *s1)
        return 1;
    }
    return 0;
  }

  static char_type* copy(char_type* dst, const char_type* src, size_t n) noexcept
  {
    MYSTL_DEBUG(src + n <= dst || dst + n <= src);//防止有重叠
    char_type* r = dst;
    for (; n != 0; --n, ++dst, ++src)
      *dst = *src;
    return r;
  }

  static char_type* move(char_type* dst, const char_type* src, size_t n) noexcept
  {
    char_type* r = dst;
    if (dst < src)//防止有重叠，分了两种情况
    {
      for (; n != 0; --n, ++dst, ++src)
        *dst = *src;
    }
    else if (src < dst)//防止有重叠，倒着来
    {
      dst += n;
      src += n;
      for (; n != 0; --n)
        *--dst = *--src;
    }
    return r;
  }

  static char_type* fill(char_type* dst, char_type ch, size_t count) noexcept
  {
    char_type* r = dst;
    for (; count > 0; --count, ++dst)
      *dst = ch;
    return r;
  }
};

//basic_string尝试分配的最小的buffer的大小
#define STRING_INIT_SIZE 32
    //basic_string
    template<class CharType,class CharTraits=mystl::char_traits<CharType>>
    class basic_string {
        public:
          typedef CharTraits traits_type;
          typedef CharTraits char_traits;

          typedef mystl::allocator<CharType> allocator_type;
          typedef mystl::allocator<CharType> data_allocator;

          typedef typename allocator_type::value value_type;
          typedef typename allocator_type::pointer pointer;
          typedef typename allocator_type::const_pointer const_pointer;
          typedef typename allocator_type::reference reference;
          typedef typename allocator_type::const_reference const_reference;
          typedef typename allocator_type::size_type size_type; //size_t
          typedef typename allocator_type::difference_type difference_type;

          typedef value_type* iterator;
          typedef const value_type* const_iterator;
          typedef mystl::reverse_iterator<iterator> reverse_iterator;
          typedef mystl::reverse_iterator<const_iterator> const_reverse_iterator;

          allocator_type get_allocator(){return allocator_type();}
          static_assert(std::is_pod<CharType>::value,"Character type of basic_string must be pod");//plain old data
          static_assert(std::is_same<CharType,typename traits_type::char_type>::value,"charType should be the same"); //basic_string的内部属性charType要和内部属性traits的属性charType一致

          public:
            static constexpr size_type npos=static_cast<size_type>(-1); //size_type的最大值

          private:
            iterator buffer_;//指向缓冲区的指针
            size_type size_;
            size_type cap_;
          
          public:
            basic_string() noexcept{try_init();}

            basic_string(size_type n,value_type ch):buffer_(nullptr),size_(0),cap_(0){init_from(n,ch);}

            basic_string(const basic_string& other,size_type pos):buffer_(nullptr),size_(0),cap_(0){
                init_from(other.buffer_,pos,other.pos-pos);
            }

            basic_string(const basic_string& other,size_type pos,size_type count):buffer_(nullptr),size_(0),cap_(0){
                init_from(other.buffer_,pos,count);
            }

            basic_string(const_pointer str):buffer_(nullptr),size_(0),cap_(0){
              init_from(str,0,char_traits::length(str));
            }
            
            basic_string(const_pointer str,size_type count):buffer_(nullptr),size_(0),cap_(0){
              init_from(str,0,count);
            }

            template<class Iter,typename std::enable_if<mystl::is_input_iterator<Iter>::value,int>::type=0>
            basic_string(Iter first,Iter last){
                copy_init(first,last,iterator_category(first));
            }

            basic_string(const basice_string& rhs):buffer_(nullptr),size_(0),cap_(0){
                init_from(rhs.buffer_,0,rhs.size_);
            }

            basic_string(basic_string&& rhs) noexcept:buffer_(rhs.buffer_),size_(rhs.size_),cap_(rhs.cap_){
                  rhs.buffer_=nullptr;
                  rhs.size_=0;
                  rhs.cap_=0;
            }

            basic_string& operator=(const basic_string& rhs);
            basic_string& operator=(basic_string&& rhs) noexcept;
            basic_string& operator=(const_pointer str);
            basic_string& operator=(value_type ch);

            ~basic_string() {destroy_buffer();}

        public:
            iterator begin() noexcept{return buffer_;}

            const_iterator begin() const noexcept{return buffer_;}

            iterator end() noexcept{return buffer_+size_;}

            const_iterator end() const noexcept{return buffer_+size_;}

            reverse_iterator rbegin() noexcept{return reverse_iterator(end());}

            const_reverse_iterator rbegin() const noexcept{return reverse_iterator(end());}

            reverse_iterator rend() noexcept {return reverse_iterator(begin());}

            const_reverse_iterator rend() const noexcept{return const_reverse_iterator(begin());}

            const_iterator cbegin() const noexcept{return begin();}
            const_iterator cend() const noexcept{return end();}
            const_reverse_iterator crbegin() const noexcept{return rbegin();}
            const_reverse_iterator crend() const noexcept{return rend();}

            bool empty() const noexcept{return size_==0;}

            size_type size() const noexcept{return size_;}

            size_type length() const noexcept{return size_;}

            size_type capacity() const noexcept{return cap_;}

            size_type max_size() const noexcept{static_cast<size_type>(-1);}

            void reserve(size_type n);
            void shrink_to_fit();

            reference operator[](size_type n){
              MYSTL_DEBUG(n<=size_);
              if(n==size_){
                *(buffer_+n)=value_type();
              }
              return *(buffer_+n);
            }

            reference operator[](size_type n) const{
              MYSTL_DEBUG(n<=size_);
              if(n==size_){
                *(buffer_+n)=value_type();
              }
              return *(buffer_+n);
            }

            reference at(size_type n){
                THROW_OUT_OF_RANGE_IF(n>=size_,"out of range");
                return (*this)[n];
            }

             reference at(size_type n) const{
                THROW_OUT_OF_RANGE_IF(n>=size_,"out of range");
                return (*this)[n];
            }

            reference front(){
                MYSTL_DEBUG(!empty());
                return *begin();
            }

             reference front() const{
                MYSTL_DEBUG(!empty());
                return *begin();
            }

            reference back(){
              MYSTL_DEBUG(!empty());
              return *(end()-1);
            }

            reference back() const{
              MYSTL_DEBUG(!empty());
              return *(end()-1);
            }

            const_pointer data() const noexcept{
              return to_raw_pointer();
            }
            const_pointer c_str() const noexcept{
              return to_raw_pointer();
            }

            iterator insert(const_iterator pos,value_type ch);
            iterator insert(const_iterator pos,size_type count,value_type ch);

            template<class Iter>
            iterator insert(const_iterator pos,Iter first,Iter last);

            void push_back(value_type ch){append(1,ch);}

            void pop_back(){
              MYSTL_DEBUG(!empty());
              --size_;
            }

            basic_string& append(size_type count,value_type ch);

            basic_string& append(const basic_string& str){
                return append(str,0,str.size_);
            }
            basic_string& append(const basic_string& str,size_type pos){
              return append(str,pos,str.size_-pos);
            }
            basic_string& append(const basic_string&,size_type pos,size_type count);

            basic_string& append(const_pointer s){return append(s,char_traits::length(s));}
            basic_string& append(const_pointer s,size_type count);

            template<class Iter,typename std::enable_if<mystl::is_input_iterator<Iter>::value,int>::type=0>
            basic_string& append(Iter first,Iter last){return append_range(first,last);}

            iterator erase(const_iterator pos);
            iterator erase(const_iterator first,const_iterator last);

            void resize(size_type count){resize(count,value_type());}
            void resize(size_type count ,value_type ch);

            void clear() noexcept{size_=0;}

             // compare
            int compare(const basic_string& other) const;
            int compare(size_type pos1, size_type count1, const basic_string& other) const;
            int compare(size_type pos1, size_type count1, const basic_string& other,
                        size_type pos2, size_type count2 = npos) const;
            int compare(const_pointer s) const;
            int compare(size_type pos1, size_type count1, const_pointer s) const;
            int compare(size_type pos1, size_type count1, const_pointer s, size_type count2) const;

            // substr
            basic_string substr(size_type index, size_type count = npos)
            {
              count = mystl::min(count, size_ - index);
              return basic_string(buffer_ + index, buffer_ + index + count);
            }

            // replace
            basic_string& replace(size_type pos, size_type count, const basic_string& str)
            {
              THROW_OUT_OF_RANGE_IF(pos > size_, "basic_string<Char, Traits>::replace's pos out of range");
              return replace_cstr(buffer_ + pos, count, str.buffer_, str.size_);
            }
            basic_string& replace(const_iterator first, const_iterator last, const basic_string& str)
            {
              MYSTL_DEBUG(begin() <= first && last <= end() && first <= last);
              return replace_cstr(first, static_cast<size_type>(last - first), str.buffer_, str.size_);
            }

            basic_string& replace(size_type pos, size_type count, const_pointer str)
            {
              THROW_OUT_OF_RANGE_IF(pos > size_, "basic_string<Char, Traits>::replace's pos out of range");
              return replace_cstr(buffer_ + pos, count, str, char_traits::length(str));
            }
            basic_string& replace(const_iterator first, const_iterator last, const_pointer str)
            {
              MYSTL_DEBUG(begin() <= first && last <= end() && first <= last);
              return replace_cstr(first, static_cast<size_type>(last - first), str, char_traits::length(str));
            }

            basic_string& replace(size_type pos, size_type count, const_pointer str, size_type count2)
            {
              THROW_OUT_OF_RANGE_IF(pos > size_, "basic_string<Char, Traits>::replace's pos out of range");
              return replace_cstr(buffer_ + pos, count, str, count2);
            }
            basic_string& replace(const_iterator first, const_iterator last, const_pointer str, size_type count)
            {
              MYSTL_DEBUG(begin() <= first && last <= end() && first <= last);
              return replace_cstr(first, static_cast<size_type>(last - first), str, count);

            }

            basic_string& replace(size_type pos, size_type count, size_type count2, value_type ch)
            {
              THROW_OUT_OF_RANGE_IF(pos > size_, "basic_string<Char, Traits>::replace's pos out of range");
              return replace_fill(buffer_ + pos, count, count2, ch);
            }
            basic_string& replace(const_iterator first, const_iterator last, size_type count, value_type ch)
            {
              MYSTL_DEBUG(begin() <= first && last <= end() && first <= last);
              return replace_fill(first, static_cast<size_type>(last - first), count, ch);
            }

            basic_string& replace(size_type pos1, size_type count1, const basic_string& str,
                                  size_type pos2, size_type count2 = npos)
            {
              THROW_OUT_OF_RANGE_IF(pos1 > size_ || pos2 > str.size_,
                                    "basic_string<Char, Traits>::replace's pos out of range");
              return replace_cstr(buffer_ + pos1, count1, str.buffer_ + pos2, count2);
            }

            template <class Iter, typename std::enable_if<
              mystl::is_input_iterator<Iter>::value, int>::type = 0>
            basic_string& replace(const_iterator first, const_iterator last, Iter first2, Iter last2)
            {
              MYSTL_DEBUG(begin() <= first && last <= end() && first <= last);
              return replace_copy(first, last, first2, last2);
            }

            // reverse
            void reverse() noexcept;

            // swap
            void swap(basic_string& rhs) noexcept;

            // 查找相关操作

            // find
            size_type find(value_type ch, size_type pos = 0)                             const noexcept;
            size_type find(const_pointer str, size_type pos = 0)                         const noexcept;
            size_type find(const_pointer str, size_type pos, size_type count)            const noexcept;
            size_type find(const basic_string& str, size_type pos = 0)                   const noexcept;

            // rfind
            size_type rfind(value_type ch, size_type pos = npos)                         const noexcept;
            size_type rfind(const_pointer str, size_type pos = npos)                     const noexcept;
            size_type rfind(const_pointer str, size_type pos, size_type count)           const noexcept;
            size_type rfind(const basic_string& str, size_type pos = npos)               const noexcept;

            // find_first_of
            size_type find_first_of(value_type ch, size_type pos = 0)                    const noexcept;
            size_type find_first_of(const_pointer s, size_type pos = 0)                  const noexcept;
            size_type find_first_of(const_pointer s, size_type pos, size_type count)     const noexcept;
            size_type find_first_of(const basic_string& str, size_type pos = 0)          const noexcept;

            // find_first_not_of
            size_type find_first_not_of(value_type ch, size_type pos = 0)                const noexcept;
            size_type find_first_not_of(const_pointer s, size_type pos = 0)              const noexcept;
            size_type find_first_not_of(const_pointer s, size_type pos, size_type count) const noexcept;
            size_type find_first_not_of(const basic_string& str, size_type pos = 0)      const noexcept;

            // find_last_of
            size_type find_last_of(value_type ch, size_type pos = 0)                     const noexcept;
            size_type find_last_of(const_pointer s, size_type pos = 0)                   const noexcept;
            size_type find_last_of(const_pointer s, size_type pos, size_type count)      const noexcept;
            size_type find_last_of(const basic_string& str, size_type pos = 0)           const noexcept;

            // find_last_not_of
            size_type find_last_not_of(value_type ch, size_type pos = 0)                 const noexcept;
            size_type find_last_not_of(const_pointer s, size_type pos = 0)               const noexcept;
            size_type find_last_not_of(const_pointer s, size_type pos, size_type count)  const noexcept;
            size_type find_last_not_of(const basic_string& str, size_type pos = 0)       const noexcept;

            // count
            size_type count(value_type ch, size_type pos = 0) const noexcept;

            public:
             basic_string& operator+=(const basic_string& str)
            { return append(str); }
            basic_string& operator+=(value_type ch)
            { return append(1, ch); }
            basic_string& operator+=(const_pointer str)
            { return append(str, str + char_traits::length(str)); }

          friend std::istream& operator >> (std::istream& is, basic_string& str)
          {
            value_type* buf = new value_type[4096];
            is >> buf;//输入流is的内容放到buf中
            basic_string tmp(buf);
            str = std::move(tmp);//右值引用，赋值给str
            delete[]buf;
            return is;
          }

          friend std::ostream& operator << (std::ostream& os, const basic_string& str)
          {
            for (size_type i = 0; i < str.size_; ++i)
              os << *(str.buffer_ + i); //os的内容放到str中
            return os;
          }

          private:
            void try_init() noexcept;
            void          fill_init(size_type n, value_type ch);

            template <class Iter>
            void          copy_init(Iter first, Iter last, mystl::input_iterator_tag);
            template <class Iter>
            void          copy_init(Iter first, Iter last, mystl::forward_iterator_tag);

            void          init_from(const_pointer src, size_type pos, size_type n);

            void          destroy_buffer();

            // get raw pointer
            const_pointer to_raw_pointer() const;

            // shrink_to_fit
            void          reinsert(size_type size);

            // append
            template <class Iter>
            basic_string& append_range(Iter first, Iter last);

            // compare
            int compare_cstr(const_pointer s1, size_type n1, const_pointer s2, size_type n2) const;

            // replace
            basic_string& replace_cstr(const_iterator first, size_type count1, const_pointer str, size_type count2);
            basic_string& replace_fill(const_iterator first, size_type count1, size_type count2, value_type ch);
            template <class Iter>
            basic_string& replace_copy(const_iterator first, const_iterator last, Iter first2, Iter last2);

            // reallocate
            void          reallocate(size_type need);
            iterator      reallocate_and_fill(iterator pos, size_type n, value_type ch);
            iterator      reallocate_and_copy(iterator pos, const_iterator first, const_iterator last);
    };

    template<class CharType,class CharTraits>
    basic_string<CharType,CharTraits>& basic_string<CharType,CharTraits>::operator=(const basic_string& rhs){
          if(this!=&rhs){
            basic_string tmp(rhs);
            swap(tmp);
          }
          return *this;
    }

    template<class CharType,class CharTraits>
    basic_string<CharType,CharTraits>& basic_string<CharType,CharTraits>::operator=(basic_string&& rhs) noexcept{
        destroy_buffer();
        buffer_=rhs.buffer_;
        size_=rhs.size_;
        cap_=rhs.cap_;
        rhs.buffer_=nullptr;
        rhs.size_=0;
        rhs.cap_=0;
        return *this;
    }

    //str是另一个字符串
    template <class CharType,class CharTraits>
    basic_string<CharType,CharTraits>& basic_string<CharType,CharTraits>::operator=(const_pointer str){
        const size_type len=char_traits::length(str);
        if(cap_<len){
          auto new_buffer=data_allocator::allocate(len+1);
          data_allocator::deallocate(buffer_);
          buffer_=new_buffer;
          cap_=len+1;
        }
        char_traits::copy(buffer_,str,len);
        size_=len;
        return *this;
    }

    template<class CharType,class CharTraits>
    basic_string<CharType,CharTraits>& basic_string<CharType,CharTraits>::operator=(value_type ch){
      if(cap_==1){
          auto new_buffer=data_allocator::allocate(2);
          data_allocator::deallocate(buffer_);
          buffer_=new_buffer;
          cap_=2;
      }
      *buffer_=ch;
      size_=1;
      return *this;
    }

    template<class CharType,class CharTraits>
    void basic_string<CharType,CharTraits>::reserve(size_type n){//重新分配n个空间
        if(cap_<n){
          THROW_LENGTH_ERROR_IF(n>max_size(),"too long to reverse");
          auto new_buffer=data_allocator::allocate(n);
          char_traits::move(new_buffer,buffer_,size_);//dst,src,n，把this挪到new_buffer处
          buffer_=new_buffer;
          cap_=n;
        }
    }

    //cap_和size_的长度不同时，将cap_缩小至和size_相同
    template<class CharType,class CharTraits>
    void basic_string<CharType,CharTraits>::shrink_to_fit(){
      if(size_!=cap_){
        reinsert(size_);
      }
    }

    template<class CharType,class CharTraits>
    typename basic_string<CharType,CharTraits>::iterator basic_string<CharType,CharTraits>::insert(const_iterator pos,value_type ch){
        iterator r=static_cast<iterator>(pos);
        if(size_==cap_){
            return reallocate_and_fill(r,1,ch);
        }
        char_traits::move(r+1,r,end()-r);//pos之后所有往后移动一位
        ++size_;
        *r=ch;
        return r;
    }

    template<class CharType,class CharTraits>
    typename basic_string<CharType,CharTraits>::iterator basic_string<CharType,CharTraits>::insert(const_iterator pos,size_type count,value_type ch){
        iterator r=static_cast<iterator>(pos);
        if(count==0) return r;
        if(cap_-size_<count){
          return reallocate_and_fill(r,n,ch);
        }
        if(pos==end()){
            char_traits::fill(end(),ch,count);
            size_+=count;
            return r;
        }
        char_traits::move(r+count,r,count);//dst,src,n；pos后的元素往后移动count位
        char_traits::fill(r,ch,count);
        size+=count;
        return r;
    }

    template<class CharType,class CharTraits>
    template<class Iter>
    typename basic_string<CharType,CharTraits>::iterator basic_string<CharType,CharTraits>::insert(const_iterator pos,Iter first,Iter last){
        iterator r=static_cast<iterator>(pos);
        const size_type len=mystl::distance(first,last);
        if(len==0) return r;
        if(cap_-size_<len){
          return reallocate_and_copy(r,first,last);
        }
        if(pos==end()){
            mystl::uninitialized_copy(first,last,end());
            size_+=len;
            return r;
        }
        char_traits::move(r+len,r,len);
        mystl::uninitialized_copy(first,last,r);
        size_+=len;
        return r;
    }

    template<class CharType,class CharTraits>
    basic_string<CharType,CharTraits>& basic_string<CharType,CharTraits>::append(size_type count,value_type ch){
        THROW_LENGTH_ERROR_IF(size_>max_size()-count,"too long to append");
        if(cap_-size_<count){
            reallocate(count);
        }
        char_traits::fill(buffer_+size_,ch,count);//pos,value,n
        size_+=count;
        return *this;
    }

    //在this的末尾加上(str[pos],str[pos+count])
    template<class CharType,class CharTraits>
    basic_string<CharType,CharTraits>& basic_string<CharType,CharTraits>::append(const basic_string& str,size_type pos,size_type count){
        THROW_LENGTH_ERROR_IF(size_>max_size()-count,"too long to append");
        if(count==0) return *this;
        if(cap_-size_<count){
          reallocate(count);
        }
        char_traits::copy(buffer_+size_,str.buffer_+pos,count);
        size_+=count;
        return *this;
    }

    template<class CharType,class CharTraits>
    basic_string<CharType,CharTraits>& basic_string<CharType,CharTraits>::append(const_pointer str,size_type count){
          THROW_LENGTH_ERROR_IF(size_>max_size()-count,"too long");
          if(cap_-size_<count){
            reallocate(count);
          }
          char_traits::copy(buffer_+size_,s,count);
          size_+=count;
          return *this;
    }

    template<class CharType,class CharTraits>
    typename basic_string<CharType,CharTraits>::iterator basic_string<CharType,CharTraits>::erase(const_iterator pos){
        MYSTL_DEBUG(pos!=end());
        iterator r=const_cast<iterator>(pos);
        char_traits::move(r,pos+1,end(),end()-pos-1);//dst,src,n
        --size_;
        return r;
    }

    template<class CharType,class CharTraits>
    typename basic_string<CharType,CharTraits>::iterator basic_string<CharType,CharTraits>::erase(const_iterator first,const_iterator last){
        if(first==begin()&&end==end()){
              clear();
              return end();
        }
        const size_type n=end()-last;
        iterator r=const_cast<iterator>(first);
        char_traits::move(r,last,n);
        size_-=(last-first);
        return r;
    }

    template<class CharType,class CharTraits>
    void basic_string<CharType,CharTraits>::resize(size_type count,value_type ch){
        if(count<size_){
            erase(buffer_+count,buffer+size_);
        }else{
            append(count-size_,ch);
        }
    }

    template<class CharType, class CharTraits>
    int basic_string<CharType, CharTraits>::compare(basic_string& other) const {
            return compare_cstr(buffer_,size_,other.buffer_,other.size_);
    }

    template<class CharType, class CharTraits>
    int basic_string<CharType, CharTraits>::compare(size_type pos1, size_type count1, const basic_string& other) const {
        auto n1=mystl::min(count1,size_-pos1);
        return compare_cstr(buffer_+pos1,n1,other.buffer_,other.size_);
    }

    //this的[pos1,pos1+count1]和other的[pos2,pos2+count2]进行比较
    template<class CharType,class CharTraits>
    int basic_string<CharType,CharTraits>::compare(size_type pos1,size_type count1,const basic_string& other,size_type pos2,size_type count2) const {
        auto n1=mystl::min(size_-pos1,count1);
        auto n2=mystl::min(other.size_-pos2,count2);
        return compare_cstr(buffer_+pos1,n1,other.buffer_+pos2,n2);
    }

    template<class CharType,class CharTraits>
    int basic_string<CharType,CharTraits>::compare(const_pointer s) const {
        auto len=char_traits::length(s);
        return compare_cstr(buffer_,size_,s,len);
    }

    template<class CharType,class CharTraits>
    int basic_string<CharType,CharTraits>::compare(size_type pos1,size_type count1,const_pointer s) const {
        auto n1=mystl::min(count1,size_-pos1);
        auto n2=char_traits::length(s);
        return compare_cstr(buffer_,n1,s,n2);
    }

    // 从下标 pos1 开始的 count1 个字符跟另一个字符串的前 count2 个字符比较
    template <class CharType, class CharTraits>
    int basic_string<CharType, CharTraits>::
    compare(size_type pos1, size_type count1, const_pointer s, size_type count2) const
    {
        auto n1 = mystl::min(count1, size_ - pos1);
        return compare_cstr(buffer_, n1, s, count2);
    }

    template<class CharType, class CharTraits>
    void basic_string<CharType, CharTraits>::reverse() noexcept {
        for(auto i=begin(),j=end();i<j;) {
            mystl::iter_swap(i++,--j);
        }
    }

    template<class CharType, class CharTraits>
    void basic_string<CharType, CharTraits>::swap(basic_string &rhs) noexcept {
        if(this!=&rhs) {
            mystl::swap(buffer_,rhs.buffer_);
            mystl::swap(size_,rhs.size_);
            mystl::swap(cap_,rhs.cap_);
        }
    }

    //查找字符
    template<class CharType,class CharTraits>
    typename basic_string<CharType,CharTraits>::size_type basic_string<CharType,CharTraits>::find(value_type ch,size_type pos) const noexcept {
        for(auto i=pos;i<size_;++i) {
            if(*(buffer_+i)==ch) return i;
        }
        return npos;//最大值，表示没找到
    }

    //从pos开始查找字符串str,若找到则返回起始下标
    //bp算法
    template<class CharType,class CharTraits>
    typename basic_string<CharType,CharTraits>::size_type
    basic_string<CharType,CharTraits>::find(const_pointer str,size_type pos) const noexcept {
        const auto len=char_traits::length(str);
        if(len==0) return pos;
        if(size_-pos<len) return npos;
        const auto left=size_-len;
        for(auto i=pos;i<=left;++i) {
            if(*(buffer_+i)==*str) {
                size_type j=1;
                for(;j<len;++j) {
                    if(*(buffer_+i+j)!=*(str+j)) break;
                }
                if(j==len) return i;
            }
        }
        return npos;
    }

    //从pos开始查找str的前count个字符
    template<class CharType,class CharTraits>
    typename basic_string<CharType,CharTraits>::size_type
    basic_string<CharType,CharTraits>::find(const_pointer str,size_type pos,size_type count) const noexcept {
        if(count==0) return pos;
        if(size_<pos+count) return npos;
        const auto left=size_-count;
        for(int i=pos;i<=left;++i) {
            if(*(buffer_+i)==*str) {
                size_type j=1;
                for(;j<count;++j) {
                    if(*(buffer_+i+j)!=*(str+j)) break;
                }
                if(j==count) return i;
            }
        }
        return npos;
    }

    template<class CharType,class CharTraits>
    typename basic_string<CharType,CharTraits>::size_type
    basic_string<CharType,CharTraits>::find(const basic_string& str,size_type pos) const noexcept {
        const size_type len =str.size_;
        if(len==0) return pos;
        if(pos+len>size_) return npos;
        const auto left=size_-len;
        for(auto i=pos;i<=left;++i) {
            if(*(buffer_+i)==str.front()) {
                size_type j=1;
                for(;j<len;++j) {
                    if(*(buffer_+i+j)!=str[j]) break;
                }
                if(j==len) return i;
            }
        }
        return npos;
    }
    
    //从pos开始反向查找ch
    //要查找的是 value_type类型的，单个字符
    template<class CharType,class CharTraits>
    typename basic_string<CharType,CharTraits>::size_type basic_string<CharType,CharTraits>::rfind(value_type ch,size_type pos) const noexcept{
            if(pos>=size_){
                pos=size_-1;
            }
            for(auto i=pos;i!=0;--i){
              if(*(buffer_+i)==ch) return i;
            }
            return front()==ch?0:nops;
    }

    //要查找的是const_pointer类型的，长度不固定
    template<class CharType,class CharTraits>
    typename basic_string<CharType,CharTraits>::size_type basic_string<CharType,CharTraits>::rfind(const_pointer str,size_type pos) const noexcept{
        if(pos>=size_) pos=size_-1; //从最后一位开始找
        const size_type len=char_traits::length(str);
        if(len==0) return pos;
        else if(len==1){
            for(auto i=pos;i!=0;--i){
              if(*(buffer_+i)==*str) return i;
            }
            return front()==*str?0:npos;
        }
        else{
            for(auto i=pos;i>=len-1;--i){
                if(*(buffer_+i)==*(str+len-1)){
                    size_type j=1;
                    for(;j<len;++j){
                        if(*(buffer_+i-j)!=*(str+len-1-j)) break;
                    }
                    if(j==len) return i-len+1;
                }
            }
        }
        return npos;
    }

    //const_pointer
    template<class CharType,class CharTraits>
    typename basic_string<CharType,CharTraits>::size_type basic_string<CharType,CharTraits>::rfind(const_pointer str,size_type pos,size_type count) const noexcept{
        if(count==0) return pos;
        if(pos>=size_) pos=size_-1;
        if(pos<count-1) return npos;
        for(auto i=pos;i>=count-1;--i){
          if(*(buffer+i)==*(str+count-1)){
              size_type j=1;
              for(;j<count;++j){
                if(*(buffer_+i-j)!=*(str+count-1-j)) break;
              }
              if(j==count) return i-count+1;
          }
        }
        return npos;
    }

    template<class CharType,class CharTraits>
    typename basic_string<CharType,CharTraits>::size_type basic_string<CharType,CharTraits>::rfind(const basic_string& str,size_type pos) const noexcept{
        const size_type count=str.size_;
        if(pos>=size_) pos=size_-1;
        if(count==0) return pos;
        if(pos<count-1) return npos;
        for(auto i=pos;i>=count-1;--i){
            if(*(buffer_+i)==str[count-1]){
                size_type j=1;
                for(;j<count;++j){
                    if(*(buffer_+i-j)!=str[count-1-j]) break;
                }
                if(j==count) return i-count+1;
            }
        }
        return npos;
    }

    template<class CharType,class CharTraits>
    typename basic_string<CharType,CharTraits>::size_type basic_string<CharType,CharTraits>::find_first_of(value_type ch,size_type pos) const noexcept{
          for(auto i=pos;i<size_;++i){
            if(*(buffer_+i)==ch) return i;
          }
          return npos;
    }

    //只返回第一个匹配的字母的位置
    template<class CharType,class CharTraits>
    typename basic_string<CharType,CharTraits>::size_type basic_string<CharType,CharTraits>::find_first_of(const_pointer s,size_type pos) const noexcept{
        const size_type len=char_traits::length(s);
        for(auto i=pos;i<size_;++i){
            value_type ch=*(buffer_+i);
            for(size_type j=0;j<len;++j){
              if(ch==*(s+j)) return i;
            }
        }
        return npos;
    }

    template<class CharType,class CharTraits>
    typename basic_string<CharType,CharTraits>::size_type basic_string<CharType,CharTraits>::find_first_of(const_pointer s,size_type pos,size_type count) const noexcept{
        for(auto i=pos;i<size_;++i){
              value_type ch=*(buffer_+i);
              for(size_type j=0;j<count;++j){
                  if(ch==*(s+j)) return i;
              }
        }
        return npos;
    }

    template<class CharType,class CharTraits>
    typename basic_string<CharType,CharTraits>::size_type basic_string<CharType,CharTraits>::find_first_of(const basic_string& s,size_type pos) const noexcept{
          const size_type count=char_traits::length(s);
          for(auto i=pos;i<size_;++i){
              value_type ch=*(buffer_+i);
              for(size_type j=0;j<count;++j){
                  if(ch==s[j]) return i;
              }
          }
          return npos;
    }

    //查找第一个不匹配的
    template<class CharType,class CharTraits>
    typename basic_string<CharType,CharTraits>::size_type basic_string<CharType,CharTraits>::find_first_not_of(value_type ch,size_type pos) const noexcept{
        for(auto i=pos;i<size_;++i){
            if(*(buffer_+i)!=ch) return i;
        }
        return npos;
    }

    template<class CharType,class CharTraits>
    typename basic_string<CharType,CharTraits>::size_type basic_string<CharType,CharTraits>::find_first_not_of(const_pointer s,size_type pos) const noexcept{
        const size_type count=char_traits::length(s);
        for(auto i=pos;i<size_;++i){
            value_type ch=*(buffer_+i);
            for(size_type j=0;j<count;++j){
                if(ch!=*(s+j)) return i;
            }
        }
        return npos;
    }
    // 从下标 pos 开始查找与字符串 s 前 count 个字符中不相等的第一个位置
    template <class CharType, class CharTraits>
    typename basic_string<CharType, CharTraits>::size_type
    basic_string<CharType, CharTraits>::
    find_first_not_of(const_pointer s, size_type pos, size_type count) const noexcept
    {
      for (auto i = pos; i < size_; ++i)
      {
        value_type ch = *(buffer_ + i);
        for (size_type j = 0; j < count; ++j)
        {
          if (ch != *(s + j))
            return i;
        }
      }
      return npos;
    }

    // 从下标 pos 开始查找与字符串 str 的字符中不相等的第一个位置
    template <class CharType, class CharTraits>
    typename basic_string<CharType, CharTraits>::size_type
    basic_string<CharType, CharTraits>::
    find_first_not_of(const basic_string& str, size_type pos) const noexcept
    {
      for (auto i = pos; i < size_; ++i)
      {
        value_type ch = *(buffer_ + i);
        for (size_type j = 0; j < str.size_; ++j)
        {
          if (ch != str[j])
            return i;
        }
      }
      return npos;
    }

    // 从下标 pos 开始查找与 ch 相等的最后一个位置
    template <class CharType, class CharTraits>
    typename basic_string<CharType, CharTraits>::size_type
    basic_string<CharType, CharTraits>::
    find_last_of(value_type ch, size_type pos) const noexcept
    {
      for (auto i = size_ - 1; i >= pos; --i)
      {
        if (*(buffer_ + i) == ch)
          return i;
      }
      return npos;
    }

    // 从下标 pos 开始查找与字符串 s 其中一个字符相等的最后一个位置
    template <class CharType, class CharTraits>
    typename basic_string<CharType, CharTraits>::size_type
    basic_string<CharType, CharTraits>::
    find_last_of(const_pointer s, size_type pos) const noexcept
    {
      const size_type len = char_traits::length(s);
      for (auto i = size_ - 1; i >= pos; --i)
      {
        value_type ch = *(buffer_ + i);
        for (size_type j = 0; j < len; ++j)
        {
          if (ch == *(s + j))
            return i;
        }
      }
      return npos;
    }

    // 从下标 pos 开始查找与字符串 s 前 count 个字符中相等的最后一个位置
    template <class CharType, class CharTraits>
    typename basic_string<CharType, CharTraits>::size_type
    basic_string<CharType, CharTraits>::
    find_last_of(const_pointer s, size_type pos, size_type count) const noexcept
    {
      for (auto i = size_ - 1; i >= pos; --i)
      {
        value_type ch = *(buffer_ + i);
        for (size_type j = 0; j < count; ++j)
        {
          if (ch == *(s + j))
            return i;
        }
      }
      return npos;
    }

    // 从下标 pos 开始查找与字符串 str 字符中相等的最后一个位置
    template <class CharType, class CharTraits>
    typename basic_string<CharType, CharTraits>::size_type
    basic_string<CharType, CharTraits>::
    find_last_of(const basic_string& str, size_type pos) const noexcept
    {
      for (auto i = size_ - 1; i >= pos; --i)
      {
        value_type ch = *(buffer_ + i);
        for (size_type j = 0; j < str.size_; ++j)
        {
          if (ch == str[j])
            return i;
        }
      }
      return npos;
    }

    // 从下标 pos 开始查找与 ch 字符不相等的最后一个位置
    template <class CharType, class CharTraits>
    typename basic_string<CharType, CharTraits>::size_type
    basic_string<CharType, CharTraits>::
    find_last_not_of(value_type ch, size_type pos) const noexcept
    {
      for (auto i = size_ - 1; i >= pos; --i)
      {
        if (*(buffer_ + i) != ch)
          return i;
      }
      return npos;
    }

    // 从下标 pos 开始查找与字符串 s 的字符中不相等的最后一个位置
    template <class CharType, class CharTraits>
    typename basic_string<CharType, CharTraits>::size_type
    basic_string<CharType, CharTraits>::
    find_last_not_of(const_pointer s, size_type pos) const noexcept
    {
      const size_type len = char_traits::length(s);
      for (auto i = size_ - 1; i >= pos; --i)
      {
        value_type ch = *(buffer_ + i);
        for (size_type j = 0; j < len; ++j)
        {
          if (ch != *(s + j))
            return i;
        }
      }
      return npos;
    }

    // 从下标 pos 开始查找与字符串 s 前 count 个字符中不相等的最后一个位置
    template <class CharType, class CharTraits>
    typename basic_string<CharType, CharTraits>::size_type
    basic_string<CharType, CharTraits>::
    find_last_not_of(const_pointer s, size_type pos, size_type count) const noexcept
    {
      for (auto i = size_ - 1; i >= pos; --i)
      {
        value_type ch = *(buffer_ + i);
        for (size_type j = 0; j < count; ++j)
        {
          if (ch != *(s + j))
            return i;
        }
      }
      return npos;
    }

    // 从下标 pos 开始查找与字符串 str 字符中不相等的最后一个位置
    template <class CharType, class CharTraits>
    typename basic_string<CharType, CharTraits>::size_type
    basic_string<CharType, CharTraits>::
    find_last_not_of(const basic_string& str, size_type pos) const noexcept
    {
      for (auto i = size_ - 1; i >= pos; --i)
      {
        value_type ch = *(buffer_ + i);
        for (size_type j = 0; j < str.size_; ++j)
        {
          if (ch != str[j])
            return i;
        }
      }
      return npos;
    }

    // 返回从下标 pos 开始字符为 ch 的元素出现的次数
    template <class CharType, class CharTraits>
    typename basic_string<CharType, CharTraits>::size_type
    basic_string<CharType, CharTraits>::
    count(value_type ch, size_type pos) const noexcept
    {
      size_type n = 0;
      for (auto i = pos; i < size_; ++i)
      {
        if (*(buffer_ + i) == ch)
          ++n;
      }
      return n;
    }

    //初始化一段buffer
    template<class CharType,class CharTraits>
    void basic_string<CharType,CharTraits>::try_init() noexcept{
        try{
          buffer_=data_allocator::allocate(static_cast<size_type>(STRING_INIT_SIZE));
          size_=0;
          cap_=0;
        }catch(...){
          buffer_=nullptr;
          size_=0;
          cap_=0;
        }
    }

    template<class CharType,class CharTraits>
    void basic_string<CharType,CharTraits>::fill_init(size_type n,value_type ch){
        const auto init_size=mystl::max(static_cast<size_type>(STRING_INIT_SIZE),n+1);
        buffer_=data_allocator::allocate(init_size);
        char_traits::fill(buffer_,ch,n);
        size_=n;
        cap_=init_size;
    }

    template<class CharType,class CharTraits>
    template<class Iter>
    void basic_string<CharType,CharTraits>::copy_init(Iter first,Iter last,mystl::input_iterator_tag){
        size_type n=mystl::distance(first,last);
        const auto init_size=mystl::max(static_cast<size_type>(STRING_INIT_SIZE),n+1);
        try{
            buffer_=data_allocator::allocate(init_size);
            size_=n;
            cap_=init_size;
        }
        catch(...){
            buffer_=nullptr;
            size_=0;
            cap_=0;
            throw;
        }
        for(;n>0;--n,++first){
          append(*first);//append逐个添加，只能顺序读取
        }
    }

     template<class CharType,class CharTraits>
    template<class Iter>
    void basic_string<CharType,CharTraits>::copy_init(Iter first,Iter last,mystl::forward_iterator_tag){
        size_type n=mystl::distance(first,last);
        const auto init_size=mystl::max(static_cast<size_type>(STRING_INIT_SIZE),n+1);
        try{
            buffer_=data_allocator::allocate(init_size);
            size_=n;
            cap_=init_size;
            mystl::uninitialized_copy(first,last,buffer_); //可多次遍历同一区间
        }
        catch(...){
            buffer_=nullptr;
            size_=0;
            cap_=0;
            throw;
        }
    }

    template<class CharType,class CharTraits>
    void basic_string<CharType,CharTraits>::init_from(const_pointer src,size_type pos,size_type count){
      const auto init_size=mystl::max(static_cast<size_type>(STRING_INIT_SIZE),count+1);
      buffer_=data_allocator::allocate(init_size);
      char_traits::copy(buffer_,src+pos,count);
      size_=count;
      cap_=init_size;
    }

    template<class CharType,class CharTraits>
    void basic_string<CharType,CharTraits>::destroy_buffer(){
        if(buffer_!=nullptr){
          data_allocator::deallocate(buffer_,cap_);
          buffer_=nullptr;
          size_=0;
          cap_=0;
        }
    }

    template<class CharType,class CharTraits>
    typename basic_string<CharType,CharTraits>::const_pointer basic_string<CharType,CharTraits>::to_raw_pointer() const{
        *(buffer_+size_)=value_type();
        return buffer_;
    }

    //重新申请一片空间，插入,用move，避免覆盖
    template<class CharType,class CharTraits>
    void basic_string<CharType,CharTraits>::reinsert(size_type size){
        auto new_buffer=data::allocator::allocate(size);
        try{
            char_traits::move(new_buffer,buffer_,size);
        }
        catch(...){
          data_allocator::deallocate(new_buffer);
        }
        buffer_=new_buffer;
        size_=size;
        cap_=size;
    }

    //在末尾添加
    template<class CharType,class CharTraits>
    template<class Iter>
    basic_string<CharType,CharTraits>& basic_string<CharType,CharTraits>::append_range(Iter first,Iter last){
        const size_type n=mystl::distance(first,last);
        THROW_LENGTH_ERROR_IF(size_>max_size()-n,"too long");
        if(cap_-size_<n) reallocate(n);
        mystl::uninitialized_copy_n(first,n,buffer_+size_);
        size_+=n;
        return *this;
    }

    template<class CharType,class CharTraits>
    int basic_string<CharType,CharTraits>::compare_cstr(const_pointer s1,size_type n1,const_pointer s2,size_type n2) const{
        auto rlen=mystl::min(n1,n2);
        auto res=char_traits::compare(s1,s2,rlen);
        if(res!=0) return res;
        if(n1<n2) return -1;
        if(n1>n2) return 1;
        return 0;
    }

    template<class CharType,class CharTraits>
    basic_string<CharType,CharTraits>& basic_string<CharType,CharTraits>::replace_cstr(const_iterator first,size_type count1,const_pointer str,size_type count2){
        if(static_cast<size_type>(cend()-first)<count1) count1=cend()-first;
        if(count1<count2) {
            const size_type add=count2-count1;
            THROW_LENGTH_ERROR_IF(size_>max_size()-add,"too long");
            if(size_>cap_+add) {
                reallocate(add);
            }
            pointer r=const_cast<pointer>(first);
            char_traits::move(r+count2,first+count1,end()-(first+count1));//空出count2的距离
            char_traits::copy(r,str,count2);
            size_+=add;
        }else {
            pointer r=const_pointer<pointer>(first);
            char_traits::move(r+count2,first+count1.end()-(first+count1));
            char_traits::copy(r,str,count2);
            size_-=(count1-count2);
        }
        return *this;
    }

    template<class CharType, class CharTraits>
    basic_string<CharType, CharTraits> &basic_string<CharType, CharTraits>::replace_fill(const_iterator first, size_type count1, size_type count2, value_type ch) {
        if(static_cast<size_type>(cend()-first)<count1) count1=cend()-first;
        if(count1<count2) {
            const size_type add=count2-count1;
            THROW_LENGTH_ERROR_IF(size()>max_size()-add,"too long");
            if(size_>cap_-add) reallocate(add);
            pointer r =const_cast<pointer>(first);
            char_traits::move(r+count2,first+count1,end()-(first+count1));
            char_traits::fill(r,ch,count2);
            size_+=add;
        }else {
            pointer r=const_cast<pointer>(first);
            char_traits::move(r+count2,first+count1,end()-(first+count1));
            char_traits::fill(r,ch,count2);
            size_-=(count1-count2);
        }
        return *this;
    }

    template<CharType,CharTraits>
    template<Iter>
    basic_string<CharType,CharTraits> &basic_string<CharType,CharTraits>::replace_copy(const_iterator first,const_iterator last,Iter first2,Iter last2) {
        size_type len1=last-first;
        size_type len2=last2-first2;
        if(len1<len2) {
            const size_type add=len2-len1;
            THROW_LENGTH_ERROR_IF(size()>max_size()-add,"too long");
            if(size_>cap_-add) reallocate(add);
            pointer r =const_cast<pointer>(first);
            char_traits::move(r+len2,first+len1,end()-(first+len1));
            char_traits::copy(r,first2,len2);
            size_+=add;
        }else {
            pointer r = const_cast<pointer>(first);
            char_traits::move(r + len2, first + len1, end() - (first + len1));
            char_traits::copy(r, first2, len2);
            size_ -= (len1 - len2);
        }
        return *this;
    }

    template<class CharType,class CharTraits>
    void basic_string<CharTraits,CharType>::reallocate(size_type need) {
        const auto new_cap=mystl::max(cap_+need,cap_(cap_>>1));
        auto new_buffer=data_allocator::allocate(new_cap);
        char_traits::move(new_buffer,buffer_,size_);
        data_allocator::deallocate(buffer_);
        buffer_=new_buffer;
        cap_=new_cap;
    }

    template<class CharType, class CharTraits>
    typename basic_string<CharType, CharTraits>::iterator basic_string<CharType, CharTraits>::reallocate_and_fill(iterator pos, size_type n, value_type ch) {
        const auto r=pos-buffer_;
        const auto old_cap=cap_;
        const auto new_cap=mystl::max(n+old_cap,old_cap+(old_cap>>1));
        auto new_buffer=data_allocator::allocate(new_cap);
        auto e1=char_traits::move(new_buffer,buffer_,r)+r;//新缓冲区的待插入区
        auto e2=char_traits::fill(e1,ch,n)+n;
        char_traits::move(e2,buffer_+r,size_-r);
        data_allocator::deallocate(buffer_,old_cap);
        buffer_=new_buffer;
        size_+=n;
        cap_=new_cap;
        return buffer_+r;
    }

    template<class CharType,class CharTraits>
    typename basic_string<CharType,CharTraits>::iterator basic_string<CharType,CharType>::reallocate_and_copy(iterator pos,const_iterator first,const_iterator last) {
        const auto r=pos-buffer_;
        const auto old_cap=cap_;
        const size_type n=mystl::distance(first,last);
        const auto new_cap=mystl::max(old_cap+n,old_cap+(old_cap>>1));
        auto new_buffer=data::allocate(new_cap);
        auto e1=char_traits::move(new_buffer,buffer_,r)+r;
        auto e2=mystl::uninitialized_copy_n(first,n,e1)+n;
        char_traits::move(e2,buffer_+r,size_-r);
        data_allocator::deallocate(buffer_,old_cap);
        buffer_=new_buffer;
        size_+=n;
        cap_=new_cap;
        return buffer_+r;
    }

    template<class CharType,class CharTraits>
    basic_string<CharType,CharTraits> operator+(const basic_string<CharType,CharTraits>& lhs,const basic_string<CharType,CharTraits>& rhs) {
        basic_string<CharType,CharTraits> tmp(lhs);//每次拼接都是申请了一片新的空间
        tmp.append(rhs);
        return tmp;
    }
    template <class CharType, class CharTraits>
basic_string<CharType, CharTraits>
operator+(const CharType* lhs, const basic_string<CharType, CharTraits>& rhs)
{
  basic_string<CharType, CharTraits> tmp(lhs);
  tmp.append(rhs);
  return tmp;
}

template <class CharType, class CharTraits>
basic_string<CharType, CharTraits>
operator+(CharType ch, const basic_string<CharType, CharTraits>& rhs)
{
  basic_string<CharType, CharTraits> tmp(1, ch);
  tmp.append(rhs);
  return tmp;
}

template <class CharType, class CharTraits>
basic_string<CharType, CharTraits>
operator+(const basic_string<CharType, CharTraits>& lhs, const CharType* rhs)
{
  basic_string<CharType, CharTraits> tmp(lhs);
  tmp.append(rhs);
  return tmp;
}

template <class CharType, class CharTraits>
basic_string<CharType, CharTraits>
operator+(const basic_string<CharType, CharTraits>& lhs, CharType ch)
{
  basic_string<CharType, CharTraits> tmp(lhs);
  tmp.append(1, ch);
  return tmp;
}

template <class CharType, class CharTraits>
basic_string<CharType, CharTraits>
operator+(basic_string<CharType, CharTraits>&& lhs,
          const basic_string<CharType, CharTraits>& rhs)
{
  basic_string<CharType, CharTraits> tmp(mystl::move(lhs));
  tmp.append(rhs);
  return tmp;
}

template <class CharType, class CharTraits>
basic_string<CharType, CharTraits>
operator+(const basic_string<CharType, CharTraits>& lhs,
          basic_string<CharType, CharTraits>&& rhs)
{
  basic_string<CharType, CharTraits> tmp(mystl::move(rhs));
  tmp.insert(tmp.begin(), lhs.begin(), lhs.end());
  return tmp;
}

template <class CharType, class CharTraits>
basic_string<CharType, CharTraits>
operator+(basic_string<CharType, CharTraits>&& lhs,
          basic_string<CharType, CharTraits>&& rhs)
{
  basic_string<CharType, CharTraits> tmp(mystl::move(lhs));
  tmp.append(rhs);
  return tmp;
}

template <class CharType, class CharTraits>
basic_string<CharType, CharTraits>
operator+(const CharType* lhs, basic_string<CharType, CharTraits>&& rhs)
{
  basic_string<CharType, CharTraits> tmp(mystl::move(rhs));
  tmp.insert(tmp.begin(), lhs, lhs + char_traits<CharType>::length(lhs));
  return tmp;
}

template <class CharType, class CharTraits>
basic_string<CharType, CharTraits>
operator+(CharType ch, basic_string<CharType, CharTraits>&& rhs)
{
  basic_string<CharType, CharTraits> tmp(mystl::move(rhs));
  tmp.insert(tmp.begin(), ch);
  return tmp;
}

template <class CharType, class CharTraits>
basic_string<CharType, CharTraits>
operator+(basic_string<CharType, CharTraits>&& lhs, const CharType* rhs)
{
  basic_string<CharType, CharTraits> tmp(mystl::move(lhs));
  tmp.append(rhs);
  return tmp;
}

template <class CharType, class CharTraits>
basic_string<CharType, CharTraits>
operator+(basic_string<CharType, CharTraits>&& lhs, CharType ch)
{
  basic_string<CharType, CharTraits> tmp(mystl::move(lhs));
  tmp.append(1, ch);
  return tmp;
}

// 重载比较操作符
template <class CharType, class CharTraits>
bool operator==(const basic_string<CharType, CharTraits>& lhs,
                const basic_string<CharType, CharTraits>& rhs)
{
  return lhs.size() == rhs.size() && lhs.compare(rhs) == 0;
}

template <class CharType, class CharTraits>
bool operator!=(const basic_string<CharType, CharTraits>& lhs,
                const basic_string<CharType, CharTraits>& rhs)
{
  return lhs.size() != rhs.size() || lhs.compare(rhs) != 0;
}

template <class CharType, class CharTraits>
bool operator<(const basic_string<CharType, CharTraits>& lhs,
               const basic_string<CharType, CharTraits>& rhs)
{
  return lhs.compare(rhs) < 0;
}

template <class CharType, class CharTraits>
bool operator<=(const basic_string<CharType, CharTraits>& lhs,
                const basic_string<CharType, CharTraits>& rhs)
{
  return lhs.compare(rhs) <= 0;
}

template <class CharType, class CharTraits>
bool operator>(const basic_string<CharType, CharTraits>& lhs,
               const basic_string<CharType, CharTraits>& rhs)
{
  return lhs.compare(rhs) > 0;
}

template <class CharType, class CharTraits>
bool operator>=(const basic_string<CharType, CharTraits>& lhs,
                const basic_string<CharType, CharTraits>& rhs)
{
  return lhs.compare(rhs) >= 0;
}

// 重载 mystl 的 swap
template <class CharType, class CharTraits>
void swap(basic_string<CharType, CharTraits>& lhs,
          basic_string<CharType, CharTraits>& rhs) noexcept
{
  lhs.swap(rhs);
}

// 特化 mystl::hash
template <class CharType, class CharTraits>
struct hash<basic_string<CharType, CharTraits>> {
        size_t operator()(const basic_string<CharType, CharTraits>& str)
        {
            return bitwise_hash((const unsigned char*)str.c_str(),
                                str.size() * sizeof(CharType));
        }
    };

}
#endif