#include "car.h"
#include <iostream>
#include <sstream>
#include <unistd.h>

using namespace std;

void *Car::run() {
  self.wait();
  arrive();
  cross();
  exit(0);
}

void Car::wakeUp() { self.signal(); }

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
  // parent->lockBlocks(direction);
  stringstream msg;
  msg << "Car " << num << " from " << direction << " is leaving crossing"
      << endl;
  cout << msg.str();
  // parent->unlockBlocks(direction);
  parent->deQueue(this);
  parent->signalLeft(direction) || parent->signalOpposite(direction) ||
      parent->signalRight(direction) || parent->signalNext(direction);
}

void Car::tellNext(Car *next) { this->next = next; }

void Car::force() {
  self.signal();
  self.signal();
}
