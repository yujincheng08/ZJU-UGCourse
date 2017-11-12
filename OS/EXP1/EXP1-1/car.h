#ifndef CAR_H
#define CAR_H
#include "cross.h"
#include "direction.h"
#include "mutex.h"
#include "pthread.h"
#include "sem.h"
#include "thread.h"

class Cross;

class Car : public Thread {
  virtual void *run() override;
  int num;
  Semaphore self;
  Direction direction;
  Cross *parent = nullptr;
  Car *next = nullptr;

public:
  Car(const int &num, const Direction &direction, Cross *parent)
      : num(num), direction(direction), parent(parent) {}
  void tellNext(Car *next);
  void arrive();
  void cross();
  void wakeUp(unsigned times = 1);
  const Direction &getDirection() { return direction; }
};
#endif
