#ifndef MUTEX_H
#define MUTEX_H

#include "pthread.h"

//simply calling functions
class Mutex {
  pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
  friend class Condiction;

public:
  bool lock() { return pthread_mutex_lock(&mutex) == 0; }
  bool trylock() { return pthread_mutex_trylock(&mutex) == 0; }
  bool unlock() { return pthread_mutex_unlock(&mutex) == 0; }
  ~Mutex() {pthread_mutex_destroy(&mutex);}
};

#endif // MUTEX_H
