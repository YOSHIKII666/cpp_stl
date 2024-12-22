//
// Created by 王少泽 on 2024/12/21.
//

#ifndef ALINSHANS_REDBUD_IO_COLOR_H_
#define ALINSHANS_REDBUD_IO_COLOR_H_

#include "../platform.h"

#if defined(REDBUD_LINUX)||defined(REDBUD_OSX)
    #include<unistd.h>
    #include<cstring>
#elif defined(REDBUD_WIN)
    #include <Windows.h>
    #include <io.h>
#endif

#include <cstdlib>
#include<iostream>
#include<type_traits>

namespace redbud {
    namespace io {
        enum class format
        {
            reset      = 0,  // All attributes off.
            bold       = 1,  // Bold or increased intensity.
            faint      = 2,  // Faint (decreased intensity).
            italic     = 3,  // Italian font.
            underline  = 4,  // Underline.
            blinkslow  = 5,  // Blink slowly.
            blinkrapid = 6,  // Blink quickly.
            inverse    = 7,  // Swap foreground and background.
            conceal    = 8,  // Hide the text.
            strikeline = 9   // Characters legible, but marked for deletion.
          };

        // Sets the foreground color.
        enum class fg
        {
            black   = 30,
            red     = 31,
            green   = 32,
            yellow  = 33,
            blue    = 34,
            purple  = 35,
            cyan    = 36,
            white   = 37,
            reserve = 38,
            reset   = 39
          };

        // Sets the background color.
        enum class bg
        {
            black   = 40,
            red     = 41,
            green   = 42,
            yellow  = 43,
            blue    = 44,
            purple  = 45,
            cyan    = 46,
            white   = 47,
            reserve = 38,
            reset   = 39
          };

        // Sets the highlight foreground color.
        enum class hfg
        {
            black  = 90,
            red    = 91,
            green  = 92,
            yellow = 93,
            blue   = 94,
            purple = 95,
            cyan   = 96,
            white  = 97
          };

        // Sets the highlight background color.
        enum class hbg
        {
            black  = 100,
            red    = 101,
            green  = 102,
            yellow = 103,
            blue   = 104,
            purple = 105,
            cyan   = 106,
            white  = 107
          };

        // Sets the control state.
        enum class state
        {
            automatic = 0,  // Automatic control.
            manual    = 1   // Manual control.
        };

        namespace details {
            inline const std::streambuf*& get_coutbuf() {
                static const std::streambuf* pout=std::cout.rdbuf(); //cout标准输出流,rdbuf,表示获取缓冲区对象
                return pout;
            }

            inline const std::streambuf*& get_cerrbuf() {
                static const std::streambuf* perr=std::cerr.rdbuf();//cerr,错误输出流.获取其缓冲区
                return perr;
            }

            inline const std::streambuf*& get_clogbuf() {
                static const std::streambuf* plog=std::clog.rdbuf();
                return plog;
            }

            inline int get_iword() {
                //std::ios_base 是 C++ 标准库中所有输入 / 输出流类（如 std::istream、std::ostream 等）的基类
                //xalloc 函数的作用是分配一个新的、可用于用户自定义数据的索引值。
                //这个索引值在整个程序的流对象范围内是唯一的，后续可以基于这个索引值，
                //通过流对象来存储和获取相应的用户自定义整数值。
                static int i=std::ios_base::xalloc();
                return i;
            }

            inline bool is_modifiable() {
            #if defined(REDBUD_LINUX)||defined(REDBUD_OSX)
                static constexpr const char* terms[]={
                    "ansi", "color", "console", "cygwin", "gnome", "konsole", "kterm",
                "linux", "msys", "putty", "rxvt", "screen", "vt100", "xterm"
        };
                const char *penv = std::getenv("TERM");
                if(penv==nullptr){
                    return false;
                }
                bool result=false;
                for(const auto& t :terms){
                    if(std::strstr(penv,t)!=nullptr){
                        result=true;
                        break;
                    }
                }
            #elif defined(REDBUD_WIN)
                static constexpr bool result=true;
            #endif
                return result;
            };

