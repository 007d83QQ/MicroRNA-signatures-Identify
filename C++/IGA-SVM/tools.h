//
//  tools.h
//  IGA
//
//  Created by user on 2022/12/12.
//

#ifndef tools_h
#define tools_h
#include <string.h>
#include <stdio.h>
#define MAX_OBJECTS 10
#define max(x,y) (((x)>(y))?(x):(y))
#define min(x,y) (((x)<(y))?(x):(y))
#define SKIP_TARGET\
    while(isspace(*p)) ++p;\
    while(!isspace(*p)) ++p;

#define SKIP_TITLE_ELEMENT\
    while(*p!=',' && *p!='\n') ++p;\
    ++p;

#define SKIP_ELEMENT\
    while(*p!=',') ++p;\
    ++p;

#define svm_freep(p) \
    if (p) { \
        delete p; \
        p = NULL; \
    } \
    (void)0
// Please use the freepa(T[]) to free an array, otherwise the behavior is undefined.
#define svm_freepa(pa) \
    if (pa) { \
        delete[] pa; \
        pa = NULL; \
    } \
    (void)0
#define svm_malloc(size, type) \
    (type *)malloc(sizeof(type)*size)
//#define  DEBUG_MODE
enum STOP_CONDITION
{
    STOP_CONDITION_GENERATION  = 0,
    STOP_CONDITION_EVALUATION_COUNT,
};

//int evalution_count;
#endif /* tools_h */
