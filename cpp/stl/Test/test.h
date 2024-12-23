//
// Created by 王少泽 on 2024/12/22.
//
#ifndef MYTINYSTL_TEST_H_
#define MYTINYSTL_TEST_H_

// 一个简单的单元测试框架，定义了两个类 TestCase 和 UnitTest，以及一系列用于测试的宏

#include <ctime>
#include <cstring>
#include <cstdio>
#include <iostream>
#include <iomanip>
#include <string>
#include <sstream>
#include <vector>

#include "Lib/redbud/io/color.h"

namespace mystl {
    namespace test {
#define green redbud::io::state::manual<<redbud::io::hfg::green
#define red redbud::io::state::manual<<redbud::io::hfg::red
    }
    class  TestCase {
    public:
        // 接受一个字符串代表案例名称
        TestCase(const char* case_name):testcase_name(case_name){}

        //一个虚函数，用于testCase
        virtual void Run()=0;
    public:
        const char* testcase_name;//测试案例的名称
        int nTestResult;//执行结果
        double nFailed;//失败的案例数
        double nPassed;//通过的案例数
    };

    class  UnitTest {
        //获取一个案例
        static UnitTest* GetInstance();
        //将案例依次加入vector
        TestCase* RegisterTestCase(TestCase* testcase);

        void Run();

    public:
        TestCase* CurrentTestCase; //当前执行的测试案例
        double nPassed;//通过的案例
        double nFailed;//失败的案例
    protected:
        std::vector<TestCase*> testcases_;
    };

    UnitTest* UnitTest::GetInstance() {
        static UnitTest instance;
        return &instance;
    }

    TestCase* UnitTest::RegisterTestCase(TestCase *testcase) {
        testcases_.push_back(testcase);
        return testcase;
    }
    void UnitTest::Run() {
        for(auto it : testcases_) {
            TestCase* testcase=it;
            CurrentTestCase=testcase;
            testcase->nTestResult=1;
            testcase->nFailed=0;
            testcase->nPassed=0;
            std::cout<<green<<"======================\n";
            std::cout<<green<<"Run TestCase:"<<testcase->testcase_name<<"\n";
            testcase->Run();
            if(testcase->nFailed==0) {
                std::cout<<green;
            }else {
                std::cout<<red;
            }
            std::cout<<" "<<testcase->nPassed<<" / "<<testcase->nFailed+testcase->nPassed
            <<"Cases passed.("<<testcase->nPassed/(testcase->nPassed+testcase->nFailed)*100<<"%)\n";
            if(testcase->nTestResult) ++nPassed;
            else ++nFailed;
        }
        std::cout<<green<<"=======================\n";
        std::cout<<green<<"Total TestCase : "<< nPassed+nFailed << "\n";
        std::cout<<green<<"Total Passed : "<<nPassed<<"\n";
        std::cout<<red<<"Total Failed : "<<nFailed<<"\n";
        std::cout<<green<<" "<<nPassed<<" / "<< nFailed+nPassed<<"TestCases passed.("<<nPassed/(nFailed+nPassed)*100<<" % )\n";
    }

    //测试案例的名称，替换为 test_name_TEST,通过定义一个宏来实现
#define TESTCASE_NAME(testcase_name) testcase_name##_TEST

    //封装过程
#define MYTINYSTL_TEST_(testcase_name)                        \
    class TESTCASE_NAME(testcase_name) : public TestCase {        \
        public:                                                       \
        TESTCASE_NAME(testcase_name)(const char* case_name)       \
        : TestCase(case_name) {};                             \
        virtual void Run();                                       \
        private:                                                      \
        static TestCase* const testcase_;                         \
    };                                                            \
    \
    TestCase* const TESTCASE_NAME(testcase_name)                  \
    ::testcase_ = UnitTest::GetInstance()->RegisterTestCase(  \
    new TESTCASE_NAME(testcase_name)(#testcase_name));    \
    void TESTCASE_NAME(testcase_name)::Run()

//EXCEPT为true,验证为true时的代码
#define EXPECT_TRUE(Condition) do{              \
        if(Condition){                           \
            UnitTest::GetInstance()->CurrentTestCase->nPassed++;        \
            std::cout<<green<<"EXPECT_TRUE succeeded! \n";                  \
        }\
        else{                       \
            UnitTest::GetInstance()->CurrentTestCase->nTestResult=0;        \
            UnitTest::GetInstance()->CurremtTestCase->nFailed++;            \
            std::cout<< red << " EXPECT_TRUE failed!\n";                \
        }                   \
    }}while (0)

#define EXPECT_FALSE(Condition) do{             \
    if(!Condition){                             \
            UnitTest::GetInstance()->CurrentTestCase->nPassed++;  \
            std::cout<<green<< "EXPECT_FALSE successded! \n";   \
           }\
       else{    \
    UnitTest::GetInstance()->CurrentTestCase->nTestResult = 0;  \
    UnitTest::GetInstance()->CurrentTestCase->nFailed++;        \
    std::cout << red << "  EXPECT_FALSE failed!\n";             \
    }} while(0)

    //EXPECT(EXPECT,ACTUAL)
