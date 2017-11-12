#ifndef DEADLOCK_H
#define DEADLOCK_H

#include "mutex.h"
#include "sem.h"
#include "thread.h"

class Cross;

class Deadlock : public Thread {
  Semaphore self;
  Cross *parent;
  Mutex &global;

protected:
  void *run() override;

public:
  Deadlock(Mutex &global, Cross *parent) : global(global), parent(parent){};
  void signal();
};
#endif // DEADLOCK_H
