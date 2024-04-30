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
## Test Data
The test data is a data set that has been crawled, processed and processed. It includes a total of five pieces of data, coad (colorectal cancer)、brca (breast cancer)、luad (lung cancer)、kidney (kidney cancer) and hnsc (head and neck cancer
), and is provided to users as sample data.
