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
  if (parent->lookAtRight(direction)) {
    parent->waiting();
    self.wait();
    parent->gone();
  }
  stringstream msg;
  msg << "Car " << num << " from " << direction << " is leaving crossing"
      << endl;
  cout << msg.str();
  parent->deQueue(this);
  parent->signalDirection(direction.left(), 2) ||
      parent->signalDirection(direction.opposite()) ||
      parent->signalDirection(direction.right()) ||
      parent->signalDirection(direction);
  parent->signalDirection(direction);
}
