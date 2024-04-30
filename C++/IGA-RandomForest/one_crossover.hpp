//
//  crossover_step.hpp
//  IGA
//
//  Created by user on 2021/4/24.
//

#ifndef crossover_step_hpp
#define crossover_step_hpp

#include <stdio.h>
#include "interface_crossover.hpp"

class individual;
class one_crossover:interface_crossover
{
public:
    one_crossover(int child_count=2);//int child_count, int parameters_count, void (*fitness_fun)(individual *_individual));
    ~one_crossover();
    bool crossover(individual *parent1, individual *parent2, individual *best_solution=nullptr);
private:

    void get_best_two_replace_parent(individual *parent1, individual *parent2, individual *child1, individual *child2);
};
#endif /* crossover_step_hpp */
