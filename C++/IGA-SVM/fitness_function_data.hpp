//
//  fitness_function_data.hpp
//  Bioinfo_IGA
//
//  Created by user on 2023/1/31.
//

#ifndef fitness_function_data_hpp
#define fitness_function_data_hpp

#include <stdlib.h>
#include "tools.h"
class individual;
class fitness_function_data
{
public:
    fitness_function_data(int _parameters_count, float _lower_bound, float _upper_bound, int _encode_bits_count, int num_objectives, bool _want_bigger_fitness_value, void (*fitness_fun)(individual *_individual));
    ~fitness_function_data();
public:
    int evalution_count;
    int parameter_count;
    float *upper_bound;
    float *lower_bound;
    int *encode_bit_length;
    float *interval=0;
    int *encode_value_range;
    bool want_bigger_fitness_value=true;
    void (*evalute)(individual *_individual)=nullptr;
    int m_num_objectives;
public:
    void init(int _parameters_count, float _lower_bound, float _upper_bound, int _encode_bits_count, int num_objectives, bool _want_bigger_fitness_value, void (*fitness_fun)(individual *_individual));
//    float get_parameters_value(int index, int parameter);
    void change_bounded_data(int index, float _lower_bound, float _upper_bound, int _encode_bits_count);
   
};
#endif /* fitness_function_data_hpp */
