#ifndef CAR_H
#define CAR_H
#include "deadlock.h"
#include "direction.h"
#include "mutex.h"
#include "pthread.h"
#include "sem.h"
#include "thread.h"
#include <queue>

class Car : public Thread {
  virtual void *run() override final; // called when thread start
  int num;  // the car number
  Semaphore self;   // Semaphore use to wait
  Direction direction;  // Car's direction
  std::queue<Car *> *queue; // array of queues from the cross, used to enqueue, dequeu and lookAtRight and signal
  Mutex *queueMutex;  // array of mutex for each queue for each direction
  Deadlock &deadlock;  // deadlock detection and handler
  
  //Signal one direction's first car to go
  //@direction: the direction to signal
  //@time: how many times to signal
  //@return: true if signal one car and false if no car to signal
  bool signalDirection(const Direction &direction, unsigned times = 1);
  //Look at the right size
  //@return: true if there's car on the right size
  bool lookAtRight();
  //Arrive at the corner, print some prompts automic
  void arrive();
  //Cross the corss, print some prompts and signal other cars
  void cross();
  //Enqueue to one direction queue
  void enQueue();
  //Dequeue from one direction queue
  void deQueue();
public:
  //Create a new car
  //@num: the car number
  //@direction: the direction the car's going
  //@queue: queues of each direction
  //@queueMutex: mutex for each queue of each direction
  //@deadlock: deadlock detection and handler thread
  Car(const int &num, const Direction &direction, std::queue<Car *> *queue,
      Mutex *queueMutex, Deadlock &deadlock)
      : num(num), direction(direction), queue(queue), queueMutex(queueMutex),
        deadlock(deadlock) {
    enQueue(); // Once created, enqueue
  }
  //wake up the car
  //@times: how many times to wake up.
  //  totally once so that the car arrives at the cross but not look at the right
  //  totally twice so that the car arrives and looks at the right
  //  totally three times so that the car ignores the right size of car and cross
  void wakeUp(unsigned times = 1);
};
#endif
