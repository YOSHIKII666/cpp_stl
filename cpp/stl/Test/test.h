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


}



#endif