#define EXPECT_EQ(v1,v2) do{ \
    if(v1==v2){     \
        UnitTest::GetInstance()->CurrentTestCase->nPassed++;    \
        std::cout<<green<<"EXPECT_EQ succeeded!\n";         \
    }\
    else{                       \
        UnitTest::GetInstance()->CurrentTestCase->nTestResult=0;\
        UnitTest::GetInstance()->CurrentTestCase->nFailed;      \
        std::cout<<red<<"EXPECT_EQ faild!\n"        \
        std::cout<<red<<"Expect:"<<v1<<"\n"     \
        std::cout<<red<<"Actual:"<<v2<<"\n"     \
    }}while(0)

    #define EXPECT_NE(v1, v2) do {                                  \
  if (v1 != v2) {                                               \
    UnitTest::GetInstance()->CurrentTestCase->nPassed++;        \
   std::cout << green << " EXPECT_NE succeeded!\n";             \
  }                                                             \
  else {                                                        \
    UnitTest::GetInstance()->CurrentTestCase->nTestResult = 0;  \
    UnitTest::GetInstance()->CurrentTestCase->nFailed++;        \
    std::cout << red << " EXPECT_NE failed!\n";                 \
    std::cout << red << " Expect:" << v1 << "\n";               \
    std::cout << red << " Actual:" << v2 << "\n";               \
}} while(0)

#define EXPECT_LT(v1, v2) do {                                  \
  if (v1 < v2) {                                                \
    UnitTest::GetInstance()->CurrentTestCase->nPassed++;        \
    std::cout << green << " EXPECT_LT succeeded!\n";            \
  }                                                             \
  else {                                                        \
    UnitTest::GetInstance()->CurrentTestCase->nTestResult = 0;  \
    UnitTest::GetInstance()->CurrentTestCase->nFailed++;        \
    std::cout << red << " EXPECT_LT failed!\n";                 \
    std::cout << red << " Expect:" << v1 << "\n";               \
    std::cout << red << " Actual:" << v2 << "\n";               \
}} while(0)

#define EXPECT_LE(v1, v2) do {                                  \
  if (v1 <= v2) {                                               \
    UnitTest::GetInstance()->CurrentTestCase->nPassed++;        \
    std::cout << green << " EXPECT_LE succeeded!\n";            \
  }                                                             \
  else {                                                        \
    UnitTest::GetInstance()->CurrentTestCase->nTestResult = 0;  \
    UnitTest::GetInstance()->CurrentTestCase->nFailed++;        \
    std::cout << red << " EXPECT_LE failed!\n";                 \
    std::cout << red << " Expect:" << v1 << "\n";               \
    std::cout << red << " Actual:" << v2 << "\n";               \
}} while(0)

#define EXPECT_GT(v1, v2) do {                                  \
  if (v1 > v2) {                                                \
    UnitTest::GetInstance()->CurrentTestCase->nPassed++;        \
    std::cout << green << " EXPECT_GT succeeded!\n";            \
  }                                                             \
  else {                                                        \
    UnitTest::GetInstance()->CurrentTestCase->nTestResult = 0;  \
    UnitTest::GetInstance()->CurrentTestCase->nFailed++;        \
    std::cout << red << " EXPECT_GT failed!\n";                 \
    std::cout << red << " Expect:" << v1 << "\n";               \
    std::cout << red << " Actual:" << v2 << "\n";               \
}} while(0)

