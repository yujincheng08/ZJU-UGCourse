#ifndef READTHREAD_H
#define READTHREAD_H

#include <QThread>
#include <QMutex>

#include "Buffer.h"
class File;
class ReadThread : public QThread
{
    Q_OBJECT
    friend class BufferManager;
private:
    QMutex Mutex;
    File *CurrentFile;
    Buffer::pos_type CurrentPos;

protected:
    virtual void run() override final;
private:
    ReadThread();
    void setNext(File *file, Buffer::pos_type pos);
};

inline ReadThread::ReadThread()
{}

inline void ReadThread::setNext(File *file, Buffer::pos_type pos)
{
    Mutex.lock();
    CurrentFile = file;
    CurrentPos = pos;
    if(!isRunning())
        start();
    Mutex.unlock();
}


#endif // READTHREAD_H
