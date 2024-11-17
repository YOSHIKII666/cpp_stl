#ifndef MYTINYSTL_EXCEPTDEF_H_
#define MYTINYSTL_EXCEPTDEF_H_

#include<stdexcept>
#include<cassert>
namespace mystl{
    //宏定义
    #define MYSTL_DEBUG(expr) assert(expr)//于在程序运行时进行断言检查。其作用是对传入的 expr（表达式）求值，如果表达式的值为真（在 C 和 C++ 中，通常非零值表示真，零值表示假），那么程序继续正常执行；但如果表达式的值为假，assert 宏就会使程序终止执行，并输出一些诊断信息
    #define THROW_LENGTH_ERROR_IF(expr,what) if((expr)) throw std::length_error(what);
    #define THROW_OUT_OF_RANGE_IF(expr,what) if((expr)) throw std::out_of_range(what);
    #define THROW_RUNTIME_ERROR_IF(expr,what) if((expr)) throw std::runtime_error(what);
}

#endif