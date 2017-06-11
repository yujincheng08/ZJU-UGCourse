#include "WriteThread.h"
void WriteThread::run()
{
    while(!WriteList.empty())
    {
        Mutex.lock();
        auto i = WriteList.front();
        i->write();
        i->Dirty = false;
        i->InList = false;
        WriteList.pop_front();
        Mutex.unlock();
    }
    quit();
}
