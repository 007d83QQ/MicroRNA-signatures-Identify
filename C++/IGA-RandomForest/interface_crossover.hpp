//
//  interface_crossover.hpp
//  IGA
//
//  Created by user on 2021/4/25.
//

#ifndef interface_crossover_hpp
#define interface_crossover_hpp

#include <stdio.h>
#include <stdlib.h>
#include "tools.h"

class individual;
class fitness_function_data;
class interface_crossover
{
public:
    virtual void init(fitness_function_data *data);
    virtual bool crossover(individual *parent1, individual *parent2, individual *best_solution=nullptr)=0;
protected:
    void set_child_count(int size);
    void create_child_data(int size);
    void clear_child_data();
protected:
    individual **m_child;
    int m_child_count;
    int m_created_child_count;
    fitness_function_data *m_fitness_function_data;
};
#endif /* interface_crossover_hpp */
