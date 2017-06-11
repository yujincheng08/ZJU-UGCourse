#ifndef WRITETHREAD_H
#define WRITETHREAD_H

#include <QObject>
#include <QThread>
#include <list>
#include <mutex>
#include "Buffer.h"
class Buffer;
class WriteThread : public QThread
{
    friend class BufferManager;
    Q_OBJECT
    std::list<Buffer *> WriteList;
    std::mutex Mutex;
private:
    WriteThread();
    //When trying to write a buffer that's being written to disk.
    //How about memory overflow but the last element always waiting to write?
    //Read immediatelly after writing complete
protected:
    virtual void run() override final;
private slots:
    void enQueue(Buffer * const &buffer);
};

inline WriteThread::WriteThread()
{}

inline void WriteThread::enQueue(Buffer * const &buffer)
{
    Mutex.lock();
    if(!buffer->InList)
        WriteList.push_back(buffer);
    Mutex.unlock();
    if(!isRunning())
        start();
}



#endif // WRITETHREAD_H
