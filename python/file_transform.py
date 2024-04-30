import pandas as pd
import numpy as np
from scipy.stats import ranksums





tcga=pd.read_csv("/Users/shaoxi/Downloads/brca_lasso.csv", header = 0)

tcga_col=list(tcga.columns)
ranksum=[]

for i in tcga_col:
    value1=[]
    value0=[]
    my_col = tcga[[i,"stage_hazard"]]
    
    for j in range(0,my_col.shape[0]):
        if (str(my_col.iloc[j,1]) == "1"):
            value1.append(my_col.iloc[j,0])
        else:
            value0.append(my_col.iloc[j,0])
    value0 = np.array(value0) 
    value1 = np.array(value1)
    ttt = str(ranksums(value0,value1))
    p = ttt[ttt.find("pvalue=")+len("pvalue="):-1]
    ranksum.append(p)



for i in range(0,tcga.shape[1]-1):
    a=ranksum[i]
    if float(a) > 0.1 :
        tcga=tcga.drop(tcga_col[i],axis=1)

s1="/Users/shaoxi/Downloads/brca_newranksum.csv"
s2="/Users/shaoxi/Downloads/kidney_ranksum拷貝.csv"
s3="/Users/shaoxi/Downloads/kidney_lasso.csv"
dfx=pd.read_csv(s1, header = 0)


df=pd.DataFrame(dfx.iloc[0][:])
df=df.transpose()

a=0
for i in range(0,dfx.shape[0]):
    if dfx.loc[i]["stage_hazard"] == 1:
        df.loc[df.shape[0]]=dfx.iloc[i][:]
        if a == 0:
            df.loc[df.shape[0]]=dfx.iloc[i][:]
            a=1
        else:
            a=0
        #df.loc[df.shape[0]]=dfx.iloc[i][:]
    else:
        df.loc[df.shape[0]]=dfx.iloc[i][:]
        
df=df.drop(0,axis=0)
df=df.sample(frac=1).reset_index(drop=True)
df.to_csv("brca_nr.csv",index=False)
