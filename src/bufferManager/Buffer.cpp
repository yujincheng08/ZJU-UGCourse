#include "Buffer.h"
#include "File.h"
#include <QDir>
#include <QString>
namespace windows
{
#include <windows.h>
}

static QString partition()
{
    QDir current = QDir::current();
    while(current.cdUp());
    return current.absolutePath();
}

static size_t calBufferSize()
{
    windows::DWORD SectorsPerCluster,lpBytesPerSector;
    windows::GetDiskFreeSpace(partition().toStdWString().c_str(),
                         &SectorsPerCluster,&lpBytesPerSector,nullptr,nullptr);
    return static_cast<size_t>(SectorsPerCluster*lpBytesPerSector);
}


const size_t Buffer::BufferSize = calBufferSize();

Buffer::Buffer(File &file,
                      const pos_type &position, const size_t &empty)
    : Buff(new char[bufferSize()]),f(file),
      Stream(f.Stream), Position(position), Size(0U)
{
    if(position < Stream.size())
    {
        Stream.seek(position);
        Size = Stream.read(Buff,bufferSize());
    }
    if(empty > Size)
    {
        memset(Buff + Size, 0, empty - Size);
        Size = empty;
    }
}

Buffer::~Buffer()
{
    f.Release(this->Position);
    delete [] Buff;
}

