#ifndef CROSS_H
#define CROSS_H

#include "car.h"
#include "deadlock.h"
#include "direction.h"
#include "mutex.h"
#include <queue>
#include <string>
#include <vector>

class Car;
class Deadlock;

class Cross {
  /* +-+-+
   * |2|1|
   * +-+-+
   * |3|0|
   * +-+-+
   */
  Mutex Blocks[4];

  std::vector<Car *> cars;

  std::queue<Car *> queue[4];

  Mutex queueMutex[4];

  Mutex global;

  unsigned wait = 0;

  Deadlock deadlock;

public:
  Cross(const std::string &sequence);
  void deQueue(Car *car);
  bool lookAtRight(const Direction &direction);
  bool signalDirection(const Direction &direction, unsigned times = 1);
  void waiting();
  void gone();
};

#endif