#define EXPECT_GE(v1, v2) do {                                  \
  if (v1 >= v2) {                                               \
    UnitTest::GetInstance()->CurrentTestCase->nPassed++;        \
    std::cout << green << " EXPECT_GE succeeded!\n";            \
  }                                                             \
  else {                                                        \
    UnitTest::GetInstance()->CurrentTestCase->nTestResult = 0;  \
    UnitTest::GetInstance()->CurrentTestCase->nFailed++;        \
    std::cout << red << " EXPECT_GE failed!\n";                 \
    std::cout << red << " Expect:" << v1 << "\n";               \
    std::cout << red << " Actual:" << v2 << "\n";               \
}} while(0)

//字符串部分
#define EXCEPT_STREQ(s1,s2) do{                 \
    if(s1 ==NULL || s2== NULL){                 \
        if(s1==NULL && s2== NULL){              \
                UniTest::GetInstance()->CurrentTestCase->nPassed++;     \
                std::cout<<green<<"EXPECT_STORD succeeded!\n"       \
               }                    \
        else{                                   \
                UnitTest::GetInstance()->CurrentTestCase->nTestResult=0;        \
                UnitTest::GetInstance()->CurrentTestCase->nFailed++;        \
                std::cout<<red<<"EXPECT_STRED faild!\n";            \
                if(s1 == NULL)  std::cout<< "Expect:NULL\n";     \
                else std::cout << " Expect:\"" << s1 << "\"\n";             \
                if(s2 == NULL) std::cout << " Actual: NULL\n";              \
                else std::cout << " Actual:\"" << s2 << "\"\n";             \
                }                                                             \
              }                                                               \
        else if(strcmp(s1,s2)==0){                  \
                UnitTest::GetInstance()->CurrentTestCase->nPassed++;        \
                std::cout<<green<<"EXPECT_STRED succeeded!  \n"             \
                }                               \
                else{                            \
                    UnitTest::GetInstance()->CurrentTestCase->nTestResult = 0;    \
                    UnitTest::GetInstance()->CurrentTestCase->nFailed++;          \
                    std::cout << red << " EXPECT_STRED failed!\n";                \
                    std::cout << red << " Expect:\"" << s1 << "\"\n";             \
                    std::cout << red << " Actual:\"" << s2 << "\"\n";             \
                    }} while(0)


#define EXPECT_STRNE(s1, s2) do {                                 \
            if (s1 == NULL || s2 == NULL) {                                 \
            if (s1 != NULL || s2 != NULL) {                               \
            UnitTest::GetInstance()->CurrentTestCase->nPassed++;        \
            std::cout << green << " EXPECT_STRNE succeeded!\n";         \
            }                                                             \
            else {                                                        \
            UnitTest::GetInstance()->CurrentTestCase->nTestResult = 0;  \
            UnitTest::GetInstance()->CurrentTestCase->nFailed++;        \
            std::cout << red << " EXPECT_STRNE failed!\n";              \
            if(s1 == NULL) std::cout << " Expect: NULL\n";              \
            else std::cout << " Expect:\"" << s1 << "\"\n";             \
            if(s2 == NULL) std::cout << " Actual: NULL\n";              \
            else std::cout << " Actual:\"" << s2 << "\"\n";             \
            }                                                             \
            }                                                               \
            else if (strcmp(s1, s2) != 0) {                                 \
            UnitTest::GetInstance()->CurrentTestCase->nPassed++;          \
            std::cout << green << " EXPECT_STRNE succeeded!\n";           \
            }                                                               \
            else {                                                          \
            UnitTest::GetInstance()->CurrentTestCase->nTestResult = 0;    \
            UnitTest::GetInstance()->CurrentTestCase->nFailed++;          \
            std::cout << red << " EXPECT_STRNE failed!\n";                \
            std::cout << red << " Expect:\"" << s1 << "\"\n";             \
            std::cout << red << " Actual:\"" << s2 << "\"\n";             \
            }} while(0)

    //指针比较
#define EXPECT_PTR_EQ(p1,p2) do {           \
    if(*p1==*p2){           \
        UnitTest::GetInstance()->CurrentTestCase->nPassed++;        \
        std::cout<<green<<"EXPECT_PTR_EQ succeeded!\n"; \
        }               \
    else{                       \
            UnitTest::GetInstance()->CurrentTestCase->nTestResult=0;        \
            UnitTest::GetInstance()->CurrentTestCase->nFailed++;        \
            std::cout << red << " EXPECT_PTR_EQ failed!\n";             \
            std::cout << red << " Expect:" << *p1 << "\n";              \
            std::cout << red << " Actual:" << *p2 << "\n";              \
            }} while(0)

