//
// Created by ranho on 5/15/23.
//

#ifndef _THREADCONTEXT_H_
#define _THREADCONTEXT_H_

#include <vector>
#include <atomic>
#include <utility>
#include <pthread.h>

#include "Barrier/Barrier.h"  // todo not sure whether this is allowed but
#include "MapReduceClient.h"

using std::pair;
using std::vector;
using std::atomic;

typedef vector<pair<K2 *, V2 *>> IntermediateVector;
typedef vector<pair<K1 *, V1 *>> InputVector;
typedef struct ThreadContext
{
    pthread_t threadID;
    Barrier *barrier;
    InputVector *inputVector;
    IntermediateVector intermediateVector;
    atomic<int> *atomicIndex;
} ThreadContext;

#endif //_THREADCONTEXT_H_
