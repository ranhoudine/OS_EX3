//
// Created by ranho on 5/13/23.
//
#ifndef _JOBCONTEXT_H_
#define _JOBCONTEXT_H_

#include <pthread.h>
#include <vector>
#include <atomic>
#include <utility>
#include "Barrier/Barrier.h"
#include "MapReduceClient.h"
#include "MapReduceFramework.h"
#include <semaphore.h>


using std::vector;
using std::atomic;
using std::pair;

typedef struct ThreadContext ThreadContext;

class JobContext
{
 public:
  vector<IntermediateVec> _shuffled;
  vector<ThreadContext* > _threadContexts;
  pthread_t *_threads;

  const MapReduceClient &_client;
  InputVec _inputVector;
  OutputVec &_outputVec;

  int _multiThreadLevel;
  bool _wasWaitForJobCalled;
  int _totalPairs;

  atomic<uint> _atomicIndex;
  atomic<uint> _doneCount;
  atomic<uint> _stage;

  Barrier _barrier;
  pthread_mutex_t *_waitForJobMutex;
  pthread_mutex_t *_getJobStateMutex;
  pthread_mutex_t *_firstThreadMutex;
  pthread_mutex_t *_reduceStageMutex;

  JobContext (const MapReduceClient &client,
              const InputVec &inputVec, OutputVec &outputVec,
              int multiThreadLevel);
  ~JobContext();

  void runJob ();

  uint64_t getNextIndex();
  void incDoneCount();
  void incStage();
  void nullifyCounters();
  uint getStage();
  uint getDoneCount();
  void nextStage();

};

typedef struct ThreadContext
{
    JobContext *_jobContext;
    const int _threadID;
    IntermediateVec _intermediateVector;

    ThreadContext(JobContext* job_context, int id, IntermediateVec &vec)
        : _jobContext(job_context),
          _threadID(id),
          _intermediateVector(vec)
    {}

} ThreadContext;
#endif //_JOBCONTEXT_H_
