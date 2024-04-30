//
//  fitness_testfunction.hpp
//  Bioinfo_IGA
//
//  Created by user on 2023/2/21.
//

#ifndef fitness_testfunction_hpp
#define fitness_testfunction_hpp

#include <stdio.h>
#include "interface_fitness.hpp"
class fitness_testfunction:interface_fitness
{
public:
    fitness_testfunction(int functionIndex);
    ~fitness_testfunction();
public:
    void fitness_function01(individual *_individual);
    void fitness_function02(individual *_individual);
    void fitness_function07(individual *_individual);
    void fitness_function(individual *_individual);
    void (*current_fitness)(individual *_individual)=nullptr;
private:
    int m_functionIndex=0;
};
#endif /* fitness_testfunction_hpp */
