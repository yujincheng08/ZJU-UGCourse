#include "deadlock.h"
#include "cross.h"
#include "direction.h"
#include <iostream>
#include <sstream>

using namespace std;

void *Deadlock::run() {
  while (true) {
    self.wait();
    global.unlock();
    stringstream msg;
    msg << "DEADLOCK: car jam detected, signaling North to go." << endl;
    cout << msg.str();
    parent->signalDirection('n');
  }
}

void Deadlock::signal() {
  global.lock();
  self.signal();
}
