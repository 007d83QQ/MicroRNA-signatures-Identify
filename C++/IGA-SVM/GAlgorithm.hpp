//
//  GAlgorithm.hpp
//  IGA
//
//  Created by user on 2021/4/23.
//

#ifndef GAlgorithm_hpp
#define GAlgorithm_hpp

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "tools.h"
#include "ga_info_statistics.hpp"
class population;
class individual;
class interface_crossover;
class fitness_function_data;
//------------------------------------------------------------------------------
class crossover_pairs
{
public:
    int m_first_index;
    int m_second_index;
};
class GAlgorithm
{
public:
    GAlgorithm();
    ~GAlgorithm();
public:
    virtual void select();
    virtual void mutate();
    virtual void dump();
public:
    void init(STOP_CONDITION mode,int stop_condition_parameter, int poputlation_size, float selection_rate, float crossover_rate, float mutation_rate, int parameter_count, int encode_bits_count, void (*fitness_fun)(individual *_individual), float lower_bound, float upper_bound, bool want_bigger_fitness_value);
    void init(STOP_CONDITION mode,int stop_condition_parameter, int poputlation_size, float selection_rate, float crossover_rate, float mutation_rate, fitness_function_data *data);
    void set_crossover_operator(interface_crossover *_crossover);
    void population_init();
    void evalute();
    void crossover();
    void exectue();
protected:
    population *m_population;
    float m_selection_rate;
    float m_crossover_rate;
    float m_mutation_rate;
    int m_population_size;
    int m_iterations_count;
    int m_mutations_count;
    bool m_want_bigger_fitness_value;
    int m_best_value_location;
    int m_best_individual_index;
    int m_replace_count;
    int m_corssover_times;
    int m_max_evalution_count;
    int m_evalution_count;

    crossover_pairs **m_pair_data;
    interface_crossover *m_crossover;
    fitness_function_data *m_fitness_function_data;
    STOP_CONDITION stop_mode;
    ga_info_statistics ga_statistics;
};
#endif /* GAlgorithm_hpp */
