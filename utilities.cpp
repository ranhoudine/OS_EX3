//
// Created by ranho on 5/13/23.
//
#ifndef UTILITIES_H

#include <algorithm>
#include <vector>
#include "ThreadContext.h"
#include "utilities.h"
#include <semaphore.h>
#include <iostream>
#include <armadillo>

using std::vector;

void *threadStartingPoint (void *threadContext)
{
  auto tContext = (ThreadContext *) threadContext;
  JobContext *jobContext = tContext->_jobContext;
  jobContext->_barrier.barrier (); // todo remove

  mapStage (tContext);
  std::sort (tContext->_intermediateVector.begin (), tContext->_intermediateVector.end (), pairComp);
  jobContext->_barrier.barrier ();

  shuffleStage (tContext);
  reduceStage (tContext);
  return nullptr;
}

void mapStage (ThreadContext *tContext)
{
  JobContext *jobContext = tContext->_jobContext;

  lock_mutex (jobContext->_firstThreadMutex);
  int oldVal = (int) jobContext->getNextIndex ();

  if (oldVal == 0)    // That way only the first Thread will change the stage.
    jobContext->incStage ();
  unlock_mutex (jobContext->_firstThreadMutex);

  while (oldVal < tContext->_jobContext->_totalPairs)
  {

    // oldVal contains the index of the pair to be processed
    K1 *key = tContext->_jobContext->_inputVector.at (oldVal).first;
    V1 *val = tContext->_jobContext->_inputVector.at (oldVal).second;
    tContext->_jobContext->_client.map (key, val, tContext);
    tContext->_jobContext->incDoneCount ();
    oldVal = (int) jobContext->getNextIndex ();
  }
}

void shuffleStage (ThreadContext *tContext)
{
  JobContext *jobContext = tContext->_jobContext;
//  sem_t sem = jobContext->_sem;
  if (tContext->_threadID == 0)
  {
    vector<IntermediateVec> unshuffled;

    jobContext->nextStage();

    for (ThreadContext *tc: jobContext->_threadContexts)
      unshuffled.push_back (tc->_intermediateVector);

    jobContext->_shuffled = shuffle (unshuffled, jobContext);

    jobContext->nextStage();

  }
  jobContext->_barrier.barrier();
}

void reduceStage (ThreadContext *tContext)
{
  JobContext *jobContext = tContext->_jobContext;

  uint64_t oldVal = jobContext->getNextIndex ();

  while (oldVal < jobContext->_shuffled.size ())
  {
    IntermediateVec *curVec = &(jobContext->_shuffled.at (oldVal));
    jobContext->_client.reduce (curVec, tContext);
    tContext->_jobContext->incDoneCount ();
    oldVal = jobContext->getNextIndex ();
  }
}

bool pairComp (IntermediatePair &a, IntermediatePair &b)    // Used in sort
{
  return a.first < b.first;
}

vector<IntermediateVec> shuffle (vector<IntermediateVec> &intermediates, JobContext *jobContext)
{
  vector<IntermediateVec> queue;
  int v;
  eraseEmptyVectors (intermediates);
  while (!intermediates.empty ())
  {
    IntermediatePair currentPair = popBiggestPair (intermediates);
    v = findVectorByKey (queue, currentPair.first);
    if (v == -1)
    {
      IntermediateVec newVec;
      newVec.push_back (currentPair);
      queue.push_back (newVec);
    }
    else
      queue[v].push_back (currentPair);

    jobContext->incDoneCount ();
    jobContext->getNextIndex ();
  }
  return queue;
}

IntermediatePair popBiggestPair (vector<IntermediateVec> &intermediates)
{

  K2 *maxK = intermediates.at (0).back ().first;
  int maxVec = 0;
  K2 *curK;
  for (int i = 1; i < intermediates.size (); ++i)
  {
    curK = intermediates.at (i).back ().first;
    if (*maxK < *curK)
    {
      maxK = curK;
      maxVec = i;
    }
  }
  IntermediatePair maxPair = intermediates.at (maxVec).back ();
  intermediates.at (maxVec).pop_back ();
  if (intermediates.at (maxVec).empty ())
    intermediates.erase (intermediates.begin () + maxVec);
  return maxPair;
}

int findVectorByKey (vector<IntermediateVec> queue, K2 *key)
{
  K2 *curKey;
  for (int i = 0; i < queue.size (); ++i)
  {
    curKey = queue[i][0].first;
    if (!(*key < *curKey) && !(*curKey < *key))
      return i;
  }
  return -1;
}

void eraseEmptyVectors (vector<IntermediateVec> &vectors)
{
  auto it = vectors.begin ();
  while (it != vectors.end ())
  {
    if (it->empty ())
    {
      it = vectors.erase (it);
    }
    else
    {
      ++it;
    }
  }
}

void lock_mutex (pthread_mutex_t *mutex)
{
  if (pthread_mutex_lock (mutex) != 0)
  {
    std::cerr << "system error: on pthread_mutex_lock" << std::endl;
    exit (1);
  }
}

void unlock_mutex (pthread_mutex_t *mutex)
{
  if (pthread_mutex_unlock (mutex) != 0)
  {
    std::cerr << "system error: on pthread_mutex_unlock" << std::endl;
    exit (1);
  }
}



#endif