# BufferManager

## Function

Try to implement a effective and smart buffer manager. 

## Usage

`File` is the main structure for programmer to use. It implemented the following interfaces, which are similar to `iostream`.

```C++
slots:
void flush(); //Commit changing to disk. Without calling this function, changing will lost.
void reset(); //Delete all buffer without saving it.
member functions:
File(string fileName); //Constructor
const pos_type &tellg(); //Return the reading cursor position.
const pos_type &tellp(); //Return the writing cursor position.
const pos_type &telle(); //Return the max cursor position: end position.
bool eof(); //If the reading touch the end of file.
void seekg(const pos_type &pos); //Move the reading cursor to pos.
void seekp(const pos_type &pos); //Move the writing cursor to pos.
void resize(const pos_type &pos); //Resize file. Simply truncate it. It will change the file's size immediately, without necessity to flush. 
void remove(); //Lazy delete the file, simply resize(0);
const T &get<T>(); //Get provided type reference.
const T &get<T>(const pos_type &pos); //Get provided type reference at pos, but not changing the cursor.
const T &peek<T>(); //Get the provided type without moving the cursor.
void put(const T& item); //Put item to the buffer. Notablly, data will be overwritten at the cursor instead of appending.
void put<T>(const T &item, const pos_type &pos); //Put a object data to the buff at pos.
File &operator>>(T &target); //Equals to target = get<T>();
File &operator<<(const T &source); //Equals to put<T>(source);
```
`BufferManager` does not provide many function to users. It just provide some setting on the buffer manager.

```C++
static File &open(const std::string &fileName); //Open file. Return a File object. This is the only way to create file object.
static size_t blockSize(); //Return the Max blocksize. To be noticed, blocksize is different to buffersize.
static const size_t &maxBuffer(); //Return the maximum number of buffers. In fact it's never used...
static void setMaxBuffer(const size_t &max); //Set the maximum number of buffers. Default to be  200000.
static size_t bufferSize(); //Return the buffer size of the disk. It depends on the hardware.
```

## Example

```C++
//When writing.
File &file = BufferManager::open("test.db"); //Open a file.
for(int i = 0; i!= 10000; ++i)
  file<<i; //Writing 1-10000 to the file. (or file.put(i);)
FixString str("abcdefghijk", 11); //A fix length string with length = 11
for(int i = 0; i!=10; ++i)
  file<<str; //Writing 10 times to the file. (or file.put(str));
file.flush(); //Remember to flush.

//When reading the file saved by the above programe.
File &file = BufferManager::open("test.db");
for(int i = 0; i!= 10000; ++i)
{
  file>>i;  //Or i = file.get<int>();
  cout<<i<<endl;
}
FixString str(11);
for(int i=0; i!=10; ++i)
{
  file>>str; //Or str = get<FixString>(11);
  cout<<str<<endl;
}
```
`FixString` provides a fix string string for buffer to read and write.

```C++
//Constructors
FixString(const size_t &size);
FixString(const char *const &string);
FixString(const char *const &string, const size_t &size);
FixString(const string &string);
FixString(const QString& string);
FixString(const FixString& string);

const size_t &size() const; //Return the size of the string.
void resize(std::size_t size); //Resize the string.
//Operators to compare two strings.
bool operator ==(const FixString &string) const;
bool operator !=(const FixString &string) const;
bool operator <(const FixString &string) const;
bool operator >(const FixString &string) const;
bool operator >=(const FixString &string) const;
bool operator <=(const FixString &string) const;

FixString &operator =(const FixString &string); //Copy a string.
//Get the char at specific position.
const char &operator [](const std::size_t &index) const;
char &operator [](const std::size_t &index);
//Use the string as a array pointer.
const char *operator +(const std::ptrdiff_t &offset) const;
char *operator +(const std::ptrdiff_t &offset);
const char *operator -(const std::ptrdiff_t &offset) const;
char *operator -(const std::ptrdiff_t &offset);
//Iterators used for foreach
const char *begin() const;
char *begin();
const char *end() const;
char *end();
//Convert the fix string to std::string.
string toString() const;
```

## Attention

1. Adjust element size according to block size. Avoid to read one element but accessing two block.
2. T above the definition of `File` *can only be* inner type and compound type or `FixString`. Neither nor pointers.
3. Both `get()`, `peek()` and `operator>>` get a constant reference of the target and have to write back to the buffer. It's useful if you only read the value. But writing seems costly.
4. Buffer overwrites data after cursor, not insert into cursor. If you are going to delete or inserting something, **use lazy deletion and free space link**.
5. `Pointer` seems dummy and has been eliminated.
6. 怕自己表达不清楚，中文说明一下字节对齐的事儿。
   - Buffer自动对齐到`bufferSize()`，所以用`put(pos)`函数的话，真正写入到文件的位置不一定是`pos`。所以用`tellp()`, `tellg()`等函数来计算位置，不要直接`pos+sizeof(T)`。
   - 举个例子。假设`bufferSize()`是20字节。如果现在在13位置插入一个长度11的定长字符串，实际上它是写入到20-31位置的。这是要注意的。但是直接`get(pos)`是可以返回正确结果的，因为`put`和`get`使用同样的对齐策略。
   - `blockSize()`始终8K，因为最长的record是8K，但是实际上对齐运算时候不按照这个来，是按照bufferSize。
   - 存记录的时候，应该尽量安排好记录储存顺序，用尽量少的block。这实际上是个bin packing问题，NPC问题 = =。
   - 因为字节对齐存在，假设已经算好bin packing，那么*每条*记录存之前，先用`tell`获取开始地址`addr`，然后*连续*使用`operator<<`存数据。然后把`addr`告诉`indexManager`就可以了。读取的时候，可以直接`seek(addr)`，然后按照*写入顺序*使用`operator>>`读数据就可以保数据正确了。
