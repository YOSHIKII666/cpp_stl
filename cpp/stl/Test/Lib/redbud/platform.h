//
// Created by 王少泽 on 2024/12/21.
//

#ifndef ALINSHANS_REDBUD_PLATFORM_H_
#define ALINSHANS_REDBUD_PLATFORM_H_

#if defined(WIN32) || defined(_WIN32) || defined(_WIN64)

#elif defined(__unix__) || defined(__unix) ||defined(__linux__)

#elif defined(__APPLE__)|| defined(__MACH__)
    #define REDBUD_OSX=1//用这个宏定义是否用的是mac系统
#else
    #error "System that is not support yet."
#endif

//编译器

#if defined(__clang__)
    #define REDBUD_CLANG __clang__
#elif defined(__GUNC__)
    #define REDBUD_GUNC __GNUC__
#elif defined(_MSC_VER)
    #define REDBUD_MSCV _MSC_VER
#else
    #error "Complier is  not supported yet"
#endif

#define REDBUD_TO_STRING(x) #x //#是字符串转换操作，可以把输入转换成字符串
#define REDBUD_STRING(x) REDBUD_TO_STRING(x)


#define REDBUD_DO_JOIN(x,y) x##y
#define REDBUD_JOIN(x,y) REDBUD_DO_JOIN(x,y)
    //#define REDBUD_DO_JOIN(x,y) x##y
    //int REDBUD_DO_JOIN(var,1) = 10; =====     int var1=10

#define REDBUD_MAJOR 1
#define REDBUD_MINOR 0
#define REDBUD_PATCH 0
#define REDBUD_VERSION REDBUD_STRING(REDBUD_MAJOR,REDBUD_MINOR,REDBUD_PATCH)

#define _VERSION_CODE (x,y,z) (((x)*100000 +((y)*100)+(z))

#define GUNC_VERSION _VERSION_CODE(__GNUC__,__GNUC_MINOR__,__GNUC_PATCHLEVEL__)


#ifndef _REDBUD_API
    #define _REDBUD_API ::redbud::
#endif

#ifndef REDBUD_HAS_CXX11
  #if defined(REDBUD_MSVC) && (REDBUD_MSVC >= 1900) //msvc编译器
    #define REDBUD_HAS_CXX11 1
  #elif defined(REDBUD_GNUC) && (GNUC_VERSION >= _VERSION_CODE(4,8,0)) && \
  defined(__GXX_EXPERIMENTAL_CXX0X__)//GUNC编译器
    #define REDBUD_HAS_CXX11 1
  #elif defined(REDBUD_CLANG) && (CLANG_VERSION >= _VERSION_CODE(3,3,0))
    #define REDBUD_HAS_CXX11 1 //CLANG编译器
  #else
    #define REDBUD_HAS_CXX11 0
  #endif
#endif // !REDBUD_HAS_CXX11


#if REDBUD_HAS_CXX11 == 0
    #error "C++11 required"
#endif

namespace redbud {
    #if REDBUD_MSVC
        #define NOMINMAX
#endif
}

#endif