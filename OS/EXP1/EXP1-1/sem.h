#ifndef SEM_H
#define SEM_H

#include <semaphore.h>

//simply calling functions
class Semaphore {
  sem_t semaphore;

public:
  Semaphore(int value = 0) { sem_init(&semaphore, 0, value); }
  bool wait() { return sem_wait(&semaphore) == 0; }
  bool signal() { return sem_post(&semaphore) == 0; }
  int getValue() {
    int value;
    sem_getvalue(&semaphore, &value);
    return value;
  }

  ~Semaphore() { sem_destroy(&semaphore); }
};

#endif // SEM_H
