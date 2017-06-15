#include "RecordManager.h"
#include "../bufferManager/File.h"
#include "../bufferManager/BufferManager.h"
#include "../interpreter/Column.h"

using namespace std;
constexpr RecordManager::pos_type RecordManager::INVALID_POS;

File &RecordManager::OpenTableFile(const string &tableName) {
    return BufferManager::open(tableName + ".tbl");
}

void RecordManager::FlushTableFile(const string &tableName) {
    OpenTableFile(tableName).flush();
}

void RecordManager::CreateTable(const string &tableName) {
    File &file = OpenTableFile(tableName);
    file.seekp(0);
    // last write record, first invalid
    file << RecordManager::INVALID_POS << RecordManager::INVALID_POS;
    file.put<pos_type>(0);
    file << RecordManager::INVALID_POS;
    // lastWritePos, firstInvalidPos, maxPos, firstValidPos
    // if both invalid it's initail empty state
}

void RecordManager::DropTable(const string &tableName) {
    // use File class delete method
    auto &file = OpenTableFile(tableName);
    file.remove();
}

void RecordManager::DeleteRecords(const string &tableName, const vector<pos_type> &positions) {
    auto metaData = getMetaData(tableName);
    auto &lastValid = get<0>(metaData);
    auto &firstInvalid = get<1>(metaData);
    auto &firstValid = get<3>(metaData);
    File &file = OpenTableFile(tableName);
    for (auto &offset : positions) {
        if (offset == lastValid) {
            // delete this from tail
            pos_type previousValid;
            file.seekg(offset);
            file.get<bool>();
            file >> previousValid;
            lastValid = previousValid;
        }
        if (offset == firstValid) {
            // delete this from head
            pos_type nextValid;
            file.seekg(offset);
            file.get<bool>();
            file.get<pos_type>();
            file >> nextValid;
            firstValid = nextValid;
        }

        pos_type nextOfPrevious, previousOfNext;
        file.seekg(offset);
        file.get<bool>();
        file >> previousOfNext >> nextOfPrevious;
        if (previousOfNext != RecordManager::INVALID_POS) {
            file.seekg(previousOfNext);
            file.get<bool>();
            file.get<pos_type>();
            file.seekp(file.tellg());
            file << nextOfPrevious;
        }
        if (nextOfPrevious != RecordManager::INVALID_POS) {
            file.seekg(nextOfPrevious);
            file.get<bool>();
            file.seekp(file.tellg());
            file << previousOfNext;
        }
        file.seekp(offset);
        file << false << firstInvalid;
        firstInvalid = offset;
    }
    setMetaData(tableName, metaData);
}

void RecordManager::InsertRecord(const string &tableName, const Record &record) {
    auto &file = RecordManager::OpenTableFile(tableName);
    auto metaData = RecordManager::getMetaData(tableName);
    auto &lastWritePos = get<0>(metaData);
    auto &firstInvalidPos = get<1>(metaData);
    auto &maxPos = get<2>(metaData);
    auto &firstValidPos = get<3>(metaData);
    pos_type nextPos;
    if (firstInvalidPos == RecordManager::INVALID_POS) {
        if (lastWritePos == RecordManager::INVALID_POS) {
            // empty initial just write at here
            firstValidPos = file.tellg();
        } else {
            // no invalid postion now, use maxPos
            // seek to the next write position
            getRecordByOffset(file, record, maxPos);
        }
        nextPos = file.tellg();
    } else {
        nextPos = firstInvalidPos;
        file.seekg(firstInvalidPos);
        file.get<bool>();
        file >> firstInvalidPos;
        // assert valid = false
    }

    if (lastWritePos != RecordManager::INVALID_POS) {
        // link this to last
        file.seekg(lastWritePos);
        bool valid;
        pos_type tmp;
        file >> valid >> tmp;
        file.seekp(file.tellg());
        file << nextPos;
    }

    file.seekp(nextPos);
    // valid
    // means end of valid list

    // structure of each record:
    // if record valid
    // valid: bool, previous: pos_type, next: pos_type, [record]
    // invalid
    // valid: bool, next:pos_type
    // valid list is double list to make random insertion and deletion easier
    // invalid list is single list because we only insert or delete in head
    file << true << lastWritePos;
    file << RecordManager::INVALID_POS;

    for (auto &column : record) {
        auto type = column.type();
        auto &rawData = *(column.name());
        if (type == Column::Int) {
            file << stoi(rawData);
        } else if (type == Column::Float) {
            file << stod(rawData);
        } else if (1 <= type && type < Column::Int) {
            file << FixString(rawData.c_str(), getColumnSize(column));
        } else {
            // Undefined should not happen
        }
    }
    // write lastWrite to file
    lastWritePos = nextPos;
    if (firstValidPos == RecordManager::INVALID_POS) {
        firstValidPos  = nextPos;
    }
    if (nextPos > maxPos) {
        maxPos = nextPos;
    }
    // write back meta data first valid, last write, max pos, firstValidPos
    setMetaData(tableName, make_tuple(lastWritePos, firstInvalidPos, maxPos, firstValidPos));
}

