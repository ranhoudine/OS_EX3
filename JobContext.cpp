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
    : _client (client), _inputVector (inputVec), _outputVec (outputVec),
      _multiThreadLevel (multiThreadLevel), _atomicIndex (0), _barrier
      (multiThreadLevel), _state ()
{
  // these two lines might be better someplace else
  this->_state.percentage = 0;
  this->_state.stage = UNDEFINED_STAGE;
}

