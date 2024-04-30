//
//  fitness_value.hpp
//  Bioinfo_IGA
//
//  Created by user on 2023/2/6.
//

#ifndef fitness_value_hpp
#define fitness_value_hpp

#include <stdio.h>
#include "tools.h"
typedef struct fitness_value
{
public:
    fitness_value();
public:
    double m_fintess_value;
    double m_multi_objs[MAX_OBJECTS];
public:
    void clone(fitness_value *other);
    void clear();
};
#endif /* fitness_value_hpp */
