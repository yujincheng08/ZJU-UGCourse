#ifndef DEADLOCK_H
#define DEADLOCK_H

#include "mutex.h"
#include "sem.h"
#include "thread.h"
#include <queue>

class Car;

class Deadlock : public Thread {
  // Use for waiting and signaling
  Semaphore self;
  // Array of queues for signaling cars
  std::queue<Car *> *queue;
  // Record how many cars is waiting for the car at the right side at the crossing.
  // If there four, deadlock occurs.
  unsigned wait = 0u;
  // Use to atomically increase and decrease `wait`
  Mutex mutex;
  
  // Being Called after starting thread
  virtual void *run() override final;
  // Signal the deadlock to handle deadlock
  void signal();
public:
  // @destruction: Creating a deadlock detecion object.
  // @params queue: Array of queues to signal car when deadlock occurs.
  Deadlock(std::queue<Car *> *queue) : queue(queue) {}
  // @description: Tell the deadlock detection thread there's a car wating. 
  //   When a car is waiting for the car at the right side at the crossing,
  //   it should call this function to detect if there's a deadlock.
  void waiting();
  // @description: Tell the deadlock detection thread
  //   that the car has been signal to go from the car at the right side.
  void gone();
};
#endif // DEADLOCK_H
