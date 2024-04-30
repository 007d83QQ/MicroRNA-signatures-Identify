//
//  population.cpp
//  IGA
//
//  Created by user on 2021/4/24.
//

#include "population.hpp"
#include "individual.hpp"
#include "one_crossover.hpp"
#include "fitness_function_data.hpp"
population::population(int individuals_count, fitness_function_data *data)
{
    if(data->want_bigger_fitness_value)
    {
        m_best_value_location=0;
    }else
    {
        m_best_value_location=individuals_count-1;
    }
    m_fitness_function_data=data;
    m_best_solution_index=0;
    set_individuals(individuals_count, m_fitness_function_data->parameter_count);// m_fitness_function_data->encode_bit_length);
//    evalution_count=0;
}
population::~population()
{
    for(int i=0;i<m_individuals_count;i++)
    {
        if(m_individuals[i]!=NULL)
        {
            delete m_individuals[i];
            m_individuals[i]=NULL;
        }
    }
    free(m_individuals);
}
void population::set_individuals(int individual_count, int parameters_count)
{
    m_individuals_count=individual_count;
    m_individuals=(individual **)malloc(sizeof(individual **)*individual_count);
    m_sorted_index=(int *)malloc(sizeof(int *)*individual_count);
    for(int i=0;i<m_individuals_count;i++)
    {
        m_individuals[i]=new individual(i, m_fitness_function_data);

    }
}
void population::set_crossover_operator(interface_crossover *_crossover)
{
    m_crossover_operator=_crossover;
}

//void population::set_fittness_function( float upper_bound, float lower_bound)
//{
////    fitness_function=fitness_fun;
//    m_upper_bound=upper_bound;
//    m_lower_bound=lower_bound;
//}
bool population::has_fitness_function()
{
    return m_fitness_function_data->evalute!=NULL;
}
individual *population::get_individual(int index)
{
    individual *_individual=NULL;
    if(m_individuals!=NULL && index>=0 && index<m_individuals_count)
    {
        _individual=m_individuals[index];
    }
    return _individual;
}
void population::init()
{
    for(int i=0;i<m_individuals_count;i++)
    {
        m_individuals[i]->init();
    }
}
void population::mutate()
{
    int individual_index = 0;
    int parameters_index = 0;
    int bit_index = 0;
    int mask_value = 0;
    int best=get_best_individual_index();
    individual_index = rand() % m_individuals_count;
//    printf("mute......best individual index: %d \n", best);
    if(individual_index ==best/*m_best_solution_index*/)
        return;
    
    parameters_index = rand() % m_fitness_function_data->parameter_count;
    bit_index = rand() % m_fitness_function_data->encode_bit_length[parameters_index];
    mask_value = 1 << bit_index;
    if(mask_value>0)
    {
        m_individuals[individual_index]->mute(parameters_index, mask_value);
    }

}
void population::sort()
{
    int    j;
 //   double fitness_value;
//    int    back_index;
    for(int i=0;i<m_individuals_count;i++)
        m_sorted_index[i] = i;
//    size = (int)((float)PS*POP_SIZE);
        // sort by value of fitness function
        // using insertion sort (suggest POP_SIZE <= 50)
    int ii=0;
    int temp;
    for(int i=0;i<m_individuals_count;i++)
    {
        ii=m_sorted_index[i];
        for(int j=i+1;j<m_individuals_count;j++)
        {
            if(m_individuals[ii]->m_evulation_value.m_fintess_value <m_individuals[m_sorted_index[j]]->m_evulation_value.m_fintess_value)
            {
                temp=m_sorted_index[j];
                m_sorted_index[j]=m_sorted_index[i];
                m_sorted_index[i]=temp;
                ii=temp;
            }
        }
    }
    
//    for(int i=1;i<m_individuals_count;i++) {
//        back_index=m_sorted_index[i];
//        fitness_value = m_individuals[back_index]->m_evulation_value.m_fintess_value;
//        j = i-1;
//
//        while(j>=0 && m_individuals[m_sorted_index[j]]->m_evulation_value.m_fintess_value < fitness_value)
//        {
//            m_sorted_index[j+1] = m_sorted_index[j];
//            j--;
//        }
//        m_sorted_index[j+1] = back_index;
//    }
    m_best_solution_index=m_sorted_index[m_best_value_location];
}
int population::get_best_individual_index()
{
//    return m_best_solution_index;
    return m_sorted_index[m_best_value_location];
}
void population::best_individual_repalce_worst_ones(int count)
{
    int worst_fitness_value_index=m_individuals_count-m_best_value_location-1;
    individual *worst_one;
    int index;
    for(int i=0;i<count;i++)
    {
        if(m_fitness_function_data->want_bigger_fitness_value)
        {
            index=worst_fitness_value_index-i;
        }else
        {
            index=worst_fitness_value_index+i;
        }
        
//        if(index<0)
//        {
//            index=-index;
//        }
        worst_one=m_individuals[m_sorted_index[index]];
        m_individuals[m_sorted_index[m_best_value_location]]->clone_whole_data_to(worst_one);
    }

}
void population::crossover(int first, int second)
{
    individual *best_solution=nullptr;
    if(m_best_solution_index>=0 && m_best_solution_index<m_individuals_count)
    {
        best_solution=m_individuals[m_best_solution_index];
    }
    if(m_crossover_operator->crossover(m_individuals[first], m_individuals[second], best_solution))
    {
        m_best_solution_index=first;
    }
}

void population::evalute()
{
//    printf("before evalute.....................%d\n", get_best_individual_index());
//    sort();
//    dump();
    for(int i=0;i<m_individuals_count;i++)
    {
        m_fitness_function_data->evalute(m_individuals[i]);
    }
//    printf("after evalute.....................%d\n", get_best_individual_index());
//    sort();
//    dump();
}
void population::evalute_best_individual()
{
    m_fitness_function_data->evalute(m_individuals[get_best_individual_index()]);
}
void population::mutate(int count)
{
//    printf("before mute.....................%d\n", get_best_individual_index());
//    sort();
//    dump();
//    printf(".....................\n");
//    m_individuals[m_best_solution_index]->dump();
    for(int i=0;i<count;i++)
    {
        mutate();
    }
    evalute_best_individual();
    printf("after mute.....................%d\n", get_best_individual_index());
    sort();
    dump();
    printf(".....................\n");
}
void population::dump()
{
    for(int i=0;i<m_individuals_count;i++)
    {
        printf("%d: ", m_sorted_index[i]);
        m_individuals[m_sorted_index[i]]->dump();
    }
}
fitness_value population::get_best_fitness_value()
{
    return m_individuals[m_best_solution_index]->m_evulation_value;
}
