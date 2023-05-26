//
// Created by ranho on 5/13/23.
//

#ifndef _UTILITIES_H_
#define _UTILITIES_H_

#include <vector>
#include "JobContext.h"
#include "MapReduceFramework.h"

void *threadStartingPoint (void *context);

void mapStage (ThreadContext *tContext);
void shuffleStage (ThreadContext *tContext);
void reduceStage (ThreadContext *tContext);

bool pairComp (IntermediatePair &a, IntermediatePair &b);
IntermediatePair popBiggestPair (vector<IntermediateVec> &intermediates);
int findVectorByKey (vector<IntermediateVec> &queue, K2 *key);
vector<IntermediateVec> shuffle (vector<IntermediateVec> &intermediates, JobContext *jobContext);
void eraseEmptyVectors (vector<IntermediateVec> &vectors);

void lock_mutex (pthread_mutex_t *mutex);
void unlock_mutex (pthread_mutex_t *mutex);

#endif //_UTILITIES_H_
