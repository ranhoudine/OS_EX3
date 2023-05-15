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
using std::atomic;
using std::pair;

typedef struct ThreadContext
{
    JobContext* _jobContext;
    pthread_t _threadID;
    IntermediateVec _intermediateVector;
} ThreadContext;

class JobContext
{
 public:
  vector<ThreadContext *> _threads;
  const MapReduceClient &_client;
  const InputVec &_inputVector;
  Barrier _barrier;
  OutputVec& _outputVec;
  int _multiThreadLevel;

  JobContext (const MapReduceClient &client,
              const InputVec &inputVec, OutputVec &outputVec,
              int multiThreadLevel);

  void runJob();

};

#endif //_JOBCONTEXT_H_
