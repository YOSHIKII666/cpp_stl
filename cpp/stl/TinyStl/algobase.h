#ifndef MYTINYSTL_ALGOBASE_H_
#define MYTINYSTL_ALGOBASE_H_

#include<cstring>
#include "iterator.h"
#include "util.h"

namespace mystl{
    template<class T>
    const T& max(const T& lhs,const T& rhs){
        return lhs>rhs?lhs:rhs;
    }

    
}

#endif