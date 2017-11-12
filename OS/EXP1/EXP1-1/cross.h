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

class Cross {
  /* +-+-+
   * |2|1|
   * +-+-+
   * |3|0|
   * +-+-+
   */
  std::vector<Car *> cars;

  std::queue<Car *> queue[4];

  Mutex queueMutex[4];

  Deadlock deadlock;

public:
  Cross(const std::string &sequence);
};

#endif
