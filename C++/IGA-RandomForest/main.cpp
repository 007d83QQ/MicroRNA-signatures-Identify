//
//  main.cpp
//  IGA
//
//  Created by user on 2021/4/23.
//
//---------------------------------------
//  GA 使用到
#include "GAlgorithm.hpp"
#include "intelligent_crossover.hpp"
#include "fitness_function_data.hpp"
//---------------------------------------

#include <iostream>
#include "fitness_svm.h"
#include "fitness_RandomForest.hpp"
#include "fitness_testfunction.hpp"
#include <time.h>
//---------------------------------------------------------------------------------
fitness_function_data *init_RandomForest()
{
    fitness_RandomForest *rf;
    fitness_function_data *ff_define=nullptr;
    rf=new fitness_RandomForest();
    rf->init("iga_kidney.csv", 5);
    int bit_string_length= 16;
    int feature_count=rf->get_feature_count();
    int rr=feature_count % bit_string_length;
    int num=feature_count / bit_string_length;
    if(rr>0)
        num++;
    int max_value=(1 << bit_string_length) -1;
    if(feature_count==0)
    {
        printf("data error, can not run IGA \n");
        return nullptr;
    }
    num++;
    ff_define=new  fitness_function_data(num, 0, max_value, bit_string_length, 2, true, (interface_fitness *)rf);
    ff_define->change_bounded_data(0,10,150,16);
    return ff_define;
}
//---------------------------------------------------------------------------------
fitness_function_data *init_svm()
{
    fitness_svm *svm_object=new fitness_svm("iga_kidney.csv",5);
    fitness_function_data *ff_define=nullptr;
    int bit_string_length= 16;
    int feature_count=svm_object->get_feature_count();
    int rr=feature_count % bit_string_length;
    int num=feature_count / bit_string_length;
    if(feature_count==0)
    {
        printf("data error, can not run IGA \n");
        return nullptr;
    }
    if(rr>0)
        num++;
    num+=2; //svm c, gamma
// gen string c, gamma, v1, v2, v3, ......vn-1, vn
//  v1~vn-1 bit_string_length: 16
//  vn bit_string_length: rr;
    int max_value=(1 << bit_string_length) -1;
    ff_define=new fitness_function_data(num, 0, max_value, bit_string_length, 6, true, (interface_fitness *)svm_object);
    ff_define->change_bounded_data(0,0,5, 16); // define C
    ff_define->change_bounded_data(1,0,1, 16);   //define gamma
    return ff_define;
}
//---------------------------------------------------------------------------------
fitness_function_data *init_testfunctions()
{
    fitness_testfunction *testfunction_object=new fitness_testfunction(7);
    fitness_function_data *ff_define=nullptr;
    int bit_string_length= 16;
    int feature_count=100;
    int num=feature_count;
    if(feature_count==0)
    {
        printf("data error, can not run IGA \n");
        return nullptr;
    }
    ff_define=new  fitness_function_data(num, 3, 13, bit_string_length, 1, true, (interface_fitness *)testfunction_object);

    return ff_define;
}
//---------------------------------------------------------------------------------
int main(int argc, const char * argv[]) {
    std::cout << "run IGA/IEA !\n";
    GAlgorithm GA;
// 0. 載入 測試資料及svm
    //------------------------------------------------------------------------
 
    //random forest
    fitness_function_data *fitnessfunction_define=init_RandomForest();
    //svm
//    fitness_function_data *fitnessfunction_define=init_svm();
//    fitness_function_data *fitnessfunction_define=init_testfunctions();
    if(fitnessfunction_define==nullptr)
    {
        exit(0);
    }
    
//  1. 測試函數參數設定-----------------------------
// fitnessfunction_data.init 參數, 依順序意義為: 參數數量, 最小值, 最大值, 編碼長度, 找最大值(布林值, false->找最小), 測試函數(定義在fitness_function.hpp 裏面)
//    fitnessfunction_data.init(10, -0.1f, 0.9f, 10, false, fitness_function01);
//    fitnessfunction_data.init(10, 3.0f, 13.0f, 10, true, fitness_function07);
    //
//    fitness_function_data fitnessfunction_data(100, 3.0f, 13.0f, 10, true, bio.bioinfo_problem_fitness_function);
    
//  2. 基因參數測定-----------------------------
//  演化停止模式, 模式參數, 族群數量, 選擇率, 交配率, 突變率, 函數資料
//  演化停止模式(STOP_MODE):
//          STOP_MODE_GENERATION(代數), 模式參數: 表示代數資料
//          STOP_MODE_EVALUATION_COUNT(評估次數), 模式參數: 表示評估次數資料
//    GA.init(STOP_CONDITION_EVALUATION_COUNT, 2000, 20, 0.1f, 0.5f, 0.008f, fitnessfunction_data);
    GA.init(STOP_CONDITION_GENERATION, 100, 30, 0.1f, 0.5f, 0.008f, fitnessfunction_define);
    
    
//  3. crossover 機制設定--選擇使用哪種方式----------------------------
//  a. one crossover
//    GA.set_crossover_operator((interface_crossover *)new one_crossover());
//  b. intellignet crossover
    GA.set_crossover_operator((interface_crossover *)new intelligent_crossover());
    
    
    
//  4. 執行------------------------------
    GA.exectue();
    return 0;
}
