#include "deadlock.h"
#include "car.h"
#include "cross.h"
#include "direction.h"
#include <iostream>
#include <sstream>

using namespace std;

void *Deadlock::run() {
  while (true) {
    self.wait(); // initially waiting
    stringstream msg; // if waked up, output prompts
    msg << "DEADLOCK: car jam detected, signaling North to go." << endl;
    cout << msg.str();
    queue[Direction('n')].front()->wakeUp(); //and wakeup the north one
  }
}

void Deadlock::signal() {
  mutex.lock(); // lock it and signal
  self.signal();
}

void Deadlock::waiting() {
  //automically increase wait
  mutex.lock();
  if (++wait == 4) {
    // if four cars waiting, deadlock!
    mutex.unlock();
    signal();
  }
  mutex.unlock();
}

void Deadlock::gone() {
  // automically decrease wait
  mutex.lock();
  --wait;
  mutex.unlock();
}