            inline bool is_terminal(const std::streambuf* buf) {
                if(buf == get_coutbuf()) {
                    #if defined(REDBUD_LINUX)|| defined(REDBUD_OSX)
                    //fileno,它的作用是获取给定的文件流对应的文件描述符。
                    //文件描述符是一个非负整数，用于在操作系统层面标识一个打开的文件或者设备。
                    //用于判断给定的文件描述符是否关联到一个终端设备。如果文件描述符对应的是一个终端设备（如控制台、终端模拟器等）
                    //isatty函数返回一个非零值（在 C/C++ 中通常将非零值视为true）；
                    //如果不是终端设备（例如，输出被重定向到一个文件或者管道等情况），则返回0（在 C/C++ 中通常将0视为false）
                    return isatty(fileno(stdout))?true:false;
                    #elif defined(REDBUD_WIN)
                                        return _isatty(_fileno(stdout))?true:false;
                    #endif
                                    }
                                    if(buf==get_cerrbuf() || buf == get_clogbuf()){
                    #if defined(REDBUD_LINUX) || defined(REDBUD_OSX)
                                        return isatty(fileno(stderr)) ? true:false;
                    #elif defined(REDBUD_WIN)
                                        return _isatty(_fileno(stderr))? true:false;
                    #endif
                }
                return false;
            }

            template <typename T>
              using color_return_t = typename std::enable_if<
                std::is_same<T, redbud::io::format>::value ||
                std::is_same<T, redbud::io::fg>::value ||
                std::is_same<T, redbud::io::bg>::value ||
                std::is_same<T, redbud::io::hfg>::value ||
                std::is_same<T, redbud::io::hbg>::value,
                std::ostream&>::type;

            template<typename T>
            using state_return_t=typename std::enable_if<std::is_same<T,redbud::io::state>::value,std::ostream&>::type;

            #if defined(REDBUD_LINUX) || defined(REDBUD_OSX)
            template<typename T>
            inline color_return_t<T> set_color(std::ostream& os, const T& value)
            {
                return os << "\033[" << static_cast<int>(value) << "m";
            }

            #elif defined(REDBUD_WIN)

            static constexpr WORD default_state = (FOREGROUND_BLUE |
                                                   FOREGROUND_GREEN |
                                                   FOREGROUND_RED);

            // Gets the corresponding RGB value on Windows.
            inline WORD get_rgb(WORD rgb)
            {
                static constexpr WORD cor[8] = { 0, 4, 2, 6, 1, 5, 3, 7 };
                return cor[rgb];
            }

            // Sets font attributes on Windows.
            inline void set_attributes(redbud::io::fg color, WORD& state)
            {
                if (color == redbud::io::fg::reserve)
                {
                    return;
                }
                state &= 0xFFF0;
                if (color == redbud::io::fg::reset)
                {
                    state |= default_state;
                    return;
                }
                state |= get_rgb(static_cast<WORD>(color) - 30);
            }

            inline void set_attributes(redbud::io::bg color, WORD& state)
            {
                if (color == redbud::io::bg::reserve)
                {
                    return;
                }
                state &= 0xFF0F;
                if (color == redbud::io::bg::reset)
                {
                    return;
                }
                state |= get_rgb(static_cast<WORD>(color) - 40) << 4;
            }

            inline void set_attributes(redbud::io::hfg color, WORD& state)
            {
                state &= 0xFFF0;
                state |= (static_cast<WORD>(0x8) |
                          get_rgb(static_cast<WORD>(color) - 90));
            }

            inline void set_attributes(redbud::io::hbg color, WORD& state)
            {
                state &= 0xFF0F;
                state |= (static_cast<WORD>(0x80) |
                          get_rgb(static_cast<WORD>(color) - 100) << 4);
            }

            inline void set_attributes(redbud::io::format format, WORD& state)
            {
                if (format == redbud::io::format::reset)
                {
                    state = default_state;
                }
            }

            inline WORD& current_state()
            {
                static WORD state = default_state;
                return state;
            }

            inline HANDLE get_console_handle()
            {
                static HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
                return h;
            }

            template <typename T>
            inline color_return_t<T> set_color(std::ostream& os, const T& value)
            {
                HANDLE h = get_console_handle();
                if (h && is_terminal(os.rdbuf()))
                {
                    set_attributes(value, current_state());
                    SetConsoleTextAttribute(h, current_state());
                    return os;
                }
                return os;
            }
            #endif
        }

        template<typename T>
        inline details::color_return_t<T> operator<<(std::ostream& os,const T& value) {
            //long& iword(int index);。这里的 index 参数是一个整数，
            //这个整数通常是通过调用 std::ios_base::xalloc() 函数获取到的
            return (os.iword(details::get_iword())||(details::is_modifiable()&&details::is_terminal(os.rdbuf())))?details::set_color(os,value):os;
        }

        template<typename T>
        inline details::state_return_t<T>
        operator<<(std::ostream& os,const T& value) {
            if(value==redbud::io::state::automatic) {
                os.iword(details::get_iword())=0;
            }
            else if(value==redbud::io::state::manual) {
                os.iword(details::get_iword())=1;
            }
            return os;
        }

    }
}
#endif