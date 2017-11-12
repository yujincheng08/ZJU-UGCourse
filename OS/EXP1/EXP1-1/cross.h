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
  Mutex Blocks[4];

  std::vector<Car *> cars;

  std::queue<Car *> queue[4];

  Mutex queueMutex[4];

  Mutex global;

  unsigned wait = 0;

  bool signalDirection(const Direction &direction);

  Deadlock deadlock;

public:
  Cross(const std::string &sequence);
  void lockBlocks(const Direction &direction);
  void unlockBlocks(const Direction &direction);
  // void tryQueue(Car *car);
  void deQueue(Car *car);
  bool lookAtRight(const Direction &direction);
  bool signalLeft(const Direction &direction);
  bool signalRight(const Direction &direction);
  bool signalOpposite(const Direction &direction);
  bool signalNext(const Direction &direction);
  void waiting();
  void gone();
};

#endif
