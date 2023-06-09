//
// Created by ranho on 5/13/23.
//

#include <vector>
#include <pthread.h>
#include <atomic>

#include "JobContext.h"
#include "utilities.h"
#include <cstdlib>
#include <iostream>

using std::vector;
using std::atomic;

JobContext::JobContext(const MapReduceClient &client,
                       const InputVec &inputVec, OutputVec &outputVec,
                       int multiThreadLevel)
        : _threads(nullptr),
          _client(client),
          _inputVector(inputVec),
          _outputVec(outputVec),
          _multiThreadLevel(multiThreadLevel),
          _wasWaitForJobCalled(false),
          _totalPairs((int) inputVec.size()),
          _atomicIndex(0),   // Meaning stage = UNDEFINED and Counter = 0
          _doneCount(0),
          _stage(0),
          _barrier(multiThreadLevel),
          _waitForJobMutex(nullptr), _getJobStateMutex(nullptr),
          _firstThreadMutex(nullptr), _reduceStageMutex(nullptr)
          {

    vector<IntermediateVec> shuffled;
    this->_shuffled = shuffled;
    _threads = new pthread_t[_multiThreadLevel];

    _waitForJobMutex = new pthread_mutex_t;
    _getJobStateMutex = new pthread_mutex_t;
    _firstThreadMutex = new pthread_mutex_t;
    _reduceStageMutex = new pthread_mutex_t;
    pthread_mutex_init(_waitForJobMutex, NULL);
    pthread_mutex_init(_getJobStateMutex, NULL);
    pthread_mutex_init(_firstThreadMutex, NULL);
    pthread_mutex_init(_reduceStageMutex, NULL);

}

JobContext::~JobContext() {
    // Clean up resources
    for (auto *thread: _threadContexts) {
        delete thread;
    }
    delete[] _threads;
    delete _waitForJobMutex;
    delete _getJobStateMutex;
    delete _firstThreadMutex;
    delete _reduceStageMutex;
}

void JobContext::runJob() {
    // Thread Context Creation

    for (pthread_t i = 0; i < (pthread_t) _multiThreadLevel; ++i) {
        IntermediateVec v;

        auto context = new ThreadContext(this, (int) i, v);
        _threadContexts.push_back(context);
        int error_code = pthread_create(_threads + i, NULL, threadStartingPoint, _threadContexts.at(i));
        if (error_code != 0) {
            std::cout << "system error: pthread_create has failed." << std::endl;
            exit(1);
        }
    }
}

// ----------- Counter Methods -------------------

/***
 * @returns the index of the next pair to map and increments the index
 * ***/
uint64_t JobContext::getNextIndex() {
    return _atomicIndex++;
}

/***
 * increments the done pairs counter by one
 */
void JobContext::incDoneCount() {
    ++_doneCount;
}

/***
 * increments the job state counter by one
 */
void JobContext::incStage() {
    ++_stage;
}

/***
 * sets the done pairs counter and pair index to zero
 */
void JobContext::nullifyCounters() {
    _atomicIndex = 0;
    _doneCount = 0;
}

/***
 * returns the job state as follows:
 * 00 - UNDEFINED STAGE
 * 01 - MAP STAGE
 * 10 - SHUFFLE STAGE
 * 11 - REDUCE STAGE
 * @return two bits representing the job state
 */
uint JobContext::getStage() {
    return (uint) _stage.load();
}

/***
 * @returns the number of pairs that have been mapped
 */
uint JobContext::getDoneCount() {
    return _doneCount.load();
}


void JobContext::nextStage() {
    lock_mutex(_getJobStateMutex);
    incStage();
    nullifyCounters();
    unlock_mutex(_getJobStateMutex);
}




