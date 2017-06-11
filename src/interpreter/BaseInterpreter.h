#ifndef BASEINTERPRETER_H
#define BASEINTERPRETER_H
#include "Action.h"
#include "Column.h"
#include "Constraint.h"
#include "Condition.h"
#include <memory>

class BaseInterpreter
{
public:
    enum
    {
        QUIT = -0x1, EXEC = -0x2, UNEXPETED = -0x3
    };
    template<typename T>
    using list = std::list<T>;
    using string = std::string;
    template<typename T>
    using ptr = std::shared_ptr<T>;
    virtual void error(const string &msg) = 0;
protected:
    Action *action = nullptr;
    virtual void reset();
    BaseInterpreter();
    virtual ~BaseInterpreter();
public:
    void setActionType(const Action::Type type);
    void addTableName(const string &tableName);
    void newColumn(const string& columnName, const Column::Type type,
                   const string &tableName = string());
    void newConstraint(const string &columnName, const Constraint::Type type);
    void addIndexName(const string &indexName);
    Condition *newCondition(const string& columnName, const Column::Type,
                      const string &tableName = string());
    Condition *newCondition(const Condition::Type type,
                      Condition *first, Condition * second = nullptr);
    void newCondition(Condition *condition);
};

inline void BaseInterpreter::setActionType(const Action::Type type)
{
    if(action->ActionType==Action::Undefined)
        action->ActionType = type;
    else
        error("Syntax error: Multiple actions.");
}

inline void BaseInterpreter::addTableName(const string &tableName)
{
    if(!action->TableName)
        action->TableName = std::make_shared<list<ptr<const string>>>();
    action->TableName->push_back(std::make_shared<const string>(tableName));
}

inline void BaseInterpreter::addIndexName(const string &indexName)
{
    if(!action->IndexName)
        action->IndexName = std::make_shared<string>(indexName);
    else
        error("Syntax error: Multiple index name.");
}

inline Condition *BaseInterpreter::newCondition(const string &columnName, const Column::Type type,
                                            const string &tableName)
{
    Condition * condition = new Condition();
    condition->Op = Condition::Node;
    condition->Value = std::make_shared<Column>(Column());
    condition->Value->ColumnType = type;
    condition->Value->Name = std::make_shared<string>(columnName);
    if(!tableName.empty())
        condition->Value->TableName = std::make_shared<string>(tableName);
    return condition;
}

inline Condition *BaseInterpreter::newCondition(const Condition::Type type,
                                            Condition *first, Condition * second)
{
    Condition * condition = new Condition();
    condition->Op = type;
    condition->FirstOperand = ptr<Condition>(first);
    condition->SecondOperand = ptr<Condition>(second);
    return condition;
}

inline void BaseInterpreter::newCondition(Condition *condition)
{
    action->Conditions = std::shared_ptr<Condition>(condition);
}

inline void BaseInterpreter::reset()
{
    if(action)
        delete action;
    action = new Action();
}

inline BaseInterpreter::BaseInterpreter()
{}

inline BaseInterpreter::~BaseInterpreter()
{
    delete action;
}

#endif // BASEINTERPRETER_H
