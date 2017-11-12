#include "deadlock.h"
#include "car.h"
#include "cross.h"
#include "direction.h"
#include <iostream>
#include <sstream>

using namespace std;

void *Deadlock::run() {
  while (true) {
    self.wait();
    mutex.unlock();
    stringstream msg;
    msg << "DEADLOCK: car jam detected, signaling North to go." << endl;
    cout << msg.str();
    queue[Direction('n')].front()->wakeUp();
  }
}

void Deadlock::signal() {
  mutex.lock();
  self.signal();
}

void Deadlock::waiting() {
  mutex.lock();
  if (++wait == 4) {
    mutex.unlock();
    signal();
  }
  mutex.unlock();
}

void Deadlock::gone() {
  mutex.lock();
  --wait;
  mutex.unlock();
}
