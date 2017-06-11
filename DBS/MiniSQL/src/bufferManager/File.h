#ifndef FILE_H
#define FILE_H

#include <QFile>
#include <type_traits>
#include <unordered_map>
#include <QObject>
#include "Buffer.h"
#include "FixString.h"

class Buffer;
class File : public QObject
{
    Q_OBJECT
    friend class BufferManager;
    friend class ReadThread;
    friend class Buffer;
public:
    template<bool T, class K = void>
    using enable_if = typename std::enable_if<T,K>::type;
    template<typename T>
    using Valid = enable_if<!std::is_class<T>::value && !std::is_pointer<T>::value, T>;
    template<typename T>
    using isFixString = enable_if<std::is_same<T,FixString>::value, T>;
    template<typename T, typename K>
    using HashMap = std::unordered_map<T,K>;
    using string = std::string;
    using pos_type = std::streamoff;
private:
    std::mutex Mutex;
    QFile Stream;
    pos_type FileSize;
    size_t BlockCount;
    pos_type ReadCursor = 0;
    pos_type WriteCursor = 0;
    HashMap<size_t, Buffer *> Buffers; //pos = first * bufferSize
    File(const string &fileName);
    void Release(const pos_type &pos);
    void Insert(Buffer *const buffer);
    bool Exist(const pos_type &pos);
    bool IsEnd(const pos_type &pos);
    size_t Convert(const pos_type &pos);
    Buffer *GetBuffer(const size_t &pos, const size_t &offset);
    //pos_type GetOffset(const pos_type &pos, const pos_type &size = 0U);
    pos_type GetPos(const pos_type &pos, const pos_type &size = 0U);
    pos_type NextPos(const pos_type &pos, const pos_type &size);
public slots:
    void flush();
    void reset();
signals:
    //void flushed();
public:
    auto tellg() const -> const pos_type &;
    auto tellp() const -> const pos_type &;
    auto telle() const -> const pos_type &;
    bool eof() const;
    void seekg(const pos_type &pos);
    void seekp(const pos_type &pos);
    void resize(const size_t &pos);
    void remove();
    template<typename T>
    auto get() -> const Valid<T> &;
    template<typename T>
    auto get(const pos_type &pos, const size_t &size) -> const isFixString<T>;
    template<typename T>
    auto get(const size_t &size) -> const isFixString<T>;
    template<typename T>
    auto get(const pos_type &pos) -> const Valid<T> &;
    template<typename T>
    auto peek() -> const Valid<T> &;
    template<typename T, typename = Valid<T>>
    void put(const T& item);
    template<typename T, typename = Valid<T>>
    void put(const T& item, const pos_type &pos);
    void put(const FixString &item);
    void put(const FixString &item, const pos_type &pos);
    template<typename T, typename = Valid<T>>
    File &operator>>(T &target);
    File &operator>>(FixString &target);
    template<typename T, typename = Valid<T>>
    File &operator<<(const T &source);
    File &operator<<(const FixString &source);
    virtual ~File();
};

inline void File::put(const FixString &item)
{
    auto next = NextPos(WriteCursor, item.size());
    put(item, WriteCursor);
    WriteCursor = next;
}

inline void File::put(const FixString &item, const pos_type &pos)
{
    auto start = GetPos(pos, item.size());
    auto offset = start % Buffer::bufferSize();
    Buffer *buff = GetBuffer(Convert(start),offset + item.size());
    buff->Mutex.lock();
    buff->Dirty = true;
    memcpy(buff->Buff+ offset,item.begin(), item.size());
    buff->Mutex.unlock();
}

inline File &File::operator>>(FixString &target)
{
    target = get<FixString>(target.size());
    return *this;
}

inline File &File::operator<<(const FixString &source)
{
    put(source);
    return *this;
}

template<typename T>
auto File::get(const size_t &size) -> const isFixString<T>
{
    auto next = NextPos(ReadCursor, size);
    ReadCursor = next;
    return get<FixString>(next - size, size);
}

template<typename T>
auto File::get(const pos_type &pos, const size_t &size) -> const isFixString<T>
{
    auto start = GetPos(pos, sizeof(T));
    auto offset = start % Buffer::bufferSize();
    Buffer *buff = GetBuffer(Convert(start),offset + sizeof(T));
    return FixString(buff->Buff+offset, size);
}

template<typename T, typename>
File &File::operator<<(const T &source)
{
    put(source);
    return *this;
}

template<typename T, typename>
File &File::operator>>(T &target)
{
    target = get<T>();
    return *this;
}

template<typename T, typename>
void File::put(const T &item)
{
    auto next = NextPos(WriteCursor, sizeof(T));
    put(item, WriteCursor);
    WriteCursor = next;
}

template<typename T, typename>
void File::put(const T &item, const pos_type &pos)
{
    auto start = GetPos(pos, sizeof(T));
    auto offset = start % Buffer::bufferSize();
    Buffer *buff = GetBuffer(Convert(start),offset + sizeof(T));
    buff->Mutex.lock();
    buff->Dirty = true;
    memcpy(buff->Buff+ offset,(const char*)&item,sizeof(T));
    buff->Mutex.unlock();
}

template<typename T>
auto File::peek() -> const Valid<T> &
{
    return get<T>(ReadCursor);
}

template<typename T>
auto File::get(const pos_type &pos) -> const Valid<T> &
{
    auto start = GetPos(pos, sizeof(T));
    auto offset = start % Buffer::bufferSize();
    Buffer *buff = GetBuffer(Convert(start),offset + sizeof(T));
    return *(T*)(buff->Buff+ offset);
}

template<typename T>
auto File::get() -> const Valid<T> &
{
    auto Cursor = ReadCursor;
    ReadCursor = NextPos(ReadCursor, sizeof(T));
    return get<T>(Cursor);
}

inline size_t File::Convert(const pos_type &pos)
{
    return pos / Buffer::bufferSize();
}

inline auto File::NextPos(const pos_type &pos, const pos_type &size) -> pos_type
{
    return GetPos(pos, size) + size;
}

inline void File::reset()
{
    for(auto &i : Buffers)
        if(i.second->Dirty)
            delete i.second;
}

inline auto File::tellg() const -> const pos_type &
{
    return ReadCursor;
}

inline auto File::tellp() const  -> const pos_type &
{
    return WriteCursor;
}

inline auto File::telle() const -> const pos_type &
{
    return FileSize;
}


inline bool File::eof() const
{
    return ReadCursor>=FileSize;
}

inline void File::seekg(const File::pos_type &pos)
{
    if(pos>=FileSize)
        ReadCursor = FileSize;
    else
        ReadCursor = pos;
}

inline void File::seekp(const File::pos_type &pos)
{
    if(pos>=FileSize)
        resize(pos);
    WriteCursor = pos;
}

inline void File::remove()
{
    resize(0);
}

inline void File::Release(const pos_type &pos)
{
    Buffers.erase(Convert(pos));
}

inline void File::Insert(Buffer * const buffer)
{
    Mutex.lock();
    Buffers.insert(std::make_pair(Convert(buffer->Position),buffer));
    Mutex.unlock();
}

inline bool File::Exist(const pos_type &pos)
{
    Mutex.lock();
    bool result = Buffers.find(Convert(pos)) != Buffers.end();
    Mutex.unlock();
    return result;
}

inline bool File::IsEnd(const pos_type &pos)
{
    Mutex.lock();
    bool result = pos >= Stream.size();
    Mutex.unlock();
    return result;
}

#endif // FILE_H
