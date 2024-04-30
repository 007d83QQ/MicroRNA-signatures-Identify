//
//  fitness_RandomForest.cpp
//  Bioinfo_IGA
//
//  Created by user on 2023/2/18.
//

#include "fitness_RandomForest.hpp"
#include "DecisionTree.h"
#include "Data.h"
#include "RandomForest.h"
#include <algorithm>
#include <vector>
#include "fitness_function_data.hpp"
#include "individual.hpp"
//static fitness_RandomForest *bio=nullptr;
fitness_RandomForest::fitness_RandomForest():
   trainData( *(new Data(true, 1719692)))
{
    useful_data=nullptr;
    n_fold=3;
//    bio=this;
}
fitness_RandomForest::~fitness_RandomForest()
{
    delete &trainData;
    svm_freep(useful_data);
}
void fitness_RandomForest::init(std::string filename, int flod) {
//    Data trainData(true, 1719692);
    trainData.read(filename);//"ga_coad.csv"
    n_fold=flod;
    useful_data=svm_malloc(trainData.featureSize, char);
//    RandomForest randomForest(100, "gini", "auto", -1, 2, 1, 1000000, 2);
    
//    cout<<"correct : "<<cross(trainData,5,randomForest)<<endl;
      
}
////------------------------------------------------------------------------------------------------
//void fitness_RandomForest::fitness_function(individual *_individual, void *arg) {
//    if(arg==nullptr)
//        return;
//    ((fitness_RandomForest *) arg)->transfer_to_random_forset_data(_individual);
////    bio->transfer_to_random_forset_data(_individual);
//}
//------------------------------------------------------------------------------------------------
void fitness_RandomForest::fitness_function(individual *_individual)
{
    int value=0;
    fitness_function_data *data=_individual->get_fitness_function_data();
    int index=0;
    int len;
    int loc=0;
    int dest_loc=0;
    loc=0;
    dest_loc=0;
    int real_used_count=0;
    int would_use=0;
    int treeDepth=(int)_individual->get_parameters_value(index);
    index++;
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
    int featureSize=real_used_count+10;
    Data sub_trainData(true, featureSize, featureSize);
    int ss=trainData.features.size();
//    featureSize=trainData.featureSize;
    for(int i=0;i<ss;i++)
    {
        std::vector<double> dd=trainData.features[i];
        std::vector<double> sample(featureSize, 0);
        int k=0;
        for(int j=0;j<trainData.featureSize-11;j++)
        {
            if(useful_data[j]==1)
            {
                sample[k++]=dd[j];
            }
        }
        sub_trainData.target.push_back(trainData.target[i]);
        sub_trainData.features.push_back(sample);
        sub_trainData.samplesVec.push_back(i);
    }
    sub_trainData.samplesSize=ss;
    sub_trainData.featuresVec.reserve(featureSize);
    for (int i = 0; i < featureSize; i++) { sub_trainData.featuresVec.push_back(i); }
    sub_trainData.featureSize=featureSize;
    fitness_value eval;
    
    eval.m_multi_objs[0]=cross(sub_trainData,n_fold, treeDepth);
    eval.m_multi_objs[1]=real_used_count;
    eval.m_fintess_value=100*(eval.m_multi_objs[0]-0.5)+60*(1-eval.m_multi_objs[1]/(float)(trainData.featureSize-11));
    
    _individual->m_evulation_value=eval;
    data->evalution_count++;
}
int fitness_RandomForest::get_feature_count()
{
    return trainData.featureSize-10;
}
//------------------------------------------------------------------------------------------------
double fitness_RandomForest::cross(Data ori,int _fold, int treeDepth){
    int len=ori.samplesSize;
    double k=len/float(_fold);
    vector<double> pred;
    RandomForest r1(100, "gini", "auto", treeDepth, 2, 1, 100000, 10);
    for(int i=0;i<_fold;i++){
        Data train,test;
        double start=k*i;
        double end=k*(i+1);
        for(int j=0;j<len;j++){
            if(j>=start && j<end ){
                test.features.push_back(ori.features[j]);
                test.target.push_back(ori.target[j]);
            }else{
                train.features.push_back(ori.features[j]);
                train.target.push_back(ori.target[j]);
            }
        }
        train.featureSize=train.features[0].size();
        test.featureSize=test.features[0].size();
        train.samplesSize=train.features.size();
        test.samplesSize=test.features.size();
        for(int j=0;j<train.featureSize;j++)train.featuresVec.push_back(j);
        for(int j=0;j<test.featureSize;j++)test.featuresVec.push_back(j);
        for(int j=0;j<train.samplesSize;j++)train.samplesVec.push_back(j);
        for(int j=0;j<test.samplesSize;j++)test.samplesVec.push_back(j);
        
        
   //     RandomForest r1=randomforest;
        r1.fit(train);
        auto results = r1.predictProba(test);
        
        for(int i=0;i<results.size();i++) pred.push_back(results[i]);
        
        test.target.clear();
        test.features.clear();
        test.featuresVec.clear();
        test.samplesVec.clear();
        
        train.target.clear();
        train.features.clear();
        train.featuresVec.clear();
        train.samplesVec.clear();
    }
    
//    cout<<"predict size: " << pred.size()<<endl;
    vector<pair<double,int>> sor;
    sor.resize(pred.size());
    for(int i=0;i<pred.size();i++)sor[i]=make_pair(pred[i], ori.target[i]);
    
    sort(sor.begin(),sor.end(),[](pair<double,int> a,pair<double,int> b){return a.first<b.first;});
    
    double ranksum=0;
    int M=0,N=0;
    double x=0,y=0;
    int lx=0,ly=0;
    for(lx=0;lx<sor.size()&&sor[lx].first<0.5;lx++){
        x+=(lx+1);
    }
    x/=lx;
    for(ly=lx;ly<sor.size();ly++){
        y+=(ly+1);
    }
    y/=(ly-lx);
    for(int i=0;i<pred.size();i++)
    {
        if(sor[i].second==1)
        {
            if(i<lx)ranksum+=x;
            else ranksum+=y;
            M++;
        }
    }
    N=pred.size()-M;
    
    double auc=(ranksum-M*(M+1)/2)/(M*N);
    int corr[4]={0};
    for(int i=0;i<pred.size();i++){
        if(sor[i].first>=0.5&&sor[i].second==1)corr[0]++;
        if(sor[i].first<0.5&&sor[i].second==0)corr[1]++;
        if(sor[i].first<0.5&&sor[i].second==1)corr[2]++;
        if(sor[i].first>=0.5&&sor[i].second==0)corr[3]++;
    }
    //cout<<corr[0]<<" "<<corr[1]<<" "<<corr[2]<<" "<<corr[3]<<endl;
    cout<<"AUC : "<<auc<<endl;
    
    return auc;
    
}