auto RecordManager::queryRecordsByOffsets(const string &tableName, const vector<pos_type> &offsets, const Record &templateRecord) -> vector<Record> {
    vector<Record> result;
    auto &file = OpenTableFile(tableName);
    for (const auto &offset: offsets) {
        result.emplace_back(getRecordByOffset(file,  templateRecord, offset));
    }
    return result;
}

auto RecordManager::queryRecordsOffsets(const string &tableName) -> vector<pos_type> {
    auto metaData = getMetaData(tableName);
    auto firstValid = get<3>(metaData);
    auto &file = OpenTableFile(tableName);
    file.seekg(firstValid);
    vector<pos_type>  result;
    while(firstValid != RecordManager::INVALID_POS) {
        result.emplace_back(firstValid);
        file.seekg(firstValid);
        file.get<bool>();
        file.get<pos_type>();
        file >> firstValid;
    }
    return result;
}

auto RecordManager::getMetaData(const string &tableName) -> MetaData {
    pos_type lastWritePos, firstInvalidPos, maxPos, firstValidPos;
    auto &file = OpenTableFile(tableName);
    file.seekg(0);
    file >> lastWritePos >> firstInvalidPos >> maxPos >> firstValidPos;
    return make_tuple(lastWritePos, firstInvalidPos, maxPos, firstValidPos);
}

 void RecordManager::setMetaData(const string &tableName, const MetaData &metaData) {
     auto &file = OpenTableFile(tableName);
     file.seekp(0);
     file << get<0>(metaData) << get<1>(metaData) << get<2>(metaData) << get<3>(metaData);
 }

auto RecordManager::getRecordByOffset(File &file,const Record &record, const pos_type &offset) -> Record {
     file.seekg(offset);
     file.get<bool>();
     file.get<pos_type>();
     file.get<pos_type>();

     Record result;
     for (auto &column : record) {
         auto type = column.type();
         Column newColumn;
         string encodedString;
         if (type == Column::Int) {
             int data;
             file >> data;
             encodedString = to_string(data);
         } else if (type == Column::Float) {
             double data;
             file >> data;
             encodedString = to_string(data);
         } else if (1 <= type && type < Column::Int) {
             FixString data(getColumnSize(column));
              file >> data;
              encodedString = data.toString();
         } else {
         }
         newColumn.Name = make_shared<string>(encodedString);
         newColumn.TableName = column.TableName;
         newColumn.ColumnType = column.ColumnType;
         result.emplace_back(newColumn);
     }
     return result;
 }

auto RecordManager::makeTestRecord(int id) -> Record {
    Column intCol;
    intCol.ColumnType = Column::Int;
    intCol.Name = make_shared<string>(to_string(id));
    Column floatCol;
    floatCol.ColumnType = Column::Float;
    floatCol.Name = make_shared<string>("3.141592");
    Column charCol;
    charCol.ColumnType = 8;
    charCol.Name = make_shared<string>("123456789");
    return {intCol, charCol, floatCol};
}

size_t RecordManager::getColumnSize(const Column& col) {
     if (col.type() == Column::Int)
         return sizeof(int);
     else if(col.type() == Column::Float)
         return sizeof(double);
     else if(col.type()<=255)
         return col.type() * sizeof(char);
     else
         return 0U;
 }

size_t RecordManager::getRecordSize(const Record &record) {
     int size = 0;
     for(const auto &col : record) {
         size += getColumnSize(col);
     }
     return size;
 }
