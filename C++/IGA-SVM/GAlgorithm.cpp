//
//  GAlgorithm.cpp
//  IGA
//
//  Created by user on 2021/4/23.
//

#include "GAlgorithm.hpp"
#include "population.hpp"
#include "one_crossover.hpp"
#include "fitness_function_data.hpp"
//-------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------
GAlgorithm::GAlgorithm()
{
    m_population=NULL;
    m_selection_rate=0;
    m_crossover_rate=0;
    m_mutation_rate=0;
    m_population_size=0;
    m_crossover=NULL;
    srand(time(NULL));
}
GAlgorithm::~GAlgorithm()
{
    delete m_population;
    for(int i=0;i<m_corssover_times*2;i++)
    {
        if(m_pair_data[i]!=NULL)
        {
            delete(m_pair_data[i]);
        }
    }
    free(m_pair_data);
}
void GAlgorithm::init(STOP_CONDITION mode,int stop_condition_parameter, int poputlation_size, float selection_rate, float crossover_rate, float mutation_rate, int parameter_count, int encode_bits_count, void (*fitness_fun)(individual *_individual), float lower_bound, float upper_bound, bool want_bigger_fitness_value)
{
    stop_mode=mode;
    if(mode==STOP_CONDITION_GENERATION)
    {
        m_iterations_count=stop_condition_parameter;
    }else
    {
        m_max_evalution_count=stop_condition_parameter;
    }
    m_population_size=poputlation_size;
    m_selection_rate=selection_rate;
    m_crossover_rate=crossover_rate;
    m_mutation_rate=mutation_rate;
    m_want_bigger_fitness_value=want_bigger_fitness_value;
    m_replace_count=(int)(m_selection_rate*m_population_size);
    m_corssover_times=(m_population_size*m_crossover_rate/2);

    m_pair_data=(crossover_pairs **)malloc(sizeof(crossover_pairs **)*m_corssover_times*2);
    
    for(int i=0;i<m_corssover_times*2;i++)
    {
        m_pair_data[i]=new crossover_pairs();
    }
    m_mutations_count = (int)(m_population_size * parameter_count * encode_bits_count * m_mutation_rate);
    m_fitness_function_data->init(parameter_count, lower_bound, upper_bound, encode_bits_count, 1, m_want_bigger_fitness_value, fitness_fun);
    m_population=new population(m_population_size, m_fitness_function_data);
    ga_statistics.set_num_multi_objectives(1);
}
void GAlgorithm::init(STOP_CONDITION mode, int stop_condition_parameter, int poputlation_size, float selection_rate, float crossover_rate, float mutation_rate, fitness_function_data *data)
{
    stop_mode=mode;
    if(mode==STOP_CONDITION_GENERATION)
    {
        m_iterations_count=stop_condition_parameter;
    }else
    {
        m_max_evalution_count=stop_condition_parameter;
    }
    m_population_size=poputlation_size;
    m_selection_rate=selection_rate;
    m_crossover_rate=crossover_rate;
    m_mutation_rate=mutation_rate;
    m_replace_count=(int)(m_selection_rate*m_population_size);
    m_corssover_times=(m_population_size*m_crossover_rate/2);

    m_pair_data=(crossover_pairs **)malloc(sizeof(crossover_pairs **)*m_corssover_times*2);
    for(int i=0;i<m_corssover_times*2;i++)
    {
        m_pair_data[i]=new crossover_pairs();
    }
    m_fitness_function_data=data;
    int count=0;
    for(int i=0;i<data->parameter_count;i++)
    {
        count+=data->encode_bit_length[i];
    }
    m_mutations_count = (int)(m_population_size * count * m_mutation_rate);
    m_population=new population(m_population_size, m_fitness_function_data);
    ga_statistics.set_num_multi_objectives(data->m_num_objectives);
}
void GAlgorithm::set_crossover_operator(interface_crossover *_crossover)
{
    m_crossover=_crossover;
    m_population->set_crossover_operator(_crossover);
    m_crossover->init(m_fitness_function_data);//m_parameters_count, m_lower_bound, m_upper_bound,fitness_function);
}
void GAlgorithm::population_init()
{
    
    if(!m_population->has_fitness_function())
    {
        perror("fitness is NULL \n");
        exit(1);
    }
    if(m_population==NULL)
    {
        perror("init is not run \n");
        exit(1);
    }
    if(m_crossover==NULL)
    {
        perror("crossover operator is not setup \n");
        exit(1);
    }
    m_population->init();
}
void GAlgorithm::evalute()
{
    m_population->evalute();
}
void GAlgorithm::select()
{
    m_population->sort();
    m_population->best_individual_repalce_worst_ones(m_replace_count);
//    m_population->dump();
}
void GAlgorithm::crossover()
{
    int i;//,j;
    int best=m_population->get_best_individual_index();
//
    for(i=0;i<m_corssover_times;i++)
    {
        for(int j=0;j<2;j++)
        {
            if(j==0)
            {
                if(i==0)
                {
                    m_pair_data[0]->m_first_index = best;
                }else
                {
                    m_pair_data[i]->m_first_index = rand() % m_population_size;
                }
                
            }else
            {
                m_pair_data[i]->m_second_index = rand() % m_population_size;
                while(m_pair_data[i]->m_second_index==m_pair_data[i]->m_first_index)
                {
                    m_pair_data[i]->m_second_index  = rand() % m_population_size;
                }
            }
        }

    }
    for(i=0;i<m_corssover_times;i++)
    {
        m_population->crossover(m_pair_data[i]->m_first_index, m_pair_data[i]->m_second_index);
    }
}
void GAlgorithm::mutate()
{
    m_population->mutate(m_mutations_count);
}
void GAlgorithm::dump()
{
    printf("........................\n");
    m_population->sort();
    m_population->dump();
}
void GAlgorithm::exectue()
{
    population_init();
    if(stop_mode==STOP_CONDITION_GENERATION)
    {
        evalute();
        for(int i=0;i<m_iterations_count;i++)
        {
//            evalute();
            select();
            printf("-------%dth, select %f \n", i, m_population->get_best_fitness_value().m_fintess_value);
            crossover();
//            evalute();
//            printf("crossover,  %f \n",  m_population->get_best_fitness_value().m_fintess_value);
            mutate();
//            printf("mutate, %f ==>index:%d \n",  m_population->get_best_fitness_value().m_fintess_value, m_population->get_best_individual_index());
            evalute();
//            printf("evalute, %f ==>index:%d \n",  m_population->get_best_fitness_value().m_fintess_value, m_population->get_best_individual_index());
            printf("-------%dth, select %f \n", i, m_population->get_best_fitness_value().m_fintess_value);
            ga_statistics.add(i+1, m_fitness_function_data->evalution_count, m_population->get_best_fitness_value());
        }
    }else
    {
        m_iterations_count=0;
        evalute();
        while (m_fitness_function_data->evalution_count<m_max_evalution_count) {
            
            select();
            crossover();
            mutate();
            evalute();
            m_iterations_count++;
            printf("%dth, %f \n", m_iterations_count, m_population->get_best_fitness_value().m_fintess_value);
            ga_statistics.add(m_iterations_count, m_fitness_function_data->evalution_count, m_population->get_best_fitness_value());
        }
    }
    printf("演化代數: %d, 評估次數: %d\n", m_iterations_count, m_fitness_function_data->evalution_count);
    if(ga_statistics.dump_fun!=nullptr)
        ga_statistics.dump_fun();
    dump();
}
