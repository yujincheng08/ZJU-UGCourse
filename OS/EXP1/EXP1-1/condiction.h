#ifndef CONDICTION_H
#define CONDICTION_H

#include "mutex.h"
#include <pthread.h>

class Condiction {
  pthread_cond_t cond = PTHREAD_COND_INITIALIZER;

public:
  void wait(Mutex &mutex) { pthread_cond_wait(&cond, &mutex.mutex); }
  void signal() { pthread_cond_signal(&cond); }
  void broadcast() { pthread_cond_broadcast(&cond); }
  ~Condiction() { pthread_cond_destroy(&cond); }
};

#endif // CONDICTION_H
