//
//  fitness_value.cpp
//  Bioinfo_IGA
//
//  Created by user on 2023/2/6.
//

#include "fitness_value.hpp"
fitness_value::fitness_value()
{
    clear();
}
void fitness_value::clear()
{
    m_fintess_value=0;
    for(int i=0;i<MAX_OBJECTS;i++)
    {
        m_multi_objs[i]=0;
    }
}
void fitness_value::clone(fitness_value *other)
{
    m_fintess_value=other->m_fintess_value;
    for(int i=0;i<MAX_OBJECTS;i++)
    {
        m_multi_objs[i]=other->m_multi_objs[i];
    }
}
