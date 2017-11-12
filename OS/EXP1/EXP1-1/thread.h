#ifndef THREAD_H
#define THREAD_H

#include "pthread.h"

class Thread {
  pthread_t tid;
  pthread_attr_t tattr;
  bool running = false;
  static void *runner(void *);

protected:
  Thread();
  virtual void *run() = 0;
  void exit(void *retval);

public:
  void start();
  void *wait();
  void terminate();
  bool isRunning() { return running; }
  virtual ~Thread();
};

#endif // THREAD_H
