//
//  intelligent_crossover.hpp
//  IGA
//
//  Created by user on 2021/4/25.
//

#ifndef intelligent_crossover_hpp
#define intelligent_crossover_hpp
#include "interface_crossover.hpp"
#include <stdio.h>
#include "tools.h"
class individual;
class interface_crossover;
class intelligent_crossover:interface_crossover
{
public:
    intelligent_crossover();
    ~intelligent_crossover();
    void init(fitness_function_data *data);
    bool crossover(individual *parent1, individual *parent2, individual *best_solution=nullptr);
private:
    void generate_orthogonal_array(int width);
    bool best_fitnessValue_in_OA_proecess(double best_fitnessValue_in_OA, int best_fitnessValue_index);
    bool current_fitnessValue_is_better_than_best_one(double best_fitnessValue_in_OA, double best_fitness_value);
    void recombine_parameters(int factor_index, int *parameter_index, individual *source_individuals, individual *target_individuals, individual *target_individuals_two=nullptr);
    void found_two_best_value(float *fintess_vlaue, int *best_value_index, bool want_bigger=true);
private:
    int tm_width, tm_height;
    char *tm_table;
    short *tm;
    int m_remain;
    individual **m_combined_individuals;
//    bool m_want_bigger_fitness_value;
    individual *candiate_solutions[4];
    individual *better_solutions[2];
    fitness_function_data *m_fitness_function_data;
};
#endif /* intelligent_crossover_hpp */
