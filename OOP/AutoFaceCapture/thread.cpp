#include "thread.h"
#include <QTimer>
#include <QThread>
Thread::Thread()
    :Enabled(true),Timer(new QTimer),
      MyThread(new QThread(this))
{
    moveToThread(MyThread);
    Timer->moveToThread(MyThread);
    connect(MyThread,MyThread->started,this,start);
    connect(MyThread,MyThread->finished,this,stop);
    connect(Timer,Timer->timeout,this,[this](){if(Enabled) emit timeout();});
}

void Thread::setTimeout(int msec)
{
    Timer->setInterval(msec);
}

void Thread::start()
{
    if(!Timer->isActive())
        Timer->start();
    else
    {
        Timer->stop();
        Timer->start();
    }
}

void Thread::stop()
{
    if(Timer->isActive())
        Timer->stop();
}

void Thread::run()
{
    if(!MyThread->isRunning())
        MyThread->start();
}

void Thread::quit()
{
    if(MyThread->isRunning())
    {
        MyThread->quit();
        MyThread->wait();
    }
}

void Thread::pause(bool enabled)
{
    Enabled = enabled;
}
