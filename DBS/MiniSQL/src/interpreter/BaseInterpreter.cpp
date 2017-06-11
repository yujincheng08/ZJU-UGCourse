#include "BaseInterpreter.h"

using namespace std;

void BaseInterpreter::newConstraint(const string &columnName, const Constraint::Type type)
{
    if(type == Constraint::None) return;
    if(!action->Constraints)
        action->Constraints = make_shared<Constraint>(Constraint());
    //cout<<"Get Constraint "<<columnName<<" type "<<type<<endl;
    if(type==Constraint::PrimaryKey)
    {
        if(action->Constraints->Primary)
            return error("Syntax error: Multiple primary keys.");
        auto column = make_shared<Column>(Column());
        action->Constraints->Primary  = column;
        column->Name = make_shared<string>(columnName);
    }
    else if(type==Constraint::Unique)
    {
        if(!action->Constraints->Uniques)
            action->Constraints->Uniques = make_shared<list<ptr<const Column>>>();
        auto column = make_shared<Column>(Column());
        column->Name = make_shared<string>(columnName);
        action->Constraints->Uniques->push_back(column);
    }
}

void BaseInterpreter::newColumn (const string& columnName, const Column::Type type,
                             const string &tableName)
{
    if(!action->Columns)
        action->Columns = std::make_shared<list<ptr<const Column>>>();
    //std::cout<<"Get column "<<columnName<<" type "<<type<<std::endl;
    if(action->Columns->size()>32U)
        return error("Syntax error: More than 32 attributes in one table.");
    auto column = std::make_shared<Column>(Column());
    column->ColumnType = type;
    column->Name = std::make_shared<string>(columnName);
    if(!tableName.empty())
        column->TableName = std::make_shared<string>(tableName);
    action->Columns->push_back(column);
}
