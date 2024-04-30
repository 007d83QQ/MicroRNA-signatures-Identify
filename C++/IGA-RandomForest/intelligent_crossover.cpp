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
int intelligent_crossover::reduce_OA_table_size(individual *parent1, individual *parent2, std::vector<int> &cut_pos, int &current_oa_height, int &current_oa_width, int &oa_jump_distance)
{
    int size=0;
    int num_different_factors=0;
    int width=tm_height;
    int n=parent1->get_fitness_function_data()->parameter_count;
    int count=0;
    int remain=0;
    std::vector<int> different_pos;

    for(int i=0;i<n;i++)
    {
        if(parent1->m_parameters[i]!=parent2->m_parameters[i])
        {
            num_different_factors++;
            different_pos.push_back(i);
        }
    }
    different_pos.push_back(n);
    while(num_different_factors<width-1) //不同因子數量小於直交表寬度, 則縮減直交表的大小
    {
        size++;
        width >>= 1; //除 2
    }
    if(size>0)
    {
        current_oa_height= tm_height >> size;
        current_oa_width= current_oa_height -1;
        oa_jump_distance= 1 << size;
    }else
    {
        current_oa_width=tm_width;
        current_oa_height=tm_height;
        oa_jump_distance=1;
        
    }
    remain=num_different_factors % current_oa_width;
    int inc=num_different_factors / current_oa_width;
    cut_pos.clear();
    cut_pos.push_back(0);
    int idx=0;
    for(int i=0;i<current_oa_width;i++)
    {
        if(i<remain)
        {
            idx+=(inc+1);
        }else
        {
            idx+=inc;
        }
        cut_pos.push_back(different_pos[idx]);
    }
    return size;
}
//-------------------------------------------------------
bool intelligent_crossover::crossover(individual *parent1, individual *parent2, individual *best_solution)
{
    
    int             i,j,k;
//    double          temp1,Max_TM_eval=-120000000.00,Min_TM_eval=12000000.00;
//    double          R;
//    unsigned int    *temp2;
    int                small_factor_index,small_choice;    // smallest difference param position
//    int             same_parameter=0;
    double            one,two,diff,small_diff;
    double            temp;//,bb;
    double best_fitnessValue_in_OA;
    bool change_best_solution_index=false;
    //根據直交表來產生實驗組合 並評估他們的 fitness value
    //並把最小或最大值記錄下來
    int index=0, valueIndex=0;
    float fitnessValue=0;
    int best_fitnessValue_index_in_OA=0;
    int copy_count=1, candiate_index=0;
    int reduce_size=0;
    std::vector<int> cut_pos;
    int current_oa_width;
    int current_oa_height;
    bool reduce;
    int oa_jump_distance=1;
//由直交表來組合新的解
    if(parent1->m_evulation_value.m_fintess_value==parent2->m_evulation_value.m_fintess_value)
        return false;
    candiate_solutions[0]=parent1;
    candiate_solutions[1]=parent2;

    reduce_size=reduce_OA_table_size(parent1,parent2, cut_pos, current_oa_height, current_oa_width, oa_jump_distance);
    
    int width_interval=oa_jump_distance*tm_width;
    
    for(i=0;i<current_oa_height;i++) {
        valueIndex=0;
        int width_index=i*width_interval;
        for(j=0;j<current_oa_width;j++)
        {
            index=*(tm_table+width_index+j)-1;
            recombine_parameters(j, cut_pos, candiate_solutions[index], m_combined_individuals[i]);
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
                best_fitnessValue_index_in_OA=i;
            }
        }else
        {
            if(best_fitnessValue_in_OA>fitnessValue)//記錄最小值
            {
                best_fitnessValue_in_OA=fitnessValue;
                best_fitnessValue_index_in_OA=i;
            }
        }
  }
//    parent1->dump();
//    parent2->dump();
//    printf("....................................................................\n");
//    for(i=0;i<current_oa_height;i++)
//    {
//        m_combined_individuals[i]->dump();
//    }
//    printf("....................................................................\n");
//    R=(double)(Max_TM_eval-Min_TM_eval)/current_oa_height;
//  R=1;
//  if(Min_TM_eval<0)
//      for(i=0;i<current_oa_height;i++)
//          m_combined_individuals[i]->m_fintess_value-=(Min_TM_eval+R);

    
    //透過直交表個因素貢獻度組合出推測最佳解組合
    k = current_oa_height/2;
    small_diff = MAXDOUBLE;
    valueIndex=0;
    small_factor_index=0;
    for(i=0;i<current_oa_width; i++)
    {
        one = 0;
        two = 0;
        for(j=0;j<k;j++)
        {
            temp = m_combined_individuals[*(tm+j*width_interval+i)]->m_evulation_value.m_fintess_value;
            if(m_fitness_function_data->want_bigger_fitness_value)
            {
                one += (temp * temp);
            }else
            {
              temp=temp*temp;
              one += temp>MINDOUBLE ? (double)1/temp:MAXDOUBLE;
              //one += 1/(double)(temp * temp);
            }
          temp =  m_combined_individuals[*(tm+(j+k)*width_interval+i)]->m_evulation_value.m_fintess_value;
          if(m_fitness_function_data->want_bigger_fitness_value)
          {
              two += (temp * temp);
          }else{
              temp=temp*temp;
              two += temp>MINDOUBLE ? (double)1/temp:MAXDOUBLE;
              //two += 1/(double)(temp * temp);
          }
      }
//        printf("\n");
      diff = one - two;
      temp = abs(diff);
//        printf("%f-%f=%f \n", one, two, diff);
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
        recombine_parameters(i, cut_pos, candiate_solutions[candiate_index], candiate_solutions[2], candiate_solutions[3]);
  //      recombine_parameters(i, &valueIndex, candiate_solutions[candiate_index], candiate_solutions[2], candiate_solutions[3]);
  }
//    if(current_oa_width==3)
//    {
//        printf("small....\n");
//    }
//    printf("....................................................................\n");
    if(small_factor_index<m_remain)
    {
        valueIndex=small_factor_index*2;
    }else
    {
        valueIndex=small_factor_index+m_remain;
    }
//    (small_factor_index, &valueIndex, candiate_solutions[small_choice], candiate_solutions[3]);
    recombine_parameters(small_factor_index, cut_pos, candiate_solutions[small_choice], candiate_solutions[3]);

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
void intelligent_crossover::recombine_parameters(int factor_index, std::vector<int> cut_point, individual *source_individuals, individual *target_individuals, individual *target_individuals_two)
{
    for(int k=cut_point[factor_index];k<cut_point[factor_index+1];k++)
    {
        target_individuals->m_parameters[k]=source_individuals->m_parameters[k];
        if(target_individuals_two!=NULL)
            target_individuals_two->m_parameters[k]=source_individuals->m_parameters[k];
    }
}
//--------------------------------------------------------------------------
bool intelligent_crossover::current_fitnessValue_is_better_than_best_one(double best_fitnessValue_in_OA, double best_fitness_value)
{
    bool res=true;
    if(m_fitness_function_data->want_bigger_fitness_value)
    {
        res=best_fitnessValue_in_OA >= best_fitness_value;
    }
    else
    {
        res=best_fitnessValue_in_OA <= best_fitness_value;
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