#define EXPECT_PTR_NE(p1, p2) do {                              \
    if (*p1 != *p2) {                                             \
    UnitTest::GetInstance()->CurrentTestCase->nPassed++;        \
    std::cout << green << " EXPECT_PTR_NE succeeded!\n";        \
    }                                                             \
    else {                                                        \
    UnitTest::GetInstance()->CurrentTestCase->nTestResult = 0;  \
    UnitTest::GetInstance()->CurrentTestCase->nFailed++;        \
    std::cout << red << " EXPECT_PTR_NE failed!\n";             \
    std::cout << red << " Expect:" << *p1 << "\n";              \
    std::cout << red << " Actual:" << *p2 << "\n";              \
    }} while(0)

    //任意 i (*p1 + i) == (*p2 + i)  i∈[0,len)
#define EXPECT_PTR_RANGE_EQ(p1,p2,len) do{      \
        if(std::equal(p1,p1+len,p2))            {       \
            UnitTest::GetInstance()->CurrentTestCase->nPassed++;    \
            std::cout<<green<<"EXPECT_PTR_RANGE_EQ succeeded!\n";           \
            }           \
        else{                   \
            UnitTest::GetInstance()->CurrentTestCase->nTestResult=0;        \
            UnitTest::GetInstance()->CurrentTestCase->nFailed++;            \
            std::cout<<red<<"EXPECT_PTR_RANGE_EQ!\n"        \
            }}while(0)

#define EXPECT_PTR_RANGE_EQ(p1,p2,len) do{      \
        if(!std::equal(p1,p1+len,p2))            {       \
        UnitTest::GetInstance()->CurrentTestCase->nPassed++;    \
        std::cout<<green<<"EXPECT_PTR_RANGE_EQ succeeded!\n";           \
        }           \
        else{                   \
        UnitTest::GetInstance()->CurrentTestCase->nTestResult=0;        \
        UnitTest::GetInstance()->CurrentTestCase->nFailed++;            \
        std::cout<<red<<"EXPECT_PTR_RANGE_EQ!\n"        \
        }}while(0)

        //容器比较
#define EXPECT_CON_EQ(c1,c2)    do{     \
        auto first1=std::begin(c1),last1=std::end(c1);      \
        auto first2=std::begin(c2),last2=std::end(c2);      \
        for(;first1!=last1 && first2!=last2;++first1,++first2){     \
            if(*first1!=*first2) break; \
            }       \
        if(first1==last1 && first2==last2){             \
            UnitTest::GetInstance()->CurrentTestCase->nPassed++;    \
            std::cout<<green<<"EXPECT_CON_EQ succeeded!\n";      \
            }           \
            else{           \
                UnitTest::GetInstance()->CurrentTestCase->nTestResult=0;        \
                UnitTest::GetInstance()->CurrentTestCase->nFailed++;            \
                std::cout << red << " EXPECT_CON_EQ failed!\n";                 \
                std::cout << red << " Expect:" << *first1 << "\n";              \
                std::cout << red << " Actual:" << *first2 << "\n";              \
            }} while(0)

#define EXPECT_CON_EQ(c1,c2)    do{     \
        auto first1=std::begin(c1),last1=std::end(c1);      \
        auto first2=std::begin(c2),last2=std::end(c2);      \
        for(;first1!=last1 && first2!=last2;++first1,++first2){     \
        if(*first1!=*first2) break; \
        }       \
        if(first1!=last1 || first2!=last2){             \
        UnitTest::GetInstance()->CurrentTestCase->nPassed++;    \
        std::cout<<green<<"EXPECT_CON_EQ succeeded!\n";      \
        }           \
        else{           \
        UnitTest::GetInstance()->CurrentTestCase->nTestResult=0;        \
        UnitTest::GetInstance()->CurrentTestCase->nFailed++;            \
        std::cout << red << " EXPECT_CON_EQ failed!\n";                 \
        std::cout << red << " Expect:" << *first1 << "\n";              \
        std::cout << red << " Actual:" << *first2 << "\n";              \
        }} while(0)

    //常用的宏定义
    // 不同情况的测试数量级
