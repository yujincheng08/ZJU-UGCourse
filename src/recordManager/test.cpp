#include "RecordManager.h"
#include <QCoreApplication>
#include <iostream>
#include "../bufferManager/File.h"
#include "../bufferManager/BufferManager.h"
#include "../interpreter/Column.h"

void testInsert(const std::string &tableName) {
        RecordManager::DropTable(tableName);
        RecordManager::CreateTable(tableName);
        auto Record = RecordManager::makeTestRecord(666);
        for (int i = 0; i < 5; i++) {
            auto Record = RecordManager::makeTestRecord(i);
            RecordManager::InsertRecord(tableName, Record);
        }
        RecordManager::FlushTableFile(tableName);
        auto offsets = RecordManager::queryRecordsOffsets(tableName);
        auto records = RecordManager::queryRecordsByOffsets(tableName, offsets, Record);
        auto metaData = RecordManager::getMetaData(tableName);
        for (auto &record: records) {
            for(auto &column: record) {
                std::cout << (*column.name()).c_str() << "\n";
            }
        }
        RecordManager::DeleteRecords(tableName, {offsets[2]});
        std::cout << "deleted, remaining: \n";
        offsets = RecordManager::queryRecordsOffsets(tableName);
        records = RecordManager::queryRecordsByOffsets(tableName, offsets, Record);
        metaData = RecordManager::getMetaData(tableName);
        for (auto &record: records) {
            for(auto &column: record) {
                std::cout << (*column.name()).c_str() << "\n";
            }
        }
        std::cout << "re insert \n";
        for (int i = 0; i < 5; i++) {
            auto Record = RecordManager::makeTestRecord(i);
            RecordManager::InsertRecord(tableName, Record);
        }
        offsets = RecordManager::queryRecordsOffsets(tableName);
        records = RecordManager::queryRecordsByOffsets(tableName, offsets, Record);
        metaData = RecordManager::getMetaData(tableName);
        for (auto &record: records) {
            for(auto &column: record) {
                std::cout << (*column.name()).c_str() << "\n";
            }
        }
        std::cout << "done\n" ;
}

void testWriteFile(const std::string &tableName) {
       // RecordManager::DropTable(tableName);
       // RecordManager::CreateTable(tableName);
        auto &file = BufferManager::open(tableName + ".tbl");

        //auto record = RecordManager::makeTestRecord();
        File::pos_type a = 0xFFFFFFFF, b= 456, c, d;
        file.seekp(98);
        file << a << b;
        file.flush();
        file.seekg(98);
        file >> c >> d;
}

int main(int argc, char *argv[]) {
    std::string tableName("test");
    QCoreApplication app(argc, argv);

     testInsert(tableName);
    //testWriteFile(tableName);
//    testWriteFile(tableName);
    return app.exec();
}
