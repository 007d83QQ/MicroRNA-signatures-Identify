//
//  main.cpp
//  Bioinfo_IGA
//
//  Created by user on 2023/1/29.
//
#include <float.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <errno.h>
#include "fitness_function_data.hpp"
#include "individual.hpp"
#include "bioinfo.h"
#include <iostream>
using namespace std;
static bioinfo *bio=nullptr;
bioinfo::bioinfo(char *filename, int num_folds)
{
    init_data_set(filename, num_folds);
    bio=this;
}
bioinfo::~bioinfo()
{
    svm_freep(useful_data);
    svm_freep(prob.y);
    svm_freep(prob.x);
    svm_freep(x_origin_space);
    svm_freep(x_space);
    
}
//------------------------------------------------------------------------------------------------
void bioinfo::init_data_set(char *filename, int num_flods)
{
    data_count=0;
    num_features=0;
    load_training_data_set(filename);
    initial=true;
    init_svm(2,2);
    cross_validation = 1;
    nr_fold = num_flods;
}
//------------------------------------------------------------------------------------------------
void bioinfo::bioinfo_problem_fitness_function(individual *_individual) {
    bio->transfer_to_svm_data(_individual);
}
void bioinfo::transfer_to_svm_data(individual *_individual)
{
    int value=0;
    fitness_function_data *data=_individual->get_fitness_function_data();
    int index=0;
    int len;
    int loc=0;
    int dest_loc=0;
    param.C =1;// _individual->get_parameters_value(index);
    index++;
    param.gamma =1/32.0;//_individual->get_parameters_value(index);
    param.gamma=0.5;
    index++;
    loc=0;
    dest_loc=0;
    int real_used_count=0;
    int would_use=0;
    for(int i=index;i<data->parameter_count;i++)
    {
        value=_individual->m_parameters[i];
        len=data->encode_bit_length[i];
        for(int j=0;j<len;j++)
        {
            would_use=value & 1;
            useful_data[loc+len-j-1]=would_use;
            value >>= 1;  //右移1個bit
            if(would_use)
                real_used_count++;
        }
        loc+=len;
    }
    svm_node *p;
    int k=0;
    for(int i=0;i<data_count;i++)
    {
        
        p=origin_data.x[i];
        prob.x[i]=&x_space[k];
        for(int j=0;j<num_features;j++)
        {
            //if(useful_data[j]==1)
            {
                x_space[k].value=p[j].value;
                x_space[k].index=p[j].index;
                k++;
            }
        }
        x_space[k].value=-1;
        x_space[k].index=-1;
        k++;
    }
    
    
    fitness_value eval;
    eval.m_multi_objs[0]=do_cross_validation();
    eval.m_multi_objs[1]=current_rate[0];
    eval.m_multi_objs[2]=current_rate[1];
    eval.m_multi_objs[3]=current_rate[2];
    eval.m_multi_objs[4]=current_rate[3];
    eval.m_multi_objs[5]=real_used_count;
    eval.m_fintess_value=0;
//    if(eval.m_multi_objs[0]>=0.5)
    {
        eval.m_fintess_value=100*(eval.m_multi_objs[0]-0.5)+60*(1-eval.m_multi_objs[5]/(float)num_features);
    }
    
    _individual->m_evulation_value=eval;
    data->evalution_count++;
//    printf("處理中.....\n");
}
void bioinfo::transfer_to_svm_data2(individual *_individual)
{
    int value=0;
    fitness_function_data *data=_individual->get_fitness_function_data();
    int index=0;
    int len;
    int loc=0;
    int dest_loc=0;
//    double v[]={3737.000, 1591.000, 21843.000, 59733.000, 12959.000, 1071864768.000};
    double v[]={146.000, 144.000, 200.000, 16.000, 128.000, 38.000, 74.000, 65.000, 217.000, 64.000, 34.000};
    int ii[]={1, 25, 26, 35, 39, 48, 49, 61, 63, 70, 74, 79, 81};
//    double v[]={0.000,  8.000, 1024.000, 256.000,  0.000,  0.000,  8.000,  0.000,  0.000,  8.000,  0.000,  0.000, 0.000,  0.000,  0.000, 2048.000,  0.000,  0.000,  0.000,  0.000,  0.000,  1.000,  0.000,  0.000,  0.000,  0.000,  0.000,  0.000,  0.000,  0.000,  0.000,  0.000, 32.000,  0.000, 8192.00,  0.000,  0.000,  0.000,  0.000, 16.000,  0.000,  0.000, 16384.000,  0.000,  0.000,  0.000,  0.000, 1024.000,  0.000, 32768.000,  0.000,  0.000,  0.000,  0.000,  0.000,  0.000,  0.000,  0.000,  0.000,  0.000,  0.000,  0.000,  0.000,  0.000,  0.000,  0.000,  0.000, 4160.000,  0.000,  0.000,  0.000,  0.000,  0.000,  0.000,  0.000, 16384.000,  0.000,  0.000,  0.000,  0.000,  0.000,  0.000,  0.000,  0.000,  0.000,  0.000,  0.000, 0.000,  0.000,  0.000,  0.000,  0.000,  0.000,  0.000,  0.000,  0.000,  0.000 };
    param.C = 1;//1;//_individual->get_parameters_value(index);
    param.gamma = 1/13.0f;//0.179;//_individual->get_parameters_value(index);
    loc=0;
    dest_loc=0;
    int real_used_count=13;
    int would_use=0;
    
    for(int i=index;i<data->parameter_count-2;i++)
    {
        value=v[i];//_individual->m_parameters[i];
        len=data->encode_bit_length[i];
        for(int j=0;j<len;j++)
        {
            would_use=value & 1;
            useful_data[loc+len-j-1]=would_use;
            value >>= 1;  //右移1個bit
            if(would_use)
                real_used_count++;
        }
        loc+=len;
    }
    svm_node *p;
    int k=0, s=0;
    for(int i=0;i<data_count;i++)
    {
        
        p=origin_data.x[i];
        prob.x[i]=&x_space[k];
        s=0;
        for(int j=0;j<num_features;j++)
        {
            if((j+1)==ii[s])
            if(useful_data[j]==1)
            {
                x_space[k].value=p[j].value;
                x_space[k].index=p[j].index;
                k++;
                s++;
                if(s>=13)
                {
                    break;
                }
            }
        }
        x_space[k].value=-1;
        x_space[k].index=-1;
        k++;
    }
/*    k=10;
    p=origin_data.x[k];
    for(int i=0;i<num_features;i++)
    {
        if(useful_data[i]==1)
        {
            printf("%f ", p[i].value);
        }
    }
    printf("\n");
    printf("data.....\n");
    p=prob.x[k];
    int i=0;
    while(1)
    {
        if(p[i].index!=-1)
        {
            printf("%f ", p[i].value);
        }else
        {
            break;
        }
        i++;
    }
    printf("\n");
    */
    fitness_value eval;
    eval.m_multi_objs[0]=do_cross_validation();
    eval.m_multi_objs[1]=current_rate[0];
    eval.m_multi_objs[2]=current_rate[1];
    eval.m_multi_objs[3]=real_used_count;
    eval.m_fintess_value=100*(eval.m_multi_objs[0])+60*(1-eval.m_multi_objs[3]/(float)num_features);
//    eval.m_fintess_value=eval.m_multi_objs[0]+40*(1-eval.m_multi_objs[1]/(float)num_features);
    _individual->m_evulation_value=eval;
    data->evalution_count++;
    printf("處理中.....\n");
}
//------------------------------------------------------------------------------------------------
//載入資料
void bioinfo::load_training_data_set(char *filename)
{
    int i;//,index;
//    char title_name[128];
    float value;
    char *p=nullptr;
    long len=0;

    fp=fopen(filename,"r");
    if(fp==NULL)
    {
        fprintf(stderr,"can't open file %s\n", filename);
        exit(1);
    }
    max_index = 0;
    min_index = 1;
    //====================================================
    //讀取第一行標題資料, 並且計算有多少特徵參數
    //
    line = (char *) malloc(max_line_len*sizeof(char));
    readline(fp);
    p=line;
    len=strlen(line);
    num_features=0;
    while(p-line<len)
    {
        SKIP_TITLE_ELEMENT
        num_features++;
    }
    num_features--;//去除 類別項目
    //====================================================
    //宣告使用到的記憶體空間
    //
    max_bound=svm_malloc(num_features+1, float);//(float *)malloc(num_nonzeros*sizeof(float));
    min_bound=svm_malloc(num_features+1, float);//(float *)malloc(num_nonzeros*sizeof(float));
    interval=svm_malloc(num_features+1, float);//(float *)malloc(num_nonzeros*sizeof(float));
    useful_data=svm_malloc(num_features+1, char);
    memset(max_bound, 0, num_features+1);
    memset(min_bound, 0, num_features+1);
    memset(interval, 0, num_features+1);
    memset(useful_data, 0, num_features+1);
    for(int i=0;i<num_features+1;i++)
    {
        min_bound[i]=MAX_FEATURE_VALUE;
        max_bound[i]=-MAX_FEATURE_VALUE;
    }
    //====================================================
    //計算資料總共幾筆, 並找出每一特徵參數的最大級最小值
    //
    data_count=0;
    while(readline(fp)!=NULL)
    {
        p=line;
        i=0;
        for(int i=0;i<num_features+1;i++)
        {
            sscanf(p,"%f,",&value);
            if(i<num_features)
            {
                max_bound[i] = max(max_bound[i], value);
                min_bound[i] = min(min_bound[i], value);
                SKIP_ELEMENT
            }
            
        }
        data_count++;
    }
    printf("row: %d, columns: %d \n", data_count, num_features);
    //====================================================
    // 計算每特徵參數的最大與最小值的差距, 為了縮放參數來準備
    //
    int num=0;
    for(int i=0;i<num_features;i++)
    {
        if(max_bound[i]== min_bound[i])
        {
            useful_data[i]=1;
            interval[i]=0;
            
        }else
        {
            interval[i]=max_bound[i]-min_bound[i];
            num++;
        }
    }
    prob.l=data_count;
    prob.y=svm_malloc(data_count, double);
    prob.x=svm_malloc(data_count, svm_node*);
    x_space = svm_malloc((num+1)*data_count, svm_node);
    
    origin_data.l=data_count;
    origin_data.y=svm_malloc(data_count, double);
    origin_data.x=svm_malloc(data_count, svm_node*);
    x_origin_space = svm_malloc((num+1)*data_count, svm_node);


//    memset(useful_data, 0, num_features*sizeof(char));
    rewind(fp);
    //====================================================
    //將資料讀取到prob 結構之中
    //
    read_data_to_memory(fp);
    fclose(fp);
    for(int i=0;i<(int)num_features;i++)
    {
        printf("%d: [%f, %f]", i, min_bound[i], max_bound[i]);
    }
    printf("\n");
    num_features=num;
    release();
}
//------------------------------------------------------------------------------------------------
void bioinfo::read_data_to_memory(FILE *fp)
{
    char *p=nullptr;
    float std_value=0;
    char *pointer;
    int i=0,j=0, k=0, ii=0, s;
    char title_name[128];
    float value;
    char buffer[65536*4];
//    FILE *fp2 = fopen("file.txt", "w");
    int idx[]={1, 25, 26, 35, 39, 48, 49, 61, 63, 70, 74, 79, 81};
    readline(fp); //title name
    feature_count[0]=0;
    feature_count[1]=0;
//    fwrite(line, 1, strlen(line), fp2);
    while(readline(fp)!=NULL)
    {
        p=line;
        ii=0;
        i=0;
        pointer=buffer;
        origin_data.x[k]=&x_origin_space[j];
//        prob.x[k]=&x_space[j];
        while(i<num_features+1 && sscanf(p,"%f,",&value)==1)
        {
            if(i<num_features && useful_data[i]==0)
            {
                std_value=-1+2*((value-min_bound[i])/interval[i]);
//                std_value=(value-min_bound[i])/interval[i];
//                if(std_value==1 || std_value==-1 || std_value==0)
//                {
//                    sprintf(title_name, "%d:%d ", ii+1,(int)std_value);
//                }else
//                {
//                    sprintf(title_name, "%d:%f ", ii+1,std_value);
//                }
                int len=strlen(title_name);
                memcpy(pointer, title_name, len);
                pointer+=len;
//                printf("%f ", value);
                x_origin_space[j].value=std_value;
                x_origin_space[j].index=ii+1;
                j++;
                ii++;

            }else
            {
//                sprintf(title_name, "%f,", value);
                if(i<num_features)
                {
//                    x_origin_space[j].value=value;
//                    x_origin_space[j].index=i+1;
                }else
                {
//                    sprintf(title_name, "%d\n", (int)value);
                    *pointer='\n';
                    pointer++;
                    if(value==0)
                    {
                        prob.y[k]=-1;
//                        sprintf(title_name, "-1 ");
                    }else
                    {
                        prob.y[k]=1;//value;
//                        sprintf(title_name, "+1 ");
                    }
//                    prob.y[k]=value;
                    x_origin_space[j].value=value;
                    x_origin_space[j].index=-1;
                    if(value==0)
                    {
                        feature_count[0]++;
                    }else
                    {
                        feature_count[1]++;
                    }
                    j++;
//                    printf("%f \n", value);
                    
                }
            }
            x_space[j].value=-1;
            x_space[j].index=-1;

            SKIP_ELEMENT
            i++;
        }
        k++;
//        fwrite(title_name, 1, strlen(title_name), fp2);
//        fwrite(buffer, 1, pointer-buffer, fp2);
    }
//    for(int i=0;i<data_count;i++)
//    {
//        printf("%f ", prob.y[i]);
//    }
//    printf("\n");
//    printf("class 0, rate: %f", (feature_count[0]/(float)data_count));
    
//    fclose(fp2);
}
//------------------------------------------------------------------------------------------------
char* bioinfo::readline(FILE *input)
{
    int len;

    if(fgets(line,max_line_len,input) == NULL)
        return NULL;

    while(strrchr(line,'\n') == NULL)
    {
        max_line_len *= 2;
        line = (char *) realloc(line, max_line_len);
        len = (int) strlen(line);
        if(fgets(line+len,max_line_len-len,input) == NULL)
            break;
    }
    return line;
}
//------------------------------------------------------------------------------------------------
void bioinfo::release()
{
    svm_freep(max_bound);
    svm_freep(min_bound);
    svm_freep(interval);
//    svm_freep(useful_data);
}
//------------------------------------------------------------------------------------------------
void bioinfo::init_svm(float c, float gamma)
{
//    int i;
    void (*print_func)(const char*) = NULL;    // default printing to stdout

    // default values
    param.svm_type = C_SVC;
    param.kernel_type = POLY;//RBF;
    param.degree = 3;
 //   param.gamma = 0;    // 1/num_features
    param.coef0 = 0;
    param.nu = 0.5;
    param.cache_size = 100;
    param.C = 1;
    param.eps = 1e-3;
    param.p = 0.1;
    param.shrinking = 1;
    param.probability = 1;
    param.nr_weight = 0;
    param.weight_label = NULL;
    param.weight = NULL;
    cross_validation = 0;
    param.C = c;
    param.gamma = gamma;
    svm_set_print_string_function(print_func);

}
//------------------------------------------------------------------------------------------------
double bioinfo::do_cross_validation()
{
    int i;
    int total_correct[2] = {0, 0};
    int total_incorrect[2] = {0, 0};
    double total_error = 0;
    double sumv = 0, sumy = 0, sumvv = 0, sumyy = 0, sumvy = 0;
    double *target = svm_malloc(prob.l*2, double);
    double acc=0;
    double auc=0;
    svm_cross_validation(&prob,&param,nr_fold,target);
    if(param.svm_type == EPSILON_SVR ||
       param.svm_type == NU_SVR)
    {
        for(i=0;i<prob.l;i++)
        {
            double y = prob.y[i];
            double v = target[2*i];
            total_error += (v-y)*(v-y);
            sumv += v;
            sumy += y;
            sumvv += v*v;
            sumyy += y*y;
            sumvy += v*y;
        }
        printf("Cross Validation Mean squared error = %g\n",total_error/prob.l);
        printf("Cross Validation Squared correlation coefficient = %g\n",
            ((prob.l*sumvy-sumv*sumy)*(prob.l*sumvy-sumv*sumy))/
            ((prob.l*sumvv-sumv*sumv)*(prob.l*sumyy-sumy*sumy))
            );
    }
    else
    {
        int num_zeros=0;
        int *_index = svm_malloc(prob.l, int);
        int temp=0;
        //sort index
        for(int i=0;i<prob.l;i++)
        {
            _index[i]=i*2+1;
        }
        for(int i=0;i<prob.l;i++)
        {
            for(int j=i+1;j<prob.l;j++)
            {
                if(target[_index[i]]>target[_index[j]])
                {
                    temp=_index[i];
                    _index[i]=_index[j];
                    _index[j]=temp;
                }
            }
        }
        int rank_sum=0;
        int n=0;
        for(int i=0;i<prob.l;i++)
        {
//            printf("%d, %d %f %f %f \n", i, _index[i], target[_index[i]], target[_index[i]-1], prob.y[(_index[i]-1)/2]);
            if(prob.y[(_index[i]-1)/2]==1)
            {
                rank_sum+=(i+1);
                n++;
            }
        }
        auc=(rank_sum-n*(n+1)/2.0)/((double)(prob.l-n)*n);
        for(i=0;i<prob.l;i++)
        {
            if(target[2*i] == prob.y[i] )
            {
                if(prob.y[i]==-1)
                {
                    ++total_correct[0];
                }else
                {
                    ++total_correct[1];
                }
            }else
            {
                if(prob.y[i]==-1)
                {
                    ++total_incorrect[0];
                }else
                {
                    ++total_incorrect[1];
                }
            }
            if(target[i]==0)
            {
                num_zeros++;
            }
        }
//        float w=0;
        current_rate[0]=(total_correct[0]);
        current_rate[1]=(total_correct[1]);
        current_rate[2]=(total_incorrect[0]);
        current_rate[3]=(total_incorrect[1]);
        cout<<"AUC : "<<auc<<endl;
//        int ii=0;
//        for(int i=0;i<prob.l;i++)
//        {
//            printf("[%d, %d-%f]", (int)prob.y[i], (int)target[2*i], target[2*i+1]);
//            if(prob.y[i]==1)
//                ii++;
//        }
//        printf("\n class 1: %d \n", ii);
  //      acc=current_rate[0]+current_rate[1]+100*(total_correct[0]+total_correct[1])/(double)data_count;
        //acc=100*(total_correct[0]+total_correct[1])/(double)data_count;
//        acc=current_rate[0]*current_rate[1];
//        printf("Cross Validation Accuracy = %g%%, class 0, count:%d \n",acc, num_zeros);
//        printf("Cross Validation auc = %g%%, class 0, count:%d \n",auc, n);
    }
    free(target);
    return auc;
}
