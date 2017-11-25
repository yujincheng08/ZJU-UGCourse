#include "cross.h"
#include <iostream>
#include <unistd.h>

using namespace std;

Cross::Cross(const string &sequence) : deadlock(queue) {
  int count = 0;// count for determing the car number
  deadlock.start(); // start deadlock detect
  for (auto &c : sequence) { // for each char
    Direction direction(c); // get the condiction
    // new a car
    Car *car = new Car(++count, direction, queue, queueMutex, deadlock);
    // push the car for waiting
    cars.push_back(car);
    // start each car, and every car they are waiting
    car->start();
  }
  for (int i = 0; i < 4; i++) {
    if (!queue[i].empty())
      queue[i].front()->wakeUp(2); //let the front car look at the right and try to cross
  }
  for (auto &c : cars) {
    c->wait();
    delete c; // wait for each car and release the momory
  }
  deadlock.terminate(); // terminate
}