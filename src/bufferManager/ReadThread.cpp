#include "ReadThread.h"
#include "BufferManager.h"
void ReadThread::run()
{
    while(true)
    {
        Mutex.lock();
        Buffer::pos_type pos = CurrentPos += Buffer::bufferSize();
        File *file = CurrentFile;
        Mutex.unlock();
        BufferManager &bm = BufferManager::bufferManager();
        if(!file->IsEnd(pos) && !file->Exist(pos) && !bm.full())
        {
            Buffer * buff = bm.buff(file, pos);
            file->Insert(buff);
        }
        else
            break;
    }
    quit();
}
