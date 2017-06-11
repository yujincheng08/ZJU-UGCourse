#ifndef COLUMN_H
#define COLUMN_H

#include <string>
#include <memory>
class Column
{
public:
    friend class BaseInterpreter;
    using Type = unsigned;
    constexpr static Type Undefined = 0U;
    constexpr static Type Int = 256U;
    constexpr static Type Float = 257U;
    using string = std::string;
    template<typename T>
    using ptr = std::shared_ptr<T>;
private:
    Type ColumnType;
    ptr<string> Name = nullptr;
    ptr<string> TableName = nullptr;
    Column();
public:
    auto type() const -> const Type &;
    auto name() const -> const ptr<const string>;
    auto tableName() const -> const ptr<const string>;
};

inline Column::Column()
{}

inline auto Column::type() const  -> const Type &
{
    return ColumnType;
}

inline auto Column::name() const -> const ptr<const string>
{
    return Name;
}

inline auto Column::tableName() const -> const ptr<const string>
{
    return TableName;
}

#endif // COLUMN_H
