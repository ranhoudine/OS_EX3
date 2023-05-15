//
// Created by ranho on 5/15/23.
//

#ifndef _THREADCONTEXT_H_
#define _THREADCONTEXT_H_

#include <vector>
#include <atomic>
#include <utility>
#include <pthread.h>
#include "JobContext.h"
#include "Barrier/Barrier.h"  // todo not sure whether this is allowed but
#include "MapReduceClient.h"
#include "JobContext.h"
//using std::vector;
//using std::atomic;
//
//
//typedef struct ThreadContext
//{
//    JobContext* _jobContext;
//    pthread_t _threadID;
//    IntermediateVec _intermediateVector;
//} ThreadContext;

#endif //_THREADCONTEXT_H_
