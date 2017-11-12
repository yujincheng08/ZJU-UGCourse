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
      queue[i].front()->wakeUp();
  }
  for (auto &c : cars) {
    c->wait();
  }
  deadlock.terminate();
}

void Cross::lockBlocks(const Direction &direction) {
  switch (direction) {
  case Direction::East:
    Blocks[2].lock();
    Blocks[1].lock();
    break;
  case Direction::West:
    Blocks[0].lock();
    Blocks[3].lock();
    break;
  case Direction::North:
    Blocks[3].lock();
    Blocks[2].lock();
    break;
  case Direction::South:
    Blocks[1].lock();
    Blocks[0].lock();
    break;
  }
}

void Cross::unlockBlocks(const Direction &direction) {
  switch (direction) {
  case Direction::East:
    Blocks[1].unlock();
    Blocks[2].unlock();
    break;
  case Direction::West:
    Blocks[3].unlock();
    Blocks[0].unlock();
    break;
  case Direction::North:
    Blocks[2].unlock();
    Blocks[3].unlock();
    break;
  case Direction::South:
    Blocks[0].unlock();
    Blocks[1].unlock();
    break;
  }
}

/*
void Cross::tryQueue(Car *car) {
  const auto &direction = car->getDirection();
  auto &mutex = queueMutex[direction];
  mutex.lock();
  while (true) {
    auto &carQueue = queue[direction];
    if (carQueue.empty()) {
      carQueue.push(car);
      mutex.unlock();
      car->arrive();
      break;
    } else {
      auto back = queue[direction].back();
      back->tellNext(car);
      mutex.unlock();
      car->sleep();
    }
  }
}
*/

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

bool Cross::signalDirection(const Direction &direction) {
  queueMutex[direction].lock();
  if (queue[direction].empty()) {
    queueMutex[direction].unlock();
    return false;
  } else {
    queue[direction].front()->force();
    queueMutex[direction].unlock();
    return true;
  }
}

bool Cross::signalLeft(const Direction &direction) {
  return signalDirection(direction.left());
}

bool Cross::signalRight(const Direction &direction) {
  return signalDirection(direction.right());
}
bool Cross::signalOpposite(const Direction &direction) {
  return signalDirection(direction.opposite());
}
bool Cross::signalNext(const Direction &direction) {
  return signalDirection(direction);
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
