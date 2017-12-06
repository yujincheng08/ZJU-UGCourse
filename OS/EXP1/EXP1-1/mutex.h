#ifndef MUTEX_H
#define MUTEX_H

#include "pthread.h"

// Exclusive lock
class Mutex {
  // Simple initialization
  pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
  // Condition variable need to a mutex to implement.
  friend class Condiction;

public:
  // Simply calls pthread functions
  bool lock() { return pthread_mutex_lock(&mutex) == 0; }
  bool trylock() { return pthread_mutex_trylock(&mutex) == 0; }
  bool unlock() { return pthread_mutex_unlock(&mutex) == 0; }
  // Destroy before destruction
  ~Mutex() {pthread_mutex_destroy(&mutex);}
};

#endif // MUTEX_H
