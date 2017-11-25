#ifndef THREAD_H
#define THREAD_H

#include "pthread.h"

class Thread {
  pthread_t tid;
  pthread_attr_t tattr;
  bool running = false;
  //use for pthread_create()
  static void *runner(void *);

protected:
  Thread();
  virtual void *run() = 0; // for child process to use
  void exit(void *retval); // exit with return value

public:
  void start(); //start a thread
  void *wait(); //wait a thread to finish
  void terminate(); // force to terminate it
  bool isRunning() { return running; } // if it's running
  virtual ~Thread();
};

#endif // THREAD_H
