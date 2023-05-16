//
// Created by ranho on 5/13/23.
//
#include "MapReduceFramework.h"
#include "utilities.h"
#include "pthread.h"
#include "Barrier/Barrier.h"
#include <atomic>
#include <utility>
#include "JobContext.h"

// I think the JobContext should be here as a struct, and it doesn't have to
// have any functions. From what I understand we need to initialize a bunch
// of threads, with each of them receiving the map function as the starting
// function, and as an argument the split input vector
using std::atomic;
using std::vector;
using std::pair;

void emit2 (K2 *key, V2 *value, void *context)
{
  return;
}
void emit3 (K3 *key, V3 *value, void *context)
{
  OutputVec *outputVec = (OutputVec *) context;
  outputVec->push_back (OutputPair (key, value));
}

JobHandle startMapReduceJob (const MapReduceClient &client,
                             const InputVec &inputVec, OutputVec &outputVec,
                             int multiThreadLevel)
{
  JobContext jobContext(client, inputVec, outputVec, multiThreadLevel);

}

void waitForJob (JobHandle job)
{
  return;
}
void getJobState (JobHandle job, JobState *state)
{
  auto jobContext = (JobContext *) job;
  int numOfPairs = (int) jobContext->_inputVector.size ();
  state->stage = (stage_t) (jobContext->_atomicIndex >> 62);
  int counter = (int) (jobContext->_atomicIndex % 1 << 31);
  float percentage = 0; // todo we need to make sure the percentage is 0 during the undefined stage

  if (state->stage != UNDEFINED_STAGE)
  {
    percentage = (float) (100 * counter) / (float) numOfPairs;
  }
  state->percentage = percentage;
}
void closeJobHandle (JobHandle job)
{
  // todo here is where we need to deallocate the memory of the various treadContexts, which might imply that we need the rule of three
  JobContext* jobContext = (JobContext*) job;
  jobContext->~JobContext();
}


