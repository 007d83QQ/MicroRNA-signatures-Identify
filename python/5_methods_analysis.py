import pandas as pd
import numpy as np
from sklearn.model_selection import train_test_split
from sklearn.tree import DecisionTreeClassifier
from sklearn import tree
from sklearn.metrics import classification_report,confusion_matrix
from sklearn.metrics import RocCurveDisplay
import matplotlib.pyplot as plt
from imblearn.over_sampling import SMOTE
from sklearn.ensemble import RandomForestClassifier
from sklearn.svm import SVC
from imblearn.over_sampling import BorderlineSMOTE
from sklearn.discriminant_analysis import LinearDiscriminantAnalysis
from sklearn.neighbors import KNeighborsClassifier
from sklearn.metrics import roc_curve, roc_auc_score, auc


s1="/Users/shaoxi/Downloads/tcga/test_kidney.csv"
s2="/Users/shaoxi/Downloads/brca_nr.csv"
s3="/Users/shaoxi/Downloads/brca_newlasso.csv"
s4="/Users/shaoxi/Downloads/brca_garesult.csv"


dfx=pd.read_csv(s2, header = 0)
df=pd.DataFrame(dfx.iloc[0][:])
df=df.transpose()

a=0
for i in range(0,dfx.shape[0]):
    if dfx.loc[i]["stage_hazard"] == 1:
        df.loc[df.shape[0]]=dfx.iloc[i][:]
        if a == 0:
            #df.loc[df.shape[0]]=dfx.iloc[i][:]
            a=1
        else:
            a=0
        #df.loc[df.shape[0]]=dfx.iloc[i][:]
    else:
        df.loc[df.shape[0]]=dfx.iloc[i][:]
        
df=df.drop(0,axis=0)
df=df.sample(frac=1).reset_index(drop=True)
#df=df.sample(frac=1).reset_index(drop=True)
print(df.shape)

x = df.drop(columns='stage_hazard')
y = df['stage_hazard']

count=0
for i in range(0,df.shape[0]):
    if y.iloc[i]==1:
        count=count+1

print(count)

#X_train, y_train = SMOTE(random_state=777).fit_resample(X_train, y_train)
#x,y = BorderlineSMOTE(random_state=777).fit_resample(x,y)



way=[]
way.append(('DT',DecisionTreeClassifier()))
way.append(('RF',RandomForestClassifier(n_estimators=100, random_state=1)))
way.append(('SVM', SVC(gamma='auto')))
way.append(('LDA', LinearDiscriminantAnalysis()))
way.append(('kNN', KNeighborsClassifier()))


#print(confusion_matrix(y_test,clf.predict(X_test)))

len=x.shape[0]
len=len/20

for i in way:
    pr=np.empty(shape=0)
    for j in range(0,20):
        xtrain=x.iloc[0:int(len*j)][:]
        xtrain=pd.concat([xtrain,x.iloc[int(len*(j+1)):int(len*20)][:]],axis=0)
        ytrain=y.iloc[0:int(len*j)][:]
        ytrain=pd.concat([ytrain,y.iloc[int(len*(j+1)):int(len*20)][:]],axis=0)
        xtest=x.iloc[int(len*j):int(len*(j+1))][:]
        pred=i[1].fit(xtrain,ytrain).predict(xtest)
        if j != 0:
            pr=np.concatenate([pr,pred])
        else:
            pr=pred
    
    auc=roc_auc_score(y,pr)
    print(str(i[0])," ",auc)

