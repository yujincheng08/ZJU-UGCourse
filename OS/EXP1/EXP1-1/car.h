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
  virtual void *run() override final; // Called when thread start
  int num;  // The car number
  Semaphore self;   // Semaphore use to wait
  Direction direction;  // Car's direction
  std::queue<Car *> *queue; // Array of queues from the cross,
                            // Used to enqueue, dequeu and lookAtRight and signal
  Mutex *queueMutex;  // Array of mutex for each queue for each direction
  Deadlock &deadlock;  // Deadlock detection and handler
  
  // @description: Signal one direction's first car to go
  // @params direction: the direction to signal
  // @params time: how many times to signal
  // @return: true if signal one car and false if no car to signal
  bool signalDirection(const Direction &direction, unsigned times = 1);
  // @description: Look at the right size
  // @return: true if there's car on the right size
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
  // @description: Create a new car
  // @params: num: the car number
  // @params direction: the direction the car's going
  // @params queue: queues of each direction
  // @params queueMutex: mutex for each queue of each direction
  // @params deadlock: deadlock detection and handler thread
  Car(const int &num, const Direction &direction, std::queue<Car *> *queue,
      Mutex *queueMutex, Deadlock &deadlock)
      : num(num), direction(direction), queue(queue), queueMutex(queueMutex),
        deadlock(deadlock) {
    enQueue(); // Once created, enqueue
  }
  // @description: Wake up the car
  // totally once and the car arrives at the cross but not look at the right
  // totally twice so that the car arrives and looks at the right
  // totally three times so that the car ignores the right size of car and cross
  // @params times: how many times to wake up.
  void wakeUp(unsigned times = 1);
};
#endif