#if defined(_DEBUG) || defined(DEBUG)
#define LEN1    10000
#define LEN2    100000
#define LEN3    1000000
#else
#define LEN1    100000
#define LEN2    1000000
#define LEN3    10000000
#endif

#define SCALE_LLL(N) (N * 20)
#define SCALE_LL(N)  (N * 10)
#define SCALE_L(N)   (N * 5)
#define SCALE_M(N)   (N)
#define SCALE_S(N)   (N / 5)
#define SCALE_SS(N)  (N / 10)
#define SCALE_SSS(N) (N / 20)

#define WIDE    14
#define PASSED std::cout<<"[ PASSED ]\n"

#define COUT(container) do {                             \
std::string con_name = #container;                     \
std::cout << " " << con_name << " :";                  \
for (auto it : container)                              \
std::cout << " " << it;                              \
std::cout << "\n";                                     \
} while(0)

#define STR_COUT(str) do {                               \
std::string str_name = #str;                           \
std::cout << " " << str_name << " : " << str << "\n";  \
} while(0)

    // 输出容器调用函数后的结果
#define FUN_AFTER(con, fun) do {                         \
std::string fun_name = #fun;                           \
std::cout << " After " << fun_name << " :\n";          \
fun;                                                   \
COUT(con);                                             \
} while(0)

#define STR_FUN_AFTER(str, fun) do {                     \
std::string fun_name = #fun;                           \
std::cout << " After " << fun_name << " :\n";          \
fun;                                                   \
STR_COUT(str);                                         \
} while(0)

    // 输出容器调用函数的值
#define FUN_VALUE(fun) do {                              \
std::string fun_name = #fun;                           \
std::cout << " " << fun_name << " : " << fun << "\n";  \
} while(0)

    // 输出测试数量级
    void test_len(size_t len1, size_t len2, size_t len3, size_t wide)
    {
        std::string str1, str2, str3;
        std::stringstream ss;
        ss << len1 << " " << len2 << " " << len3;
        ss >> str1 >> str2 >> str3;
        str1 += "   |";
        std::cout << std::setw(wide) << str1;
        str2 += "   |";
        std::cout << std::setw(wide) << str2;
        str3 += "   |";
        std::cout << std::setw(wide) << str3 << "\n";
    }

#define TEST_LEN(len1, len2, len3, wide) \
test_len(len1, len2, len3, wide)

    //性能测试的宏
#define FUN_TEST_FORMAT1(mode,fun,arg,count) do{        \
       srand((int)time(0));         \
       clock_t start,end;           \
       mode c;                      \
       char buf[10];            \
       start=clock();           \
       for(size_t i=0;i<count;++i)  \
            c.fun(arg);         \
       end=clock();         \
       int n=static_cast<int>(static_cast<double>(end-start)/CLOCKS_PER_SEC*1000);   \
        std::snprintf(buf,sizeof(buf),"%d",n);    \
        std::string t = buf;     \
        t += "ms   |";              \
        std::cout<<std::setw(WIDE)<<t;          \
       }while(0)

#define FUN_TEST_FORMAT2(mode,fun,arg1,arg2,count) do{          \
        srand(int)time(0);                          \
        clock_t start,end;                          \
        mode c;                                     \
        char buf[10];                               \
        start=clock()                               \
        for(size_t i=0;i<count;++i)                 \
            c.fun(c.arg1(),arg2);                   \
        end=clock();                                \
        int n=static_cast<int>(static_cast<double>(end-start)/CLOCKS_PER_SEC*1000); \
        std::snprintf(buf,sizeof(buf),"%d",n);      \
        std::string t=buf;                          \
        t+="ms  |";                                 \
        std::cout<<std::setw(WIDE)<<t;              \
        }while (0)

#define LIST_SORT_DO_TEST(mode, count) do {                  \
        srand((int)time(0));                                       \
        clock_t start, end;                                        \
        mode::list<int> l;                                         \
        char buf[10];                                              \
        for (size_t i = 0; i < count; ++i)                         \
            l.insert(l.end(), rand());                               \
        start = clock();                                           \
        l.sort();                                                  \
        end = clock();                                             \
        int n = static_cast<int>(static_cast<double>(end - start)  \
        / CLOCKS_PER_SEC * 1000);                              \
        std::snprintf(buf, sizeof(buf), "%d", n);                  \
        std::string t = buf;                                       \
        t += "ms    |";                                            \
        std::cout << std::setw(WIDE) << t;                         \
        } while(0)

