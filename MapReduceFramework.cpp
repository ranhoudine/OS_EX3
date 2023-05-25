//
// Created by ranho on 5/13/23.
//
#include "MapReduceFramework.h"
#include "pthread.h"
#include <atomic>
#include <utility>
#include "JobContext.h"
#include "utilities.h"
// I think the JobContext should be here as a struct, and it doesn't have to
// have any functions. From what I understand we need to initialize a bunch
// of threads, with each of them receiving the map function as the starting
// function, and as an argument the split input vector
#include <iostream>

using std::atomic;
using std::vector;
using std::pair;

void emit2 (K2 *key, V2 *value, void *context)
{
  auto tContext = static_cast<ThreadContext *>(context);
  tContext->_intermediateVector.push_back (IntermediatePair (key, value));
}
void emit3 (K3 *key, V3 *value, void *context)
{
  auto tContext = static_cast<ThreadContext *>(context);
  tContext->_jobContext->_outputVec.push_back (OutputPair (key, value));
}

JobHandle startMapReduceJob (const MapReduceClient &client,
                             const InputVec &inputVec, OutputVec &outputVec,
                             int multiThreadLevel)
{
  auto *jobContext = new JobContext (client, inputVec, outputVec, multiThreadLevel);
  jobContext->runJob ();
  return static_cast<JobHandle>(jobContext);
}

void waitForJob (JobHandle job)
{
  auto jobContext = static_cast<JobContext *>(job);

  lock_mutex (jobContext->_waitForJobMutex);
  if (jobContext->_wasWaitForJobCalled)
  {
    unlock_mutex (jobContext->_waitForJobMutex);
    return;
  }

  jobContext->_wasWaitForJobCalled = true;
  for (int i = 0; i < jobContext->_multiThreadLevel; ++i)
  {

    int a = pthread_join (jobContext->_threads[i], NULL);
    if (a!= 0)
    {
      std::cerr << "system error: pthread_join failed." << std::endl;
      exit (1);
    }
  }
  unlock_mutex (jobContext->_waitForJobMutex);
}

void getJobState (JobHandle job, JobState *state)
{
  auto jobContext = static_cast<JobContext *>(job);

  lock_mutex (jobContext->_getJobStateMutex);
  uint numOfPairs = (uint) jobContext->_inputVector.size ();
  uint numOfVectors = (uint) jobContext->_shuffled.size ();
//  uint64_t counter = jobContext->_atomicIndex;
  state->stage = (stage_t) jobContext->getStage ();
  uint64_t doneCount = jobContext->getDoneCount ();

//  uint doneCount = (uint) (jobContext->_atomicIndex >> 31) % (1 << 31);
  float percentage = 0; // todo we need to make sure the percentage is 0 during the undefined stage

  if (state->stage == MAP_STAGE)
  {
    percentage = (float) (100 * doneCount) / (float) numOfPairs;
  }
  if (state->stage == SHUFFLE_STAGE)
  {
    uint numOfPairsStage2 = 0;

    for (auto tc: jobContext->_threadContexts)
      numOfPairsStage2 += tc->_intermediateVector.size ();

    percentage = (float) (100 * doneCount) / (float) numOfPairsStage2;

  }

  if (state->stage == REDUCE_STAGE)
  {
    if (numOfVectors == 0)
      percentage = 0;
    else
      percentage = (float) (100 * doneCount) / (float) numOfVectors;
    if (percentage == 100)
    {
      state->percentage = percentage;
      unlock_mutex (jobContext->_getJobStateMutex);
      return;
    }
  }
  state->percentage = percentage;
  unlock_mutex (jobContext->_getJobStateMutex);
}

void closeJobHandle (JobHandle job)
{
  JobState currentState;
  getJobState (job, &currentState);
  if (currentState.stage == REDUCE_STAGE and currentState.percentage == 100)
  {
    auto jobContext = static_cast<JobContext *>(job);
    sem_destroy (&(jobContext->_sem));
  }
  else
  {
    waitForJob (job);
    auto jobContext = static_cast<JobContext *>(job);
    sem_destroy (&(jobContext->_sem));
  }
}




