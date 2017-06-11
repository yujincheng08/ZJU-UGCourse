#ifndef FIXSTRING_H
#define FIXSTRING_H

#include <string>
#include <QString>
#include <iostream>
class FixString
{
    std::size_t Size;
    char *String;
    int cmp(const FixString &b) const;
public:
    FixString(const std::size_t &size);
    FixString(const char *const &string);
    FixString(const char *const &string, const std::size_t &size);
    FixString(const std::string &string);
    FixString(const QString& string);
    FixString(const FixString& string);
    const std::size_t &size() const;
    void resize(std::size_t size);
    bool operator ==(const FixString &string) const;
    bool operator !=(const FixString &string) const;
    bool operator <(const FixString &string) const;
    bool operator >(const FixString &string) const;
    bool operator >=(const FixString &string) const;
    bool operator <=(const FixString &string) const;
    FixString &operator =(const FixString &string);
    const char &operator [](const std::size_t &index) const;
    char &operator [](const std::size_t &index);
    const char *operator +(const std::ptrdiff_t &offset) const;
    char *operator +(const std::ptrdiff_t &offset);
    const char *operator -(const std::ptrdiff_t &offset) const;
    char *operator -(const std::ptrdiff_t &offset);
    const char *begin() const;
    char *begin();
    const char *end() const;
    char *end();
    std::string toString() const;
    ~FixString();
    friend std::ostream &operator <<(std::ostream &out, const FixString &string);
};

inline FixString::FixString(const std::size_t &size)
    :Size(size),String(new char[Size])
{
    memset(String,0,size);
}

inline FixString::FixString(const char * const &string, const std::size_t &size)
    :Size(size), String(new char[Size])
{
    memcpy(String, string, size);
}

inline FixString::FixString(const char * const &string)
    :FixString(string, strlen(string))
{}

inline FixString::FixString(const std::string &string)
    :FixString(string.c_str(),string.length())
{}

inline FixString::FixString(const QString &string)
    :FixString(string.toStdString())
{}

inline FixString::FixString(const FixString &string)
    :FixString(string.String,string.Size)
{}

inline int FixString::cmp(const FixString &b) const
{
    if(Size!=b.Size)
        return Size -  b.Size;
    return strncmp(String, b.String, Size);
}

inline const std::size_t &FixString::size() const
{
    return Size;
}

inline void FixString::resize(std::size_t size)
{
    if(size>255)
        size = 255;
    char *tmp = new char[size];
    memcpy(tmp, String, std::min(Size, size));
    if(size > Size)
        memset(String + size, 0, size-Size);
    delete String;
    String = tmp;
    Size = size;
}

inline bool FixString::operator ==(const FixString &string) const
{
    return cmp(string) == 0;
}

inline bool FixString::operator !=(const FixString &string) const
{
    return !(*this==string);
}

inline bool FixString::operator <(const FixString &string) const
{
    return cmp(string) < 0;
}

inline bool FixString::operator >(const FixString &string) const
{
    return cmp(string) > 0;
}

inline bool FixString::operator <=(const FixString &string) const
{
    return cmp(string) <= 0;
}

inline FixString &FixString::operator =(const FixString &string)
{
    if(Size == string.Size)
    {
        memcpy(String, string.String, Size);
        return *this;
    }
    else
    {
        Size = string.Size;
        delete String;
        String = new char[Size];
        memcpy(String, string.String, Size);
        return *this;
    }
}

inline bool FixString::operator >=(const FixString &string) const
{
    return cmp(string) >= 0;
}

inline char &FixString::operator [](const std::size_t &index)
{
    return String[index];
}

inline const char &FixString::operator [](const std::size_t &index) const
{
    return String[index];
}

inline char *FixString::operator +(const std::ptrdiff_t &offset)
{
    return String + offset;
}

inline const char *FixString::operator +(const std::ptrdiff_t &offset) const
{
    return String + offset;
}

inline char *FixString::operator -(const std::ptrdiff_t &offset)
{
    return String - offset;
}

inline const char *FixString::operator -(const std::ptrdiff_t &offset) const
{
    return String - offset;
}

inline char *FixString::begin()
{
    return String;
}

inline const char *FixString::begin() const
{
    return String;
}

inline char *FixString::end()
{
    return String + Size;
}

inline std::string FixString::toString() const
{
    return std::string(String, String + Size + 1);
}

inline FixString::~FixString()
{
    if(String)
        delete [] String;
}

inline std::ostream &operator <<(std::ostream &out, const FixString &string)
{
    std::string tmp;
    tmp.assign(string.begin(), string.end());
    out<<tmp;
    return out;
}

inline const char *FixString::end() const
{
    return String + Size;
}
#endif // FIXSTRING_H
