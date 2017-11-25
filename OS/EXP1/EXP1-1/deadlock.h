#ifndef DEADLOCK_H
#define DEADLOCK_H

#include "mutex.h"
#include "sem.h"
#include "thread.h"
#include <queue>

class Car;

class Deadlock : public Thread {
  // use to wait and signal
  Semaphore self;
  // use to signal cars
  std::queue<Car *> *queue;
  // use to automically increase and decrease wait
  Mutex mutex;
  // how many cars waiting. if four, deadlock
  unsigned wait = 0u;
  
  //called after starting thread
  virtual void *run() override final;
  //signal the deadlock to handle deadlock
  void signal();
public:
  Deadlock(std::queue<Car *> *queue) : queue(queue) {}
  //for cars to tell me he is waiting
  void waiting();
  //for cars to tell me he has gone
  void gone();
};
#endif // DEADLOCK_H
