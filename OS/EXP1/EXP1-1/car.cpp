#include "car.h"
#include <iostream>
#include <sstream>
#include <unistd.h>

using namespace std;

// @description: This function will be call when is called thread starts.
void *Car::run() {
  self.wait(); // Wait for arriving at the crossing.
  arrive(); // Arrive at the crossing, output prompts.
  self.wait(); // Wait to cross.
  cross(); // Look at the right (maybe) and cross the crossing.
  exit(nullptr); // Exit the thread.
}

// @description: wake up the car for serval times.
// @params times: how many times will the car be waked up.
void Car::wakeUp(unsigned times) {
  // Signal for `times` times.
  while (times--) {
    self.signal();
  }
}

// @description: the car arrives at the crossing.
void Car::arrive() {
  // Simply output prompts.
  // Using stringstream for aotmic output.
  stringstream msg;
  msg << "Car " << num << " from " << direction << " arrives at crossing"
      << endl;
  cout << msg.str();
}

void Car::cross() {
  // If there's a car at the right.
  if (lookAtRight()) {
    deadlock.waiting();// Tell the deadlock I am waiting at for the right car.
    self.wait(); // And wait for the car at the right side,
                 // or maybe the deadlock thread to signal me.
    deadlock.gone(); // After being signaled, tell the deadlock I am gone.
  }
  // Use stringstream, output prompts.
  stringstream msg;
  msg << "Car " << num << " from " << direction << " is leaving crossing"
      << endl;
  cout << msg.str();
  // Dequeue before signaling others, avoid signaling myself.
  deQueue();
  // Signal car on the left, if not success, signal the opposite one, 
  // if not success again, signal the right one,
  // if not success again, signal the back one.
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
  // Atomically deque.
  queueMutex[direction].lock();
  queue[direction].pop();
  queueMutex[direction].unlock();
}

bool Car::lookAtRight() {
  // Get the right direction.
  auto right = direction.right();
  // This proceduer should be automic
  queueMutex[right].lock();
  // If the right queue is empty, meaning no cars at the right, return false, otherwise, return true
  bool result = !queue[right].empty();
  queueMutex[right].unlock();
  return result;
}

bool Car::signalDirection(const Direction &direction, unsigned times) {
  // This proceduer should be atomic
  queueMutex[direction].lock();
  if (queue[direction].empty()) {
    // If empty, return false
    queueMutex[direction].unlock();
    return false;
  } else {
    // Otherwise, wake up the first car in this direction and return true
    queue[direction].front()->wakeUp(times);
    queueMutex[direction].unlock();
    return true;
  }
}

void Car::enQueue() {
  // Atomically push itself into the queue.
  queueMutex[direction].lock();
  queue[direction].push(this);
  queueMutex[direction].unlock();
}
