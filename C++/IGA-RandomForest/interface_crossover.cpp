//
//  interface_crossover.cpp
//  IGA
//
//  Created by user on 2021/4/25.
//

#include "interface_crossover.hpp"
#include "individual.hpp"
void interface_crossover::set_child_count(int size)
{
    if(m_created_child_count!=m_child_count)
    {
        clear_child_data();
        create_child_data(size);
    }
}
void interface_crossover::create_child_data(int size)
{
    m_child_count=size;
    m_child=(individual **)malloc(sizeof(individual **)*m_child_count);
    for(int i=0;i<m_child_count;i++)
    {
        m_child[i]=new individual(m_fitness_function_data);
//        m_child[i]->init(m_lower_bound, m_upper_bound);
    }
    m_created_child_count=size;
}
void interface_crossover::clear_child_data()
{
    if(m_child) //
    {
        for(int i=0;i<m_child_count;i++)
        {
            if(m_child[i]!=NULL)
            {
                delete m_child[i];
            }
            
        }
        free(m_child);
    }
}
void interface_crossover::init( fitness_function_data *data)
{
    m_fitness_function_data=data;
    set_child_count(m_child_count);
}
