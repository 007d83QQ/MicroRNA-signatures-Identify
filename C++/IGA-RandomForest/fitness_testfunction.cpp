//
//  fitness_testfunction.cpp
//  Bioinfo_IGA
//
//  Created by user on 2023/2/21.
//
#include "individual.hpp"
#include "fitness_function_data.hpp"
#include <cmath>
#include "fitness_testfunction.hpp"
fitness_testfunction::fitness_testfunction(int functionIndex)
{
    m_functionIndex=functionIndex;
}
fitness_testfunction::~fitness_testfunction()
{
    
}
void fitness_testfunction::fitness_function(individual *_individual)
{

    switch(m_functionIndex)
    {
    case 1:
            fitness_function01(_individual);
        break;
    case 2:
            fitness_function02(_individual);
        break;
    case 7:
            fitness_function07(_individual);
        break;
    }
}

//min fitness value->0
// [-0.1, 0.9]
void fitness_testfunction::fitness_function01(individual *_individual)
{
    int i,k=10;
    double result;
    double val1,pi=3.1416,val2;
    fitness_function_data *data=_individual->get_fitness_function_data();
    int count=data->parameter_count;
    result=0;
    for(i=0;i<count;i++)
    {
        val1 = _individual->get_parameters_value(i);
        val2=val1 * k * pi;
        result +=fabs((double)sin(val2)/(val2));
        
    }
    data->evalution_count++;
    fitness_value eval;
    eval.m_fintess_value=result;
    eval.m_multi_objs[0]=result;
    _individual->m_evulation_value=eval;
}
//------------------------------------------------------------------------------------
void fitness_testfunction::fitness_function02(individual *_individual)
{
    int i=0;
    double result;
    double val1, val2;
    fitness_function_data *data=_individual->get_fitness_function_data();
    int count=data->parameter_count;
    result=0;
    val2 = _individual->get_parameters_value(0);
    for(i=1;i<count;i++)
    {
        val1=val2;
        val2 = _individual->get_parameters_value(i);
        result -=(sin(val1+val1)+sin(2*val1*val2/3.0));
    }
    data->evalution_count++;
    fitness_value eval;
    eval.m_fintess_value=result;
    eval.m_multi_objs[0]=result;
    _individual->m_evulation_value=eval;
}
//-------------------------------------------------------------------------------------
// fitness function --> sum i=1 to n [ -[sin(xi)+sin((2*xi)/3)]  ]
// xi value belong to [3, 13]
// find max fitness value->1.216 X n
void fitness_testfunction::fitness_function07(individual *_individual)
{
    double result=0;
    double val1;
//    double ff[]={6.765, 5.156, 10.042,  5.757,  5.506,  4.446, 11.186,  4.128,  4.816,  4.893};
    fitness_function_data *data=_individual->get_fitness_function_data();
    int count=data->parameter_count;
#ifdef DEBUG_MODE
    printf("[%2d] ", _individual->m_individual_index_in_population);
    if(_individual->m_individual_index_in_population==-1)
    {
        printf("crossover  \n");
    }
#endif
    for(int i=0;i<count;i++)
    {
        val1 = _individual->get_parameters_value(i);
#ifdef DEBUG_MODE
        printf("%f ", val1);
#endif
 //       val1 =ff[i];
        result -=(sin(val1) + sin(val1*2/3.0));
    }
#ifdef DEBUG_MODE
    printf("=>%f \n", result);
#endif
    data->evalution_count++;
    fitness_value eval;
    eval.m_fintess_value=result;
    eval.m_multi_objs[0]=result;
    _individual->m_evulation_value=eval;

}
