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

#define STRING_INIT_SIZE 32
    //basic_string
    template<class CharType,class CharTraits=mystl::char_traits<CharType>>
    class basic_string {

    };
}
#endif