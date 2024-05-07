# Identifying microRNA signatures and predicting the risk of cancer patients by using machine learning technology  
MicroRNA is closely related to cancer, but due to its large number of variables, small amount of data, and serious data imbalance, it is difficult for general methods to handle this type of data.
This project will focus on processing this material using a variety of different feature selection methods, including both existing and original treatments.  
**Details are as follows:**
* Existing methods
  - Ranksum-test
  - Lasso regression
* Original methods
  - IGA-SVM
  - IGA-RandomForest
## Usage
* `R` code for web crawler. This code can crawl cancer data from GDC.  
* `Python` code for data preprocessing and feature selection(including Ranksum-test and Lasso Regression). Edit file paths for code to process and analyze data.
* `C++` code includes two types of original feature selection methods:IGA-SVM(Combinig Intellegence Genetic Algorithm with SVM algorithm) and IGA-RandomForest
(Combinig Intellegence Genetic Algorithm with Random Forest algorithm).Edit file paths for code to process and analyze data.This may take a longer time.
## Requirements
* C++ Complier
* Python Complier
* numpy
* sklearn
* pandas
* scipy
## Project Details
This study employs various approaches to engineer microRNA feature selection, aiming to identify the optimal method. In addition to applying two existing methodologies, Rank-sum test and Lasso Regression, this project modifies the feature selection process of genetic algorithms. The following outlines the specifics:

The figure below illustrates the interaction between the evolutionary mechanism and the evaluation function of genetic algorithms. In this study, the genetic algorithm is employed to select partial variables from the dataset and adjust the parameters of the evaluation function according to the evolutionary mechanism. The adjusted data is then inputted into the evaluation function. Within the evaluation function, a five-fold cross-validation is conducted using the algorithm, and an adaptive score is calculated based on the fitness function. The genetic algorithm adjusts its evolutionary direction based on the fitness score of the evaluation function, thus iteratively seeking the optimal solution.

![image](https://github.com/007d83QQ/MicroRNA-signatures-Identify/blob/main/github/fitness-function.png)



This study will adjust the collaborative operation of the fitness function with genetic algorithms, as well as different classification algorithms, to derive more accurate gene selection results. The details of the algorithmic modifications are as follows:

### 1. Encoding of Chromosomes:  
Given the vast number of features in biological data, we represent them using the binary encoding of 1s and 0s in genetic algorithms, where 1 indicates the inclusion of a feature and 0 indicates its exclusion. Therefore, if there are 200 features, the chromosome length of an individual in the genetic algorithm consists of a binary string with 200 1/0 combinations. Additionally, for compatibility with genetic algorithms, the evaluation function utilized in this experiment is SVM and random forest, requiring the following preprocessing steps to achieve more precise modeling.
#### Encoding for SVM:  
To facilitate the smooth operation of the SVM algorithm, the encoding in genetic algorithms incorporates the modeling parameters of SVM: C and gamma. This enables it to autonomously adjust and search for the optimal values of C and gamma. In this study, both values are integrated into the encoding of individuals, as follows:

|c|Y|1|0|1|0|1|1|1|0|1|1|1|1|1|
|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|

#### Encoding for Random Forest:
For random forests, the genetic algorithm can control the parameter of maximum tree depth (D). Therefore, it is also integrated into the encoding of genetic algorithm individuals, as follows:

|D|1|1|0|1|1|1|1|1|0|1|0|1|1|1|
|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|

### 2. Fitness Function Design:
When comparing genetic algorithms using different evaluation methods, we aim to achieve two objectives simultaneously: high AUC value (indicating high model evaluation capability) and the use of fewer features (for ease of subsequent research). To accomplish this, we comprehensively compare the number of features selected by the genetic algorithm and the calculated AUC value after modeling with the selected feature data, which serves as the fitness value for the genetic algorithm. The formula for calculating the fitness value is as follows:  


*`Fitness Value = w1 * (AUC - 0.5) + w2 * (1 - Selected Feature Count / Total Feature Count)`*  


Where w1 and w2 are weights. In our experiments, we use w1 = 100 and w2 = 50.

## Test Data
The test data is a data set that has been crawled, processed and processed. It includes a total of five pieces of data, coad (colorectal cancer)、brca (breast cancer)、luad (lung cancer)、kidney (kidney cancer) and hnsc (head and neck cancer
), and is provided to users as sample data.

## DEMO


https://github.com/007d83QQ/MicroRNA-signatures-Identify/assets/109134845/23a92165-6bd9-43f6-8578-16f39e20d6bf



