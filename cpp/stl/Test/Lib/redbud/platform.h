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



