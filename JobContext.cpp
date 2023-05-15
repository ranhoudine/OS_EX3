//
// Created by ranho on 5/13/23.
//

#include "JobContext.h"
#include "utilities.h"

using std::vector;
using std::atomic;

JobContext::JobContext (const MapReduceClient *client,
                        const InputVec *inputVec, OutputVec *outputVec,
                        int multiThreadLevel)
    : _client (client), _inputVec (inputVec), _outputVec (outputVec),
      _multiThreadLevel (multiThreadLevel)
{
  initializeJobContext ();
}

