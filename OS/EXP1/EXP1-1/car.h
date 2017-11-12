#ifndef CAR_H
#define CAR_H
#include "deadlock.h"
#include "direction.h"
#include "mutex.h"
#include "pthread.h"
#include "sem.h"
#include "thread.h"
#include <queue>

class Car : public Thread {
  virtual void *run() override;
  int num;
  Semaphore self;
  Direction direction;
  std::queue<Car *> *queue;
  Mutex *queueMutex;
  Deadlock &deadlock;

public:
  Car(const int &num, const Direction &direction, std::queue<Car *> *queue,
      Mutex *queueMutex, Deadlock &deadlock)
      : num(num), direction(direction), queue(queue), queueMutex(queueMutex),
        deadlock(deadlock) {
    enQueue();
  }
  bool signalDirection(const Direction &direction, unsigned times = 1);
  bool lookAtRight();
  void arrive();
  void cross();
  void wakeUp(unsigned times = 1);
  void enQueue();
  void deQueue();
};
#endif
