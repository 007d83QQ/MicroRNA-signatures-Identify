//
//  ga_info_statistics.hpp
//  IGA
//
//  Created by user on 2023/1/31.
//

#ifndef ga_info_statistics_hpp
#define ga_info_statistics_hpp

#include <stdio.h>
#include <vector>
#include "fitness_value.hpp"
//typedef struct fitness_value;

struct generation_info{
//    int generation;
    int cost;
 //   double fitness_value;
    fitness_value m_evaluation;
public:
    generation_info(int _cost, fitness_value _value)
    {
        cost=_cost;
        m_evaluation=_value;
    }
};
class ga_info_statistics
{
public:
    ga_info_statistics(int num=1);
    ~ga_info_statistics();
public:
    void set_num_multi_objectives(int num);
    void add(int generation, int cost, fitness_value _fitness);
    void setup_dump_fun(void (*_dump_fun)());
private:
    static void dump();
    
public:
    void (*dump_fun)()=nullptr;
private:
    std::vector<generation_info *> ga_info;
    int num_multi_objectives;
};
#endif /* ga_info_statistics_hpp */
