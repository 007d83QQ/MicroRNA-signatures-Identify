//
//  crossover_step.cpp
//  IGA
//
//  Created by user on 2021/4/24.
//

#include "one_crossover.hpp"
#include "individual.hpp"
#include "fitness_function_data.hpp"
//---------------------------------------------------------
one_crossover::one_crossover(int child_count)
{
    m_child_count=child_count;
    m_created_child_count=0;
}

one_crossover::~one_crossover()
{
    clear_child_data();
}

bool one_crossover::crossover(individual *parent1, individual *parent2, individual *best_solution)
{
    int param_index = 1+ rand() % (m_fitness_function_data->parameter_count-1);
    individual *temp;
    printf(".................crossover...............\n");
    parent1->dump();
    parent2->dump();
    printf("child......\n");
    for(int i=0;i<m_child_count;i++)
    {
        m_child[i]->copy_parameters(parent1, 0,param_index );
        m_child[i]->copy_parameters(parent2, param_index, m_fitness_function_data->parameter_count-param_index);
        m_fitness_function_data->evalute(m_child[i]);
        m_child[i]->dump();
        temp=parent1;
        parent1=parent2;
        parent2=temp;
    }
    
    get_best_two_replace_parent(parent1, parent2, m_child[0], m_child[1]);
    parent1->dump();
    parent2->dump();
    printf("crossover end...\n");
    return false;
}
void one_crossover::get_best_two_replace_parent(individual *parent1, individual *parent2, individual *child1, individual *child2)
{
    individual *ind[4]={parent1, parent2, child1, child2};
    int index[4];
    int max_index=0;
    int temp;
    float max;
    for(int i=0;i<4;i++)
    {
        index[i]=i;
    }
    for(int i=0;i<2;i++)
    {
        max=ind[index[i]]->m_evulation_value.m_fintess_value;
        max_index=i;
        for(int j=i+1;j<4;j++)
        {
            if(max<ind[index[j]]->m_evulation_value.m_fintess_value)
            {
                max=ind[index[j]]->m_evulation_value.m_fintess_value;
                max_index=j;
            }
        }
        temp=index[i];
        index[i]=index[max_index];
        index[max_index]=temp;
    }
    if(index[0]==1)
    {
        temp=index[0];
        index[0]=index[1];
        index[1]=temp;
    }
    if(index[1]==0)
    {
        temp=index[0];
        index[0]=index[1];
        index[1]=temp;
    }
    for(int i=0;i<2;i++)
    {
        if(index[i]!=i)
        {
            ind[index[i]]->clone_whole_data_to(ind[i]);
        }
    }
}
