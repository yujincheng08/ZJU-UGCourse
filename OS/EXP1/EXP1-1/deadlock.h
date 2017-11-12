#ifndef DEADLOCK_H
#define DEADLOCK_H

#include "mutex.h"
#include "sem.h"
#include "thread.h"
#include <queue>

class Car;

class Deadlock : public Thread {
  Semaphore self;
  std::queue<Car *> *queue;
  Mutex mutex;
  unsigned wait = 0u;

protected:
  void *run() override;

public:
  Deadlock(std::queue<Car *> *queue) : queue(queue) {}
  void signal();
  void waiting();
  void gone();
};
#endif // DEADLOCK_H
