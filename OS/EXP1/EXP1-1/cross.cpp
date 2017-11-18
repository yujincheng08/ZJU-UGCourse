#include "cross.h"
#include <iostream>
#include <unistd.h>

using namespace std;

Cross::Cross(const string &sequence) : deadlock(queue) {
  int count = 0;
  Car *previous[4] = {nullptr, nullptr, nullptr, nullptr};
  deadlock.start();
  for (auto &c : sequence) {
    Direction direction(c);
    Car *car = new Car(++count, direction, queue, queueMutex, deadlock);
    cars.push_back(car);
    car->start();
  }
  for (int i = 0; i < 4; i++) {
    if (!queue[i].empty())
      queue[i].front()->wakeUp(2);
  }
  for (auto &c : cars) {
    c->wait();
    delete c;
  }
  deadlock.terminate();
}

/*
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
*/
