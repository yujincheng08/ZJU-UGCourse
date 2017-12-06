#include "thread.h"

// @description: static function for starting thread to call member function.
void *Thread::runner(void *This) {
  void *retval;
  retval = static_cast<Thread *>(This)->run();
  pthread_exit(retval);
}

// Initialize pthread attribute.
Thread::Thread() { pthread_attr_init(&tattr); }

void Thread::start() {
  // Set thread status.
  running = true;
  // Create a thread.
  pthread_create(&tid, &tattr, runner, this);
}

void *Thread::wait() {
  if (running) {
    void *result;
    // If the thread is running, wait until it terminates.
    pthread_join(tid, &result);
    // Update thread information.
    running = false;
    // Return the result.
    return result;
  } else {
    // If not running, return `nullptr`.
    return nullptr;
  }
}

void Thread::terminate() {
  pthread_cancel(tid); //for to terminate it
  running = false;
}

//simply calling the function
void Thread::exit(void *retval) { pthread_exit(retval); }

Thread::~Thread() {
  wait();
  pthread_attr_destroy(&tattr);
}
