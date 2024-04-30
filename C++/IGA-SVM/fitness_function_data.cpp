//
//  fitness_function_data.cpp
//  Bioinfo_IGA
//
//  Created by user on 2023/1/31.
//

#include "fitness_function_data.hpp"
fitness_function_data::fitness_function_data(int _parameters_count, float _lower_bound, float _upper_bound, int _encode_bits_count, int num_objectives, bool _want_bigger_fitness_value, void (*fitness_fun)(individual *_individual))
{
    init( _parameters_count, _lower_bound, _upper_bound, _encode_bits_count, num_objectives, _want_bigger_fitness_value, fitness_fun);
}
fitness_function_data::~fitness_function_data()
{
    svm_freep(encode_bit_length);//_encode_bits_count;
    svm_freep(lower_bound);
    svm_freep(upper_bound);
    svm_freep(interval);
    svm_freep(encode_value_range);
}


void fitness_function_data::init(int _parameters_count, float _lower_bound, float _upper_bound, int _encode_bits_count, int num_objectives, bool _want_bigger_fitness_value, void (*fitness_fun)(individual *_individual))
{
    parameter_count=_parameters_count;
    encode_bit_length=svm_malloc(parameter_count, int);//_encode_bits_count;
    lower_bound=svm_malloc(parameter_count, float);
    upper_bound=svm_malloc(parameter_count, float);
    interval=svm_malloc(parameter_count, float);
    encode_value_range=svm_malloc(parameter_count, int);
    for(int i=0;i<parameter_count;i++)
    {
        encode_bit_length[i]=_encode_bits_count;
        lower_bound[i]=_lower_bound;
        upper_bound[i]=_upper_bound;
        interval[i]=_upper_bound-_lower_bound;
        encode_value_range[i]=1 << _encode_bits_count;
    }
//       interval=_upper_bound-_lower_bound;
//        encode_value_range=1 << _encode_bits_count;
    want_bigger_fitness_value=_want_bigger_fitness_value;
    evalute=fitness_fun;
    evalution_count=0;
    m_num_objectives=num_objectives;
}
//float fitness_function_data::get_parameters_value(int index, int parameter)
//{
//    float value=0;
//    value=lower_bound[index]+ interval[index] * (parameter/(double)encode_value_range[index]);
//    return value;
//}
void fitness_function_data::change_bounded_data(int index, float _lower_bound, float _upper_bound, int _encode_bits_count)
{
    lower_bound[index]=_lower_bound;
    upper_bound[index]= _upper_bound;
    interval[index]=_upper_bound-_lower_bound;
    encode_bit_length[index]=_encode_bits_count;
    encode_value_range[index]=1 << _encode_bits_count;
}
