#include "BufferManager.h"
#include "ReadThread.h"
#include "WriteThread.h"
#include "BufferList.h"
using namespace std;
size_t BufferManager::MaxBuffer = 200000;
BufferManager &BufferManager::bufferManager()
{
    static BufferManager bm;
    return bm;
}

File &BufferManager::open(const string &fileName)
{
    return bufferManager().Open(fileName);
}

BufferManager::~BufferManager()
{
    readThread->terminate();
    readThread->wait();
    for(auto &i : Files)
        i.second->flush();
    writeThread->wait();
    for(auto &i : Files)
        delete i.second;
}

Buffer *BufferManager::buff(File *file, const Buffer::pos_type &pos, const Buffer::pos_type &size)
{
    Mutex.lock();
    Buffer *buffer = nullptr;
    if(list->size() > MaxBuffer - 1)
    {
        for(size_t i = 0U; i < list->size() - MaxBuffer + 1; ++i)
        {
            auto last = list->last();
            if(last->removeable())
                delete last;
            else
                break;
        }
    }
    buffer = new Buffer(*file, pos, size);
    Mutex.unlock();
    return buffer;
}

void BufferManager::preRead(File *file, const Buffer::pos_type &pos)
{
    readThread->setNext(file, pos);
}

void BufferManager::queueBuff(Buffer * const buffer)
{
    writeThread->enQueue(buffer);
}

BufferManager::BufferManager()
    :readThread(new ReadThread()),writeThread(new WriteThread())
{}

File &BufferManager::Open(const string &fileName)
{
    auto i = Files.find(fileName);
    if(i == Files.end())
    {
        QFile *Stream = new QFile(fileName.c_str(),this);
        Stream->open(QFile::ReadWrite|QFile::Unbuffered);
        File *file = new File(*Stream);
        Files.insert(std::make_pair(fileName,file));
        return *file;
    }
    else
        return *i->second;
}

bool BufferManager::full()
{
    Mutex.lock();
    bool result = list->size()>=MaxBuffer;
    Mutex.unlock();
    return result;
}
