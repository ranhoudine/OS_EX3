//
// Created by ranho on 5/13/23.
//
#ifndef _JOBCONTEXT_H_
#define _JOBCONTEXT_H_

#include <pthread.h>
#include <vector>
#include <atomic>
#include <utility>
#include "Barrier/Barrier.h"  // todo not sure whether this is allowed but
#include "MapReduceClient.h"
#include "MapReduceFramework.h"
#include "ThreadContext.h"
#include <semaphore.h>

#define NULLIFY_MASK ((uint64_t)3 << 62)               // 11 00000 00000

using std::vector;
using std::atomic;
using std::pair;
typedef struct ThreadContext ThreadContext;
class JobContext
{
 public:
  vector<IntermediateVec> _shuffled;
  vector<ThreadContext* > _threadContexts; // todo maybe it would be better to not use pointers here, we'll see
  pthread_t *_threads; //testing here todo

  const MapReduceClient &_client;
  InputVec _inputVector;
  Barrier _barrier;
  OutputVec &_outputVec;
  int _multiThreadLevel;
  atomic<uint> _atomicIndex;
  atomic<uint> _doneCount;
  atomic<uint> _stage;
  bool _wasWaitForJobCalled;
  pthread_mutex_t *_waitForJobMutex;
  pthread_mutex_t *_getJobStateMutex;
  pthread_mutex_t *_firstThreadMutex;
  pthread_mutex_t *_reduceStageMutex; // todo added this mutex
  int _totalPairs; // todo temporary variable

  JobContext (const MapReduceClient &client,
              const InputVec &inputVec, OutputVec &outputVec,
              int multiThreadLevel);

//  JobContext &operator= (const JobContext &job);
//  JobContext (const JobContext &job);
  void runJob ();

  uint64_t getNextIndex();
  void incDoneCount();
  void incStage();
  void nullifyCounters();
  uint getStage();
  uint getDoneCount();
  uint atomicLoad(atomic<uint> atomic);
  void nextStage();

};

typedef struct ThreadContext
{
    JobContext *_jobContext;
    const int _threadID;
    IntermediateVec _intermediateVector;

    ThreadContext(JobContext* job_context, int id, IntermediateVec vec)
        : _jobContext(job_context),
          _threadID(id),
          _intermediateVector(vec)
    {}

} ThreadContext;
#endif //_JOBCONTEXT_H_
