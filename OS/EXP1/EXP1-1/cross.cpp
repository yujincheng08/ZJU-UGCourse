#include "cross.h"
#include <iostream>
#include <unistd.h>

using namespace std;

Cross::Cross(const string &sequence)
  : deadlock(queue) { // Initial deadlock detection thread.
  int count = 0; // counter for determing the car number.
  deadlock.start(); // Start deadlock detection.
  for (auto &c : sequence) { // for each char
    Direction direction(c); // get the condiction from the car
    // Create a car with car number.
    Car *car = new Car(++count, direction, queue, queueMutex, deadlock);
    // Push the car to the vector.
    cars.push_back(car);
    // And start the car.
    car->start();
  }

  // Now signal the first cars of each direction to arrive at the crossing, 
  // and look at the right side, meaning we should signal twice.
  for (int i = 0; i < 4; i++) {
    if (!queue[i].empty()) // Only signal if there is a car in the queue.
      queue[i].front()->wakeUp(2);
  }
  // Waiting each car to terminate and then release the memory.
  for (auto &c : cars) {
    c->wait();
    delete c;
  }
  // Ternimate the deadlock detection thread.
  deadlock.terminate();
}
