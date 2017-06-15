#include "WriteThread.h"
#include "BufferManager.h"
void WriteThread::run()
{
    while(!WriteList.empty())
    {
        Mutex.lock();
        auto i = WriteList.front();
        BufferManager::bufferManager().writeBuffer(i);
        WriteList.pop_front();
        Mutex.unlock();
    }
    quit();
}
