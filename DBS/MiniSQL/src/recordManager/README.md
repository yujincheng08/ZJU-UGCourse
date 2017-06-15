# RecordManager

## Function
An interface to insert, query, delele records via ```BufferManager```

## Usage
```
static  void CreateTable(std::string tableName); // create a file to store a table (one table per file)
static  void DropTable(std::string tableName); // delete the file
static  void DeleteRecords(std::vector<File::pos_type> positions); // simply set the valid sign to false,
static  File::pos_type InsertRecords(Record records); // append the record to file
static  std::vector<Record> queryRecords(); // not sure about this method's parameter
```
## Contract
store a bool before each record to represent if this record is deleted