#define MAP_EMPLACE_DO_TEST(mode,con,count) do{                 \
        srand((int)time(0));                    \
        clock_t start,end;                      \
        mode::con<int,int> c;                      \
        start = clock();                    \
        char buf[10];                       \
        for(size_t i=0;i<count;++i)         \
            c.emplace(mode::make_pair(rand(),rand());           \
        end=clock();            \
        int n = static_cast<int>(static_cast<double>(end - start)  \
       / CLOCKS_PER_SEC * 1000);                              \
        std::snprintf(buf, sizeof(buf), "%d", n);                  \
        std::string t = buf;                                       \
        t += "ms    |";                                            \
        std::cout << std::setw(WIDE) << t;                         \
    } while(0)
#define CON_TEST_P1(con, fun, arg, len1, len2, len3)         \
  TEST_LEN(len1, len2, len3, WIDE);                          \
  std::cout << "|         std         |";                    \
  FUN_TEST_FORMAT1(std::con, fun, arg, len1);                \
  FUN_TEST_FORMAT1(std::con, fun, arg, len2);                \
  FUN_TEST_FORMAT1(std::con, fun, arg, len3);                \
  std::cout << "\n|        mystl        |";                  \
  FUN_TEST_FORMAT1(mystl::con, fun, arg, len1);              \
  FUN_TEST_FORMAT1(mystl::con, fun, arg, len2);              \
  FUN_TEST_FORMAT1(mystl::con, fun, arg, len3);

#define CON_TEST_P2(con, fun, arg1, arg2, len1, len2, len3)  \
  TEST_LEN(len1, len2, len3, WIDE);                          \
  std::cout << "|         std         |";                    \
  FUN_TEST_FORMAT2(std::con, fun, arg1, arg2, len1);         \
  FUN_TEST_FORMAT2(std::con, fun, arg1, arg2, len2);         \
  FUN_TEST_FORMAT2(std::con, fun, arg1, arg2, len3);         \
  std::cout << "\n|        mystl        |";                  \
  FUN_TEST_FORMAT2(mystl::con, fun, arg1, arg2, len1);       \
  FUN_TEST_FORMAT2(mystl::con, fun, arg1, arg2, len2);       \
  FUN_TEST_FORMAT2(mystl::con, fun, arg1, arg2, len3);

#define MAP_EMPLACE_TEST(con, len1, len2, len3)              \
  TEST_LEN(len1, len2, len3, WIDE);                          \
  std::cout << "|         std         |";                    \
  MAP_EMPLACE_DO_TEST(std, con, len1);                       \
  MAP_EMPLACE_DO_TEST(std, con, len2);                       \
  MAP_EMPLACE_DO_TEST(std, con, len3);                       \
  std::cout << "\n|        mystl        |";                  \
  MAP_EMPLACE_DO_TEST(mystl, con, len1);                     \
  MAP_EMPLACE_DO_TEST(mystl, con, len2);                     \
  MAP_EMPLACE_DO_TEST(mystl, con, len3);

#define LIST_SORT_TEST(len1, len2, len3)                     \
  TEST_LEN(len1, len2, len3, WIDE);                          \
  std::cout << "|         std         |";                    \
  LIST_SORT_DO_TEST(std, len1);                              \
  LIST_SORT_DO_TEST(std, len2);                              \
  LIST_SORT_DO_TEST(std, len3);                              \
  std::cout << "\n|        mystl        |";                  \
  LIST_SORT_DO_TEST(mystl, len1);                            \
  LIST_SORT_DO_TEST(mystl, len2);                            \
  LIST_SORT_DO_TEST(mystl, len3);

// 简单测试的宏定义
#define TEST(testcase_name) \
  MYTINYSTL_TEST_(testcase_name)

// 运行所有测试案例
#define RUN_ALL_TESTS() \
  mystl::test::UnitTest::GetInstance()->Run()

// 是否开启性能测试
#ifndef PERFORMANCE_TEST_ON
#define PERFORMANCE_TEST_ON 1
#endif // !PERFORMANCE_TEST_ON

// 是否开启大数据量测试
#ifndef LARGER_TEST_DATA_ON
#define LARGER_TEST_DATA_ON 0
#endif // !LARGER_TEST_DATA_ON

}    // namespace test
#endif // !MYTINYSTL_TEST_H_
