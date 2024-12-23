//
// Created by 王少泽 on 2024/12/23.
//

#ifndef MYTINYSTL_MAP_TEST_H_
#define MYTINYSTL_MAP_TEST_H_

// map test : 测试 map, multimap 的接口与它们 insert 的性能

#include <map>

#include "../TinySTL/map.h"
#include "../TinySTL/vector.h"
#include "test.h"

namespace mystl {
    namespace test {
        namespace map_test {
#define PAIR mystl::pair<int,int>

#define MAP_COUT(m) do{\
std::string m_name =#m; \
std::cout<<" "<<m_name<<" :"; \
for(auto it:m)  \
std::cout<<" <"<<it.first<<","<<it.second<<">";  \
std::cout<<std::endl;           \
}while(0)

            //map的函数操作
#define MAP_FUN_AFTER(con,fun) do { \
std::string str = #fun; \
std::cout<<"After"<<str<<" :"<<std::endl;   \
fun;        \
MAP_COUT(con);\
}while(0)

            //map的函数值
#define MAP_VALUE(fun) do{  \
std::string str=#fun;\
auto it=fun; \
std::cout << " " << str << " : <" << it.first << "," << it.second << ">\n"; \
} while(0)
            void map_test() {
                std::cout << "[===============================================================]" << std::endl;
                std::cout << "[------------------ Run container test : map -------------------]" << std::endl;
                std::cout << "[-------------------------- API test ---------------------------]" << std::endl;
                mystl::vector<PAIR> v;
                for(int i=0;i<5;++i)
                    v.push_back(PAIR(i,i));
                mystl::map<int,int> m1;
                mystl::map<int,int,mystl::greater<int>> m2;
                mystl::map<int,int> m3(v.begin(),v.end());
                mystl::map<int,int> m4(v.begin(),v.end());
                mystl::map<int,int> m5(m3);
                mystl::map<int,int> m6(std::move(m3));
                mystl::map<int,int> m7;
                m7=m4;
                mystl::map<int, int> m8;
                m8 = std::move(m4);
                mystl::map<int, int> m9{ PAIR(1,1),PAIR(3,2),PAIR(2,3) };
                mystl::map<int, int> m10;
                m10 = { PAIR(1,1),PAIR(3,2),PAIR(2,3) };

                for(int i=5;i>0;--i) {
                    MAP_FUN_AFTER(m1,m1.emplace(i,i));
                }
                MAP_FUN_AFTER(m1,m1.emplace_hint(m1.begin(),0,0));
            }
        }
    }
}
#endif
