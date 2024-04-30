import numpy as np
import pandas as pd
from scipy.stats import ranksums

tcga=pd.read_csv(r'/Users/shaoxi/Downloads/tcga/test_kidney.csv')
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


#print(ranksum)
for i in range(0,tcga.shape[1]-1):
    a=ranksum[i]
    if float(a) > 0.01 :
        tcga=tcga.drop(tcga_col[i],axis=1)

tcga.to_csv("kidney_newranksum.csv",index=False)
    