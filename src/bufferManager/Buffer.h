#ifndef BUFFER_H
#define BUFFER_H

#include <QIODevice>
#include <BufferListItem.h>
#include <mutex>
#include <cstring>
//Buffer of a file block
class QString;
class File;
class Buffer : protected BufferListItem
{
    friend class BufferManager;
    friend class File;
    friend class WriteThread;
public:
    using pos_type = std::streamoff;
private:
    static const size_t BufferSize;
    char *Buff;
    File &f;
    QIODevice &Stream;
    pos_type Position;
    size_t Size;
    bool Dirty = false;
    bool InList = false;
    std::mutex Mutex;
private:
    static QString partition();
    explicit Buffer(File &stream,
                    const pos_type &position,
                    const size_t &empty = 0U);
    void write();
    void changeSize(const size_t &size);
protected:
    virtual bool removeable() override final;
public:
    virtual ~Buffer() override;
    static size_t bufferSize();
};

inline void Buffer::write()
{
    Mutex.lock();
    Stream.seek(Position);
    Stream.write(Buff, Size);
    Mutex.unlock();
}

inline void Buffer::changeSize(const size_t &size)
{
    if(size>Size)
        memset(Buff+Size,0,size-Size);
    //else
    Size=size;
}

inline bool Buffer::removeable()
{
    return Dirty == false;
}

inline size_t Buffer::bufferSize()
{
    return BufferSize;
}

#endif // BUFFER_H
