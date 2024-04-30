import pandas as pd
import numpy as np
from sklearn.preprocessing import StandardScaler
from sklearn.pipeline import Pipeline
from sklearn.model_selection import train_test_split, GridSearchCV
from sklearn.linear_model import Lasso
from imblearn.over_sampling import SMOTE
from imblearn.over_sampling import BorderlineSMOTE

df = pd.read_csv("/Users/shaoxi/Downloads/iga_hnsc.csv", header = 0)

x = df.drop(columns='stage_hazard')
y = df['stage_hazard']
#X_train, X_test, y_train, y_test = train_test_split(x, y, test_size=0.3,random_state=11)
x,y = BorderlineSMOTE(random_state=456).fit_resample(x,y)
scaler = StandardScaler().fit(x)
x = scaler.transform(x)

features = np.array(df.columns)[0:171]

pipeline = Pipeline([
                     ('scaler',StandardScaler()),
                     ('model',Lasso())
])

search = GridSearchCV(pipeline,
                      {'model__alpha':np.arange(0.01,10,0.01)},
                      cv = 5, scoring="neg_mean_squared_error",verbose=3
                      )

search.fit(x,y)
search.best_params_
coefficients = search.best_estimator_.named_steps['model'].coef_

importance = np.abs(coefficients)
print(importance)
print(np.array(features)[importance > 0])
for i in range (0,df.shape[1]-1):
    if importance[i]==0:
        df=df.drop(labels=features[i],axis=1)

df.to_csv("hnsc_newlasso.csv",index=False)
