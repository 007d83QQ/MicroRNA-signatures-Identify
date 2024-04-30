if (!requireNamespace("BiocManager", quietly = TRUE))
  install.packages("BiocManager")
BiocManager::install("TCGAbiolinks")
library(TCGAbiolinks)
library(openxlsx)
query.exp.hg38<- GDCquery(
  project = "TCGA-COAD",
  experimental.strategy = "miRNA-Seq",
  data.category = "Transcriptome Profiling", 
  data.type = "miRNA Expression Quantification",
  workflow.type= "BCGSC miRNA Profiling"
)

query.exp.hg38.2=query.exp.hg38
tmp=query.exp.hg38.2$results[[1]]
tmp=tmp[which(!duplicated(tmp$cases)),]
query.exp.hg38.2$results[[1]]=tmp

GDCdownload(query.exp.hg38.2)
query.exp.hg38.2 <- GDCprepare(query.exp.hg38.2)
test=data.frame(query.exp.hg38.2)
write.csv(test,file = "C:/Users/user/OneDrive/桌面/folder/miRNA/TCGA-COAD.csv")