#include "Barrier.h"
#include <cstdlib>
#include <cstdio>
#include <cassert>
#include <iostream>

Barrier::Barrier (int numThreads)
    : mutex (PTHREAD_MUTEX_INITIALIZER), cv (PTHREAD_COND_INITIALIZER), count (0), numThreads (numThreads)
{}

Barrier::~Barrier ()
{
  int error_code = pthread_mutex_destroy (&mutex);
  if (error_code != 0)
  {
    std::cout <<  "[[Barrier]] error on pthread_mutex_destroy" << std::endl;
    exit (1);
  }
  if (pthread_cond_destroy (&cv) != 0)
  {
    fprintf (stderr, "[[Barrier]] error on pthread_cond_destroy");
    exit (1);
  }
}

void Barrier::barrier ()
{
  int retval = pthread_mutex_lock (&mutex);
  if (retval != 0){
    fprintf (stderr, "[[Barrier]] error on pthread_mutex_lock");
    exit (1);
  }
  if (++count < numThreads)
  {
    if (pthread_cond_wait (&cv, &mutex) != 0)
    {
      fprintf (stderr, "[[Barrier]] error on pthread_cond_wait");
      exit (1);
    }
  }
  else
  {
    count = 0;
    if (pthread_cond_broadcast (&cv) != 0)
    {
      fprintf (stderr, "[[Barrier]] error on pthread_cond_broadcast");
      exit (1);
    }
  }
  if (pthread_mutex_unlock (&mutex) != 0)
  {
    fprintf (stderr, "[[Barrier]] error on pthread_mutex_unlock");
    exit (1);
  }
}
