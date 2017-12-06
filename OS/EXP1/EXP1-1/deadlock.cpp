#include "deadlock.h"
#include "car.h"
#include "cross.h"
#include "direction.h"
#include <iostream>
#include <sstream>

using namespace std;

void *Deadlock::run() {
  // Infinite loop
  while (true) {
    // At the very beginning, it should wait because there will not be deadlock.
    self.wait();
    // Begin to handle the deadlock.
    // The routine will be waked up when deadlock occurs.
    // Firstly, simply output the prompt.
    stringstream msg;
    msg << "DEADLOCK: car jam detected, signaling North to go." << endl;
    cout << msg.str();
    // Then, simply wake up the car from north.
    queue[Direction('n')].front()->wakeUp();
  }
}

void Deadlock::signal() {
  // Signal `self` and the deadlock routine will start handling deadlock.
  self.signal();
}

void Deadlock::waiting() {
  // Atomically increase `wait`.
  mutex.lock();
  if (++wait == 4) {
    // If there is four car waiting in the crossing, deadlock occurs.
    // Wakeup the handler routine.
    signal();
  }
  mutex.unlock();
}

void Deadlock::gone() {
  // Simply atomically decrease `wait`.
  mutex.lock();
  --wait;
  mutex.unlock();
}
