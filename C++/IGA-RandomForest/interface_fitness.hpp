//
//  iterface_fitness.hpp
//  Bioinfo_IGA
//
//  Created by user on 2023/2/21.
//

#ifndef iterface_fitness_hpp
#define iterface_fitness_hpp

#include <stdio.h>
class individual;
class interface_fitness
{
public:
    virtual void fitness_function(individual *_individual)=0;
    
    
};
#endif /* iterface_fitness_hpp */
