//
//  bioinfo.h
//  Bioinfo_IGA
//
//  Created by user on 2023/2/1.
//

#ifndef bioinfo_h
#define bioinfo_h
#include "svm.h"

class bioinfo
{
public:
    bioinfo(char *filename, int num_folds);
    ~bioinfo();
private:
    char *line = NULL;
    int max_line_len = 1024;
    int max_index;
    int min_index;
    long int num_features = 0;
    FILE *fp, *fp_restore = NULL;
    FILE *data;
    float *max_bound, *min_bound;
    char *useful_data;
    float *interval;
    char *buffer;
    struct svm_parameter param;        // set by parse_command_line
    struct svm_problem prob;        // set by read_problem
    struct svm_problem origin_data;        // set by read_problem
    struct svm_model *model;
    struct svm_node *x_origin_space;
    struct svm_node *x_space;
    double feature_count[2];
    double current_rate[4];
    int cross_validation;
    int nr_fold;
    bool initial=false;
    int data_count=0;
private:
    void init_data_set(char *filename, int num_flods);
    char* readline(FILE *input);
    void load_training_data_set(char *filename);
    void release();
    void init_svm(float c, float gamma);
    double do_cross_validation();
    void read_data_to_memory(FILE *fp);
    void transfer_to_svm_data(individual *_individual);
    void transfer_to_svm_data2(individual *_individual);
public:
    static void bioinfo_problem_fitness_function(individual *_individual);
    int get_feature_count() {return num_features;}
};

#endif /* bioinfo_h */
