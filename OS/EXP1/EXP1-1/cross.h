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
  // Storing all cars. For waiting all the cars to terminate.
  std::vector<Car *> cars;

  // Queue of cars for each direction.
  std::queue<Car *> queue[4];

  // Mutex for each direciton.
  Mutex queueMutex[4];

  // Deadlock detect object.
  Deadlock deadlock;

public:
  // @destruction: Create a cross object and end when all cars cross the crossing.
  // A cross object only initialize and start every car threads and deadlock dection threads.
  // @params sequence: Car sequence, containing `W`, `N`, `E`, `S` charactors,
  //   other charactors will be ignored.
  Cross(const std::string &sequence);
};

#endif
