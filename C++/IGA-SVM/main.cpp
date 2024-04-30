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
#include "bioinfo.h"
#include <time.h>
int main(int argc, const char * argv[]) {
    std::cout << "run IGA/IEA !\n";
    GAlgorithm GA;
    srand( time(NULL) );
// 0. 載入 測試資料及svm
    bioinfo bio("brca_newlasso.csv",5);
    int bit_string_length= 16;
    int feature_count=bio.get_feature_count();
    int rr=feature_count % bit_string_length;
    int num=feature_count / bit_string_length;
    if(rr>0)
        num++;
    num+=2; //svm c, gamma
// gen string c, gamma, v1, v2, v3, ......vn-1, vn
//  v1~vn-1 bit_string_length: 16
//  vn bit_string_length: rr;
//    int index=0;
    int max_value=(1 << bit_string_length) -1;
    fitness_function_data fitnessfunction_data(num, 0, max_value, bit_string_length, 6, true, bio.bioinfo_problem_fitness_function);
    fitnessfunction_data.change_bounded_data(0,0,1, 16);
    fitnessfunction_data.change_bounded_data(1,0,1, 16);

    
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
    GA.init(STOP_CONDITION_GENERATION, 100, 30, 0.1f, 0.5f, 0.008f, &fitnessfunction_data);
    
    
//  3. crossover 機制設定--選擇使用哪種方式----------------------------
//  a. one crossover
//    GA.set_crossover_operator((interface_crossover *)new one_crossover());
//  b. intellignet crossover
    GA.set_crossover_operator((interface_crossover *)new intelligent_crossover());
    
    
    
//  4. 執行------------------------------
    GA.exectue();
    return 0;
}
