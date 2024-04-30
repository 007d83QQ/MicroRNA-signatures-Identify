//
//  individual.hpp
//  IGA
//
//  Created by user on 2021/4/24.
//

#ifndef individual_hpp
#define individual_hpp
#define MAX_VALUE 4096
#define MAX_FEATURE_VALUE 2000000
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "tools.h"
#include "fitness_value.hpp"
class fitness_function_data;

class individual
{
public:
    individual(int index, fitness_function_data *data);
    individual(fitness_function_data *data);
    ~individual();
    void setup(int index, fitness_function_data *data);
public:
    void init();
    void mute(int parameters_index, int mask_value);
    void clone_whole_data_to(individual *other);
    void copy_parameters(individual *target, int start_postion, int len);
    float get_min_value(int index=0);
    float get_interval_value(int index=0) ;
    void dump();
    fitness_function_data *get_fitness_function_data();
    double get_parameters_value(int index);
public:
    bool m_is_change;
    int *m_parameters;
    fitness_value m_evulation_value;
//    double m_fintess_value;
//    double m_multi_objs[MAX_OBJECTS];
    int m_individual_index_in_population=-1;
private:
    fitness_function_data *m_fitness_function_data;
    
};
#endif /* individual_hpp */
