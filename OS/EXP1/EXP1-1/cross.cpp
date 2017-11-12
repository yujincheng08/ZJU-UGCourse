#include "cross.h"
#include <iostream>
#include <unistd.h>

using namespace std;

Cross::Cross(const string &sequence) : deadlock(global, this) {
  int count = 0;
  Car *previous[4] = {nullptr, nullptr, nullptr, nullptr};
  deadlock.start();
  for (auto &c : sequence) {
    Direction direction(c);
    Car *car = new Car(++count, direction, this);
    auto &pre = previous[direction];
    if (pre)
      pre->tellNext(car);
    pre = car;
    queue[direction].push(car);
    cars.push_back(car);
    car->start();
  }
  for (int i = 0; i < 4; i++) {
    if (!queue[i].empty())
      queue[i].front()->wakeUp(2);
  }
  for (auto &c : cars) {
    c->wait();
  }
  deadlock.terminate();
}

void Cross::deQueue(Car *car) {
  const auto &direction = car->getDirection();
  queueMutex[direction].lock();
  queue[direction].pop();
  queueMutex[direction].unlock();
}

bool Cross::lookAtRight(const Direction &direction) {
  auto right = direction.right();
  queueMutex[right].lock();
  bool result = !queue[right].empty();
  queueMutex[right].unlock();
  return result;
}

bool Cross::signalDirection(const Direction &direction, unsigned times) {
  queueMutex[direction].lock();
  if (queue[direction].empty()) {
    queueMutex[direction].unlock();
    return false;
  } else {
    queue[direction].front()->wakeUp(times);
    queueMutex[direction].unlock();
    return true;
  }
}

void Cross::waiting() {
  global.lock();
  if (++wait == 4) {
    global.unlock();
    deadlock.signal();
  }
  global.unlock();
}

void Cross::gone() {
  global.lock();
  --wait;
  global.unlock();
}
