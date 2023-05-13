//
// Created by ranho on 5/13/23.
//

#include "JobContext.h"
#include <vector>
using std::vector;

JobContext::JobContext(const MapReduceClient& client,
           const InputVec& inputVec, OutputVec& outputVec,
           int multiThreadLevel)
           : _client(client), _inputVec(inputVec), _outputVec(outputVec),
           _multiThreadLevel(multiThreadLevel)
{
//  _client = client;
//  _inputVec = inputVec;
//  _outputVec = outputVec;
//  _multiThreadLevel = multiThreadLevel;
  _threads = new ThreadContext[_multiThreadLevel];

}

