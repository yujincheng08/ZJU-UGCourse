#ifndef RECORDMANAGER_H
#define RECORDMANAGER_H

#include <vector>
#include <string>

class File;
class Column;

class RecordManager
{
    friend int main(int argc, char *argv[]);
    friend void testInsert(const std::string &tableName);
public:
    template<typename T>
    using vector = std::vector<T>;
    using Record = vector<Column>;
    using pos_type = std::streamoff;
    using MetaData = std::tuple<pos_type, pos_type, pos_type, pos_type>;
    using string  = std::string;
    constexpr static pos_type INVALID_POS = 0xffffffff;
    static File &OpenTableFile(const string &tableName);
    static void FlushTableFile(const string &tableName);
    // check if the table already exits before calling this
    static void CreateTable(const string &tableName);
    // check if the table already exits before calling this
    static void DropTable(const string &tableName);
    //
    static void DeleteRecords(const string &tableName, const vector<pos_type> &positions);
    static void InsertRecord(const string &tableNam, const Record &record);
    static vector<Record> queryRecordsByOffsets(const string &tableName, const vector<pos_type> &offsets,  const Record &templateRecord);
    static vector<pos_type> queryRecordsOffsets(const string &tableName);
private:
    // lastWritePos, firstInvalidPos, maxPos, firstValidPos
    static MetaData getMetaData(const string &tableName);
    static void setMetaData(const string &tableName, const MetaData &metaData);
    static Record getRecordByOffset(File &file, const Record &templateRecord, const pos_type &offset);
    static size_t getColumnSize(const Column &col);
    static size_t getRecordSize(const Record &record);
    static void seekToPreviousFiled(File &file, pos_type begin);
    static void seekToRecordData(File &file, pos_type);
#ifdef TEST
    static Record makeTestRecord(int id);
#endif

};

#endif // RECORDMANAGER_H
