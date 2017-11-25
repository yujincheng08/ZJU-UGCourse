 #include "car.h"
#include <iostream>
#include <sstream>
#include <unistd.h>

using namespace std;

void *Car::run() {
  self.wait(); // wait for arriving
  arrive(); // arrive at the crossing
  self.wait(); // wait for crossing
  cross(); // look at right and cross the crossing
  exit(nullptr);
}

void Car::wakeUp(unsigned times) {
  while (times--) {
    self.signal(); //signal times times
  }
}

void Car::arrive() {
  //Simply output prompts
  stringstream msg;
  msg << "Car " << num << " from " << direction << " arrives at crossing"
      << endl;
  cout << msg.str();
}

void Car::cross() {
  // if there's car at the right
  if (lookAtRight()) {
    deadlock.waiting();// if so, tell the deadlock
    self.wait(); // and wait
    deadlock.gone(); // after being signal, tell the deadlock I am gone
  }
  //Output prompts
  stringstream msg;
  msg << "Car " << num << " from " << direction << " is leaving crossing"
      << endl;
  cout << msg.str();
  // dequeue before signaling others
  deQueue();
  // signal car on the left, if not success, signal the opposite one, 
  // if not success again, signal the right one,
  // if not success again, signal the back one
  signalDirection(direction.left(), 2) ||
      signalDirection(direction.opposite()) ||
      signalDirection(direction.right()) || signalDirection(direction);
  // signal the back one. If some is signal above, the back car only arrives at the cross,
  // but not look at the right.
  // if no cars at the other queue, the back car will signal twice, and it will look at the right
  // and try to cross
  signalDirection(direction);
}

void Car::deQueue() {
  // automically deque
  queueMutex[direction].lock();
  queue[direction].pop();
  queueMutex[direction].unlock();
}

bool Car::lookAtRight() {
  auto right = direction.right();
  // the proceduer should be automic
  queueMutex[right].lock();
  // if the right queue is empty, return false, otherwise, return true
  bool result = !queue[right].empty();
  queueMutex[right].unlock();
  return result;
}

bool Car::signalDirection(const Direction &direction, unsigned times) {
  // this proceduer should be automic
  queueMutex[direction].lock();
  if (queue[direction].empty()) {
    // if empty, return false
    queueMutex[direction].unlock();
    return false;
  } else {
    // otherwise, wake up and return true
    queue[direction].front()->wakeUp(times);
    queueMutex[direction].unlock();
    return true;
  }
}

void Car::enQueue() {
  // automically push itself
  queueMutex[direction].lock();
  queue[direction].push(this);
  queueMutex[direction].unlock();
}
