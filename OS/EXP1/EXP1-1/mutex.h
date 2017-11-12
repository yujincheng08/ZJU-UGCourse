#ifndef MUTEX_H
#define MUTEX_H

#include "pthread.h"

class Mutex {
  pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
  friend class Condiction;

public:
  bool lock() { return pthread_mutex_lock(&mutex) == 0; }
  bool trylock() { return pthread_mutex_trylock(&mutex) == 0; }
  bool unlock() { return pthread_mutex_unlock(&mutex) == 0; }
};

#endif // MUTEX_H
