//
//  intelligent_crossover.cpp
//  IGA
//
//  Created by user on 2021/4/25.
//

#include "intelligent_crossover.hpp"
#include "individual.hpp"
#include "fitness_function_data.hpp"
#define MAXDOUBLE 12000000000000
#define MINDOUBLE -12000000000000
intelligent_crossover::intelligent_crossover()
{
    m_remain=0;
}
intelligent_crossover::~intelligent_crossover()
{
    
}
void intelligent_crossover::init(fitness_function_data *data)
{
    m_fitness_function_data=data;
    generate_orthogonal_array(data->parameter_count);
    m_combined_individuals=new individual*[tm_height];
    for(int i=0;i<tm_height;i++)
    {
        m_combined_individuals[i] = new individual(m_fitness_function_data);
    }
    for(int i=0;i<2;i++)
    {
        candiate_solutions[2+i]=new individual(m_fitness_function_data);
        better_solutions[i]=new individual(m_fitness_function_data);
    }
    
}
bool intelligent_crossover::crossover(individual *parent1, individual *parent2, individual *best_solution)
{
    
    int             i,j,k;
//    double          temp1,Max_TM_eval=-120000000.00,Min_TM_eval=12000000.00;
//    double          R;
//    unsigned int    *temp2;
    int                small_factor_index,small_choice;    // smallest difference param position
    int             same_parameter=0;
    double            one,two,diff,small_diff;
    double            temp;//,bb;
    double best_fitnessValue_in_OA;
    bool change_best_solution_index=false;

//由直交表來組合新的解
    if(parent1->m_evulation_value.m_fintess_value==parent2->m_evulation_value.m_fintess_value)
        return true;
    candiate_solutions[0]=parent1;
    candiate_solutions[1]=parent2;
    
//根據直交表來產生實驗組合 並評估他們的 fitness value
//並把最小或最大值記錄下來
    int index=0, valueIndex=0;
    float fitnessValue=0;
    int best_fitnessValue_index_in_OA=0;
    int copy_count=1, candiate_index=0;
    for(i=0;i<tm_height;i++) {
        valueIndex=0;
        for(j=0;j<tm_width;j++)
        {
            index=*(tm_table+i*tm_width+j)-1;
            recombine_parameters(j, &valueIndex, candiate_solutions[index], m_combined_individuals[i]);
            
        }
        m_fitness_function_data->evalute(m_combined_individuals[i]);
        fitnessValue=m_combined_individuals[i]->m_evulation_value.m_fintess_value;
#ifdef DEBUG_MODE
        m_combined_individuals[i]->dump();
#endif
        if(i==0) {
            best_fitnessValue_in_OA=fitnessValue;
            best_fitnessValue_index_in_OA=0;
        }
        if(m_fitness_function_data->want_bigger_fitness_value)
        {
            if(best_fitnessValue_in_OA<fitnessValue) //紀錄最大值
            {
                best_fitnessValue_in_OA=fitnessValue;
//                Max_TM_index=i;
                best_fitnessValue_index_in_OA=i;
            }
        }else
        {
            if(best_fitnessValue_in_OA>fitnessValue)//記錄最小值
            {
                best_fitnessValue_in_OA=fitnessValue;
//                Min_TM_index=i;
                best_fitnessValue_index_in_OA=i;
            }
        }


  }

//    R=(double)(Max_TM_eval-Min_TM_eval)/tm_height;
//  R=1;
//  if(Min_TM_eval<0)
//      for(i=0;i<tm_height;i++)
//          m_combined_individuals[i]->m_fintess_value-=(Min_TM_eval+R);

    
    //透過直交表個因素貢獻度組合出推測最佳解組合
    k = tm_height/2;
    small_diff = MAXDOUBLE;
    valueIndex=0;

    for(i=0;i<tm_width; i++)
    {
        one = 0;
        two = 0;
        for(j=0;j<k;j++)
        {
            temp = m_combined_individuals[*(tm+j*tm_width+i)]->m_evulation_value.m_fintess_value;
            if(m_fitness_function_data->want_bigger_fitness_value)
            {
                one += (temp * temp);
            }else
            {
              temp=temp*temp;
              one += temp>MINDOUBLE ? (double)1/temp:MAXDOUBLE;
              //one += 1/(double)(temp * temp);
            }
          temp =  m_combined_individuals[*(tm+(j+k)*tm_width+i)]->m_evulation_value.m_fintess_value;
          if(m_fitness_function_data->want_bigger_fitness_value)
          {
              two += (temp * temp);
          }else{
              temp=temp*temp;
              two += temp>MINDOUBLE ? (double)1/temp:MAXDOUBLE;
              //two += 1/(double)(temp * temp);
          }
      }
      diff = one - two;
      temp = abs(diff);
        //主效果MSE之中, 在其中找差距最小的來替換
      if(temp < small_diff) {
          small_diff = temp;
          small_factor_index = i;
          if(diff > 0)
              small_choice = 1;
          else
              small_choice = 0;
      }

      if(diff>0) {
          candiate_index = 0;
      }
      else {
          candiate_index = 1;
      }
        recombine_parameters(i, &valueIndex, candiate_solutions[candiate_index], candiate_solutions[2], candiate_solutions[3]);
  }

    if(small_factor_index<m_remain)
    {
        valueIndex=small_factor_index*2;
    }else
    {
        valueIndex=small_factor_index+m_remain;
    }
    recombine_parameters(small_factor_index, &valueIndex, candiate_solutions[small_choice], candiate_solutions[3]);

    m_fitness_function_data->evalute(candiate_solutions[2]);
    m_fitness_function_data->evalute(candiate_solutions[3]);
#ifdef DEBUG_MODE
    printf("....candiate................................................\n");
    candiate_solutions[0]->dump();
    candiate_solutions[1]->dump();
    candiate_solutions[2]->dump();
    candiate_solutions[3]->dump();
#endif
  // find the better 2 genes from T1,T2,B1,B2
  // using selection sort
    
//-------------------------
    //在原來parent ,所有取樣, 推測解 中找尋兩個最佳的組合
    //取代原來的parents
    int best_fitnessValue_index_in_candiate_solutions[2];
    float fitness_value[4];
    for(int i=0;i<4;i++)
    {
        fitness_value[i]=candiate_solutions[i]->m_evulation_value.m_fintess_value;
    }
    found_two_best_value(fitness_value, best_fitnessValue_index_in_candiate_solutions, m_fitness_function_data->want_bigger_fitness_value);
    
//    printf("....candiate................................................\n");
//    candiate_solutions[best_fitnessValue_index_in_candiate_solutions[0]]->dump();
//    candiate_solutions[best_fitnessValue_index_in_candiate_solutions[1]]->dump();
    
    //直交表中有不錯解, 則加入候選
    if(best_fitnessValue_in_OA_proecess(best_fitnessValue_in_OA, best_fitnessValue_index_in_candiate_solutions[0]))
    {
        if(current_fitnessValue_is_better_than_best_one(best_fitnessValue_in_OA, best_solution->m_evulation_value.m_fintess_value))
        {
            change_best_solution_index=true;
        }
        m_combined_individuals[best_fitnessValue_index_in_OA]->clone_whole_data_to(better_solutions[0]);
        candiate_solutions[best_fitnessValue_index_in_candiate_solutions[0]]->clone_whole_data_to(better_solutions[1]);
    }else //推論出來的解是較佳解
    {
        if(current_fitnessValue_is_better_than_best_one(candiate_solutions[best_fitnessValue_index_in_candiate_solutions[0]]->m_evulation_value.m_fintess_value, best_solution->m_evulation_value.m_fintess_value))
        {
            change_best_solution_index=true;
        }
        candiate_solutions[best_fitnessValue_index_in_candiate_solutions[0]]->clone_whole_data_to(better_solutions[0]);
        candiate_solutions[best_fitnessValue_index_in_candiate_solutions[1]]->clone_whole_data_to(better_solutions[1]);
    }
    better_solutions[0]->clone_whole_data_to(parent1);
    better_solutions[1]->clone_whole_data_to(parent2);

#ifdef DEBUG_MODE
    printf("....candiate................................................\n");
    parent1->dump();
    parent2->dump();
#endif
    return change_best_solution_index;
}
//--------------------------------------------------------------------------
void intelligent_crossover::found_two_best_value(float *fintess_value, int *best_value_index, bool want_bigger)
{
    int k;
    for(int i=0;i<2;i++) {
        k = i;
        best_value_index[i]=i;
        for(int j=i+1;j<4;j++)
        {
            if(want_bigger)
            {
                if(fintess_value[j]>fintess_value[k])
                {
                    k = j;
                    best_value_index[i]=j;
                }
            }else
            {
                if(fintess_value[j]<fintess_value[k])
                {
                    k = j;
                    best_value_index[i]=j;
                }
            }
        }
        
        if(want_bigger)//移除比較
        {
            fintess_value[best_value_index[i]]=-MAX_VALUE;
        }else
        {
            fintess_value[best_value_index[i]]= MAX_VALUE;
        }
        
        
    }
}
//--------------------------------------------------------------------------
//依據直交表的因子來產生新的參數組合
//有些因子對應一個參數, 有些是兩個, 因子索引值小於 m_remain 則是兩個參數
//原則上, 產生一個新的參數組合, 若設定 target_individuals_two 參數時, 則可以多複製一份
//--------------------------------------------------------------------------
void intelligent_crossover::recombine_parameters(int factor_index, int *parameter_index, individual *source_individuals, individual *target_individuals, individual *target_individuals_two)
{
    int copy_count=1;
    int valueIndex=*parameter_index;
    if(factor_index<m_remain)
    {
        copy_count=2;
    }
    for(int k=0;k<copy_count;k++)
    {
        target_individuals->m_parameters[valueIndex]=source_individuals->m_parameters[valueIndex];
        if(target_individuals_two!=NULL)
            target_individuals_two->m_parameters[valueIndex]=source_individuals->m_parameters[valueIndex];
        valueIndex++;
    }
    *parameter_index=valueIndex;
}
//--------------------------------------------------------------------------
bool intelligent_crossover::current_fitnessValue_is_better_than_best_one(double best_fitnessValue_in_OA, double best_fitness_value)
{
    bool res=true;
    if(m_fitness_function_data->want_bigger_fitness_value)
    {
        res=best_fitnessValue_in_OA > best_fitness_value;
    }
    else
    {
        res=best_fitnessValue_in_OA < best_fitness_value;
    }
    return res;
}
//--------------------------------------------------------------------------
bool intelligent_crossover::best_fitnessValue_in_OA_proecess(double best_fitnessValue_in_OA, int best_fitnessValue_index)
{
    bool res=true;
    
    if(m_fitness_function_data->want_bigger_fitness_value)
    {
        res=best_fitnessValue_in_OA > candiate_solutions[best_fitnessValue_index]->m_evulation_value.m_fintess_value;
    }else
    {
        res=best_fitnessValue_in_OA < candiate_solutions[best_fitnessValue_index]->m_evulation_value.m_fintess_value;
    }
    
    return res;
}
//--------------------------------------------------------------------------
void intelligent_crossover::generate_orthogonal_array(int parameters_cout)
{
    int    i,j,k,m;
    int    dist,block;
    int    p1,p2;
    int count = parameters_cout;
    tm_width = parameters_cout;
    tm_height = 1;
    while (parameters_cout>1) {
        parameters_cout >>= 1;
        tm_height <<= 1;
    }
    if(tm_width>tm_height)
    {
        int ww=(tm_height << 1)-1;
        
        if(ww==tm_width)
        {
            tm_height=ww+1;
        }else
        {
            tm_width=tm_height-1;
        }
    }
    m_remain= count % tm_width;
    tm_table = (char *)malloc(sizeof(char)*tm_height*tm_width);
    tm       = (short *)malloc(sizeof(short)*tm_height*tm_width);

    m = 1;
    dist  = tm_height;
    block = tm_height/2;
    for(i=0;i<tm_width;i++) {
        p1 = 0;
        p2 = tm_height/2;
        if((i+1) == m) {
            for(j=0;j<tm_height;j+=dist) {
                for(k=0;k<block;k++) {
                    *(tm_table+(j+k)*tm_width+i) = 1;
                    *(tm_table+(j+block+k)*tm_width+i) = 2;
                    *(tm+(p1++)*tm_width+i) = j+k;
                    *(tm+(p2++)*tm_width+i) = j+block+k;
                }
            }
            m <<= 1;    // m = m * 2   乘
            dist >>= 1;
            block >>= 1;  // 除 2
        }
        else {
            k = m/2;
            for(j=0;j<tm_height;j++) {
                if(*(tm_table+j*tm_width+i-k) == *(tm_table+j*tm_width+k-1)) {
                    *(tm_table+j*tm_width+i) = 1;
                    *(tm+(p1++)*tm_width+i) = j;
                }
                else {
                    *(tm_table+j*tm_width+i) = 2;
                    *(tm+(p2++)*tm_width+i) = j;
                }
            }
        }
    }
}
