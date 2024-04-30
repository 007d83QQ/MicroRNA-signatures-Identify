//
//  individual.cpp
//  IGA
//
//  Created by user on 2021/4/24.
//

#include "individual.hpp"
#include "population.hpp"
#include "fitness_function_data.hpp"
#include "fitness_value.hpp"

individual::individual(int index, fitness_function_data *data)
{
    setup(index, data);
}
individual::individual(fitness_function_data *data)
{
    setup(-1, data);
}
individual::~individual()
{
    if(m_fitness_function_data->parameter_count>0)
    {
        free(m_parameters);
        m_parameters=NULL;
    }
 //   m_fitness_function_data->parameter_count=0;
}
void individual::setup(int index, fitness_function_data *data)
{
    int size=data->parameter_count;
    if(size>0)
    {
        int *encode_value_range=data->encode_value_range;
        m_individual_index_in_population=index;
        m_fitness_function_data=data;
        m_parameters=(int *)malloc(sizeof(int)*size);
        for(int i=0;i<size;i++)
        {
            m_parameters[i]=rand() % encode_value_range[i];
        }
        m_is_change=true;
    }else
    {
        perror("parameter count must bigger over 0 \n");
    }
}
void individual::init()
{
    int *encode_value_range=m_fitness_function_data->encode_value_range;
    for(int i=0;i<m_fitness_function_data->parameter_count;i++)
    {
        m_parameters[i]=rand() % encode_value_range[i];
    }
    m_evulation_value.clear();
}
float individual::get_min_value(int index ) {return m_fitness_function_data->lower_bound[index];}
float individual::get_interval_value(int index) {return m_fitness_function_data->interval[index];}
void individual::mute(int parameters_index, int mask_value)
{
    m_parameters[parameters_index] ^=mask_value;
    m_is_change=true;
}
void individual::clone_whole_data_to(individual *target)
{
    for(int i=0;i<m_fitness_function_data->parameter_count;i++)
    {
        target->m_parameters[i]=m_parameters[i];
    }
    target->m_evulation_value=m_evulation_value;
//    target->m_evulation_value.clone(&m_evulation_value);
//    target->m_fintess_value=m_fintess_value;
}
void individual::copy_parameters(individual *target, int start_postion, int len)
{
    int size=start_postion+len;
    size=size<=m_fitness_function_data->parameter_count ? size:m_fitness_function_data->parameter_count;
    for(int i=start_postion;i<size;i++)
    {
        m_parameters[i]=target->m_parameters[i];
    }
}
double individual::get_parameters_value(int index)
{
    int range=m_fitness_function_data->encode_value_range[index]-1;
    float interval=m_fitness_function_data->interval[index];
    float min=m_fitness_function_data->lower_bound[index];
    return (m_parameters[index]/(float)range) * interval + min;
}
void individual::dump()
{
    for(int i=0;i<m_fitness_function_data->parameter_count;i++)
    {
        printf(" %6.3f", get_parameters_value(i)); //(m_parameters[i]/(float)range[i]-1)*interval[i]+min[i]
        //printf(" %d", m_parameters[i]);
    }
    printf(" => %f , multi-objs:", m_evulation_value.m_fintess_value);
    for(int i=0;i<m_fitness_function_data->m_num_objectives;i++)
    {
        printf(" %f, ", m_evulation_value.m_multi_objs[i]);
    }
    printf("\n");
}
fitness_function_data *individual::get_fitness_function_data(){ return m_fitness_function_data;}
