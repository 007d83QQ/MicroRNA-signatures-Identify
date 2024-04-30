//
//  fitness_RandomForest.hpp
//  Bioinfo_IGA
//
//  Created by user on 2023/2/18.
//

#ifndef fitness_RandomForest_hpp
#define fitness_RandomForest_hpp

#include <stdio.h>
#include <string>
#include "interface_fitness.hpp"
class Data;
class RandomForest;
class individual;

class fitness_RandomForest:interface_fitness
{
public:
    fitness_RandomForest();
    ~fitness_RandomForest();
public:
 //   static void fitness_function(individual *_individual, void *arg=nullptr);
    void fitness_function(individual *_individual);
//    void transfer_to_random_forset_data(individual *_individual);
public:
    void init(std::string filename, int flod);
    double cross(Data ori,int n_fold, int treeDepth);
    int get_feature_count();
private:
//    RandomForest &randomForest;
    Data &trainData;
    char *useful_data;
    int n_fold;
};
#endif /* fitness_RandomForest_hpp */
