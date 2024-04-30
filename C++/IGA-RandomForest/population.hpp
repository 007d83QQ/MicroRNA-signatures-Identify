//
//  population.hpp
//  IGA
//
//  Created by user on 2021/4/24.
//

#ifndef population_hpp
#define population_hpp

#include <stdio.h>
#include "tools.h"
#include "fitness_value.hpp"
class individual;
class interface_crossover;
class fitness_function_data;
class population
{
public:
    population(int individuals_count, fitness_function_data *_fitness_function_data);
    ~population();
public:
    void set_individuals(int individual_count, int parameter_count);
    void init();
    individual *get_individual(int index);
    int get_individual_index_with_best_fitness();
    void sort();
    int get_best_individual_index();
    void best_individual_repalce_worst_ones( int count);
    bool has_fitness_function();
    void evalute();
    void set_crossover_operator(interface_crossover *_crossover);
private:
    fitness_function_data *m_fitness_function_data;
public:
    virtual void crossover(int first, int second);
    virtual void dump();
    fitness_value get_best_fitness_value();
    void mutate(int count);
    void evalute_best_individual();
private:
    virtual void mutate();
public:
    interface_crossover *m_crossover_operator;
    individual** m_individuals;
    int m_individuals_count;
    int *m_sorted_index;
    int m_best_value_location;
    int m_best_solution_index;
    int m_child_count;
    float m_upper_bound, m_lower_bound;
};
#endif /* population_hpp */
