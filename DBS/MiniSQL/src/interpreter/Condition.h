#ifndef CONDITION_H
#define CONDITION_H
#include <memory>
class Column;
class Condition
{
public:
    friend class BaseInterpreter;
    template<typename T>
    using ptr = std::shared_ptr<T>;
    enum Type
    {
        Underfined, Node, NotEqual, Equal,
        GreaterThan, LessThan, LessEqual,
        GreaterEqual, And, Or, Not
    };
private:
    Type Op = Underfined;
    ptr<Condition> FirstOperand = nullptr;
    ptr<Condition> SecondOperand = nullptr;
    ptr<Column> Value = nullptr;
    Condition();
public:
    auto op() const -> const Type &;
    auto firstOperand() const -> const ptr<const Condition>;
    auto secondOperand() const -> const ptr<const Condition>;
    auto value() const -> const ptr<const Column>;
};

inline Condition::Condition()
{}

inline auto Condition::op() const -> const Type &
{
    return Op;
}

inline auto Condition::firstOperand() const -> const ptr<const Condition>
{
    return FirstOperand;
}

inline auto Condition::secondOperand() const -> const ptr<const Condition>
{
    return SecondOperand;
}

inline auto Condition::value() const -> const ptr<const Column>
{
    if(Op == Node)
        return Value;
    else
        return nullptr;
}

#endif // CONDITION_H
