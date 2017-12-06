#ifndef THREAD_H
#define THREAD_H

#include "pthread.h"

// Abstract class for a thread.
class Thread {
  pthread_t tid; // Thread's id.
  pthread_attr_t tattr; // Thread's attribute.
  bool running = false; // Thread's status.
  static void *runner(void *); // For pthread_create().

protected:
  // @description: Create a thread object but not started.
  Thread();
  // @description: This function will be called when the thread starts.
  // Derived classes should override this function.
  // @retrun: pointer of the return object.
  virtual void *run() = 0;
  // @description: End the thread with the pointer of the return object.
  // Returning on `run` function or calling `exit` function has the same effect.
  void exit(void *retval);

public:
  // @description: Start the thread. It will call `run` function.
  void start();
  // @description: Block until the thread terminated.
  // @return: pointer of the return object.
  void *wait();
  // @description: Force terminate the thread without returning value.
  void terminate();
  // @description: Check if the thread is running.
  // @return: If the thread is running, return true, otherwise return false.
  bool isRunning() { return running; } // if it's running
  // @description: Destruction function.
  virtual ~Thread();
};

#endif // THREAD_H
