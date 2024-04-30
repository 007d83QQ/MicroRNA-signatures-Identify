//
//  ga_info_statistics.cpp
//  IGA
//
//  Created by user on 2023/1/31.
//

#include "ga_info_statistics.hpp"
static ga_info_statistics *self;
ga_info_statistics::ga_info_statistics(int num)
{
    num_multi_objectives=num;
    ga_info.clear();
    self=this;
    dump_fun=dump;
}
ga_info_statistics::~ga_info_statistics()
{
    for(int i=0;i<ga_info.size();i++)
    {
        free(ga_info[i]);
    }
    ga_info.clear();
}
void ga_info_statistics::set_num_multi_objectives(int num)
{
    self->num_multi_objectives=num;
}
void ga_info_statistics::add(int generation, int cost, fitness_value _fitness)
{
    generation_info *info=new generation_info(cost, _fitness);
    ga_info.push_back(info);
}
void ga_info_statistics::dump()
{
    ga_info_statistics *obj=self;
    for(int i=0;i<obj->ga_info.size();i++)
    {
        printf("%3d th, cost =%d, fitness=%f ", i+1, obj->ga_info[i]->cost, obj->ga_info[i]->m_evaluation.m_fintess_value);
        for(int j=0;j<obj->num_multi_objectives;j++)
        {
            printf("%f ", obj->ga_info[i]->m_evaluation.m_multi_objs[j]);
        }
        printf("\n");
    }
}
