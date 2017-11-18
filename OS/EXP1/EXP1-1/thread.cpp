#include "thread.h"

void *Thread::runner(void *This) { static_cast<Thread *>(This)->run(); }

Thread::Thread() { pthread_attr_init(&tattr); }

void Thread::start() {
  running = true;
  pthread_create(&tid, &tattr, runner, this);
}

void *Thread::wait() {
  if (running) {
    void *result;
    pthread_join(tid, &result);
    running = false;
    return result;
  } else {
    return nullptr;
  }
}

void Thread::terminate() {
  pthread_cancel(tid);
  running = false;
}

void Thread::exit(void *retval) { pthread_exit(retval); }

Thread::~Thread() {
  wait();
  pthread_attr_destroy(&tattr);
}
