#include "car.h"
#include <iostream>
#include <sstream>
#include <unistd.h>

using namespace std;

void *Car::run() {
  self.wait();
  arrive();
  self.wait();
  cross();
  exit(0);
}

void Car::wakeUp(unsigned times) {
  while (times--) {
    self.signal();
  }
}

void Car::arrive() {
  stringstream msg;
  msg << "Car " << num << " from " << direction << " arrives at crossing"
      << endl;
  cout << msg.str();
}

void Car::cross() {
  if (lookAtRight()) {
    deadlock.waiting();
    self.wait();
    deadlock.gone();
  }
  stringstream msg;
  msg << "Car " << num << " from " << direction << " is leaving crossing"
      << endl;
  cout << msg.str();
  deQueue();
  signalDirection(direction.left(), 2) ||
      signalDirection(direction.opposite()) ||
      signalDirection(direction.right()) || signalDirection(direction);
  signalDirection(direction);
}

void Car::deQueue() {
  queueMutex[direction].lock();
  queue[direction].pop();
  queueMutex[direction].unlock();
}

bool Car::lookAtRight() {
  auto right = direction.right();
  queueMutex[right].lock();
  bool result = !queue[right].empty();
  queueMutex[right].unlock();
  return result;
}

bool Car::signalDirection(const Direction &direction, unsigned times) {
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

void Car::enQueue() { queue[direction].push(this); }
