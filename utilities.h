//
// Created by ranho on 5/13/23.
//

#ifndef _UTILITIES_H_
#define _UTILITIES_H_
#include <vector>
#include "JobContext.h"
void* threadStartingPoint(void* context);

void createThreads(JobContext jobContext);
#endif //_UTILITIES_H_
