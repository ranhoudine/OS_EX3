//
// Created by ranho on 5/13/23.
//

#ifndef _UTILITIES_H_
#define _UTILITIES_H_
#include <vector>
#include "JobContext.h"
#include "MapReduceFramework.h"

void* threadStartingPoint(void* context);

bool pairComp(IntermediatePair a, IntermediatePair b);
void sortIntermediateVec(IntermediateVec* vector);
IntermediatePair popBiggest(vector<IntermediateVec> intermediates);
int findVectorByKey(vector<IntermediateVec> queue, K2* key);
void threadMap(ThreadContext* context);
vector<IntermediateVec> shuffle (vector<IntermediateVec> intermediates, JobContext *jobContext);
#endif //_UTILITIES_H_
