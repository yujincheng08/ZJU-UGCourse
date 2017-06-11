#ifndef BUFFERMANAGER_H
#define BUFFERMANAGER_H

#include "Buffer.h"
#include "File.h"
#include <QObject>
#include <string>
#include <unordered_map>
#include <mutex>

class ReadThread;
class WriteThread;
class BufferManager : public QObject
{
    Q_OBJECT
    template<typename T, typename K>
    using HashMap = std::unordered_map<T,K>;
    using string = std::string;
    friend class File;
    friend class ReadThread;
private:
    ReadThread *readThread = nullptr;
    WriteThread *writeThread = nullptr;
    BufferList *list = Buffer::list();
    HashMap<string, File *> Files;
    File &Open(const string &fileName);
    std::mutex Mutex;
    static size_t MaxBuffer;
    static BufferManager &bufferManager();
    Buffer *buff(File *file, const Buffer::pos_type &pos, const Buffer::pos_type &size = Buffer::bufferSize());
    void preRead(File *file, const Buffer::pos_type &pos);
    void queueBuff(Buffer * const buffer);
    explicit BufferManager();
    void wait();
    bool full();
public:
    static File &open(const std::string &fileName);
    static size_t blockSize();
    static const size_t &maxBuffer();
    static void setMaxBuffer(const size_t &max);
    static size_t bufferSize();
};

inline size_t BufferManager::blockSize()
{
    return 8192U;
}

inline const size_t &BufferManager::maxBuffer()
{
    return MaxBuffer;
}

inline void BufferManager::setMaxBuffer(const size_t &max)
{
    MaxBuffer = max;
}

inline size_t BufferManager::bufferSize()
{
    return Buffer::bufferSize();
}

#endif
