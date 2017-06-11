#ifndef CONSTRAINT_H
#define CONSTRAINT_H
#include <list>
#include <string>
#include <memory>
class Column;
class Constraint
{
public:
    friend class BaseInterpreter;
    template<typename T>
    using list = std::list<T>;
    using string = std::string;
    template<typename T>
    using ptr = std::shared_ptr<T>;
    enum Type
    {
        None, PrimaryKey, Unique
    };

private:
    ptr<const Column> Primary = nullptr;
    ptr<list<ptr<const Column>>> Uniques = nullptr;
    Constraint();
public:
    auto primaryKey() const -> const ptr<const Column>;
    auto uniques() const -> const ptr<list<ptr<const Column>>>;
};

inline Constraint::Constraint()
{}

inline auto Constraint::primaryKey() const -> const ptr<const Column>
{
    return Primary;
}

inline auto Constraint::uniques() const -> const ptr<list<ptr<const Column>>>
{
    return Uniques;
}

#endif // CONSTRAINT_H
