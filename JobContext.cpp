//
// Created by ranho on 5/13/23.
//

#include "JobContext.h"
#include "utilities.h"

using std::vector;
using std::atomic;

JobContext::JobContext (const MapReduceClient &client,
                        const InputVec &inputVec, OutputVec &outputVec,
                        int multiThreadLevel)
    : _client (client),
      _inputVector (inputVec),
      _outputVec (outputVec),
      _multiThreadLevel (multiThreadLevel),
      _atomicIndex (0),   // Meaning stage = UNDEFINED and Counter = 0
      _barrier (multiThreadLevel), _wasWaitForJobCalled(false)
{

  vector<IntermediateVec> shuffled;
  this->_shuffled = shuffled;
//  _sem = new sem_t; // Declare the semaphore variable
//  if (_sem == nullptr)
//  {
//    std::cerr << "Memory allocation failure" << std::endl;
//    exit(1);
//  }
  int initialValue = 0;  // Initial value for the semaphore

  if (sem_init (&_sem, 0, initialValue) != 0)
  {
    std::cerr << "Semaphore initialization failed" << std::endl;
    exit (1);
  }
}

