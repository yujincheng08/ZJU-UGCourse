#ifndef RECORDMANAGER_H
#define RECORDMANAGER_H

#include <QObject>
#include "../bufferManager/File.h"

class RecordManager : public QObject
{
    Q_OBJECT
public:
    explicit RecordManager(QObject *parent = 0);
    static  bool CreateTable(std::string tableName);
    static  bool DropTable(std::string tableName);
    static  bool DeleteRecords(std::vector<File::pos_type> positions);
    static  bool InsertRecords(std::vector<Record> records);
    static  std::vector<Record> queryRecords(std::vector<Perdicator> conditions);

signals:

public slots:
};

#endif // RECORDMANAGER_H
