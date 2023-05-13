//
// Created by ranho on 5/13/23.
//

#ifndef _JOBCONTEXT_H_
#define _JOBCONTEXT_H_
#include <pthread.h>
#include "Barrier/Barrier.h"  // todo not sure whether this is allowed but
#include <vector>
#include "MapReduceClient.h"
using std::vector;
// we'll tackle that later
typedef struct ThreadContext
{
    int threadID;
    Barrier *barrier;
} ThreadContext;

class JobContext
{
  ThreadContext[] _threads;
  const MapReduceClient &_client;
  const InputVec &_inputVec;
  OutputVec &_outputVec;
  int _multiThreadLevel;

  JobContext (const MapReduceClient &client,
              const InputVec &inputVec, OutputVec &outputVec,
              int multiThreadLevel);
};

#endif //_JOBCONTEXT_H_