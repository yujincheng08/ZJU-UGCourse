#include "Interpreter.h"
#include "Column.h"
#include <ostream>

using namespace std;

static ostream &operator<<(ostream &out, const Column c)
{

    if(c.name())
        out<<*c.name();
    out<<"{"<<c.type() <<"}";
    if(c.tableName())
        out<<"["<< *c.tableName() << "]";
    return out;
}

static void printCondition(Interpreter::ptr<const Condition> c)
{
    if(!c) return;
    cout<<"(";
    if(c->firstOperand() && c->firstOperand()->op()==Condition::Node)
        cout<<*c->firstOperand()->value();
    else
        printCondition(Interpreter::ptr<const Condition>(c->firstOperand()));
    cout<<" "<<c->op()<<" ";
    if(!c->secondOperand())
    {
        cout<<")";
        return;
    }
    if(c->secondOperand()->op()==Condition::Node)
        cout<<*c->secondOperand()->value();
    else
        printCondition(Interpreter::ptr<const Condition>(c->secondOperand()));
    cout<<")";
}

extern void testInterpreter(Action *action)
{
    cout<<"Type: "<<action->actionType()<<endl;
    if(action->tableName())
    {
        cout<<"Table List: "<<endl;
        for(auto i : *action->tableName())
        {
            cout<<"\t"<<*i<<endl;
        }
    }
    if(action->indexName())
    {
        cout<<"Index Name: "<<
              "\t"<<*action->indexName()<<endl;
    }
    if(action->columns())
    {
        cout<<"Column List: "<<endl;
        for(auto i : *action->columns())
        {
            cout<<"\t"<<*i<<endl;
        }
    }
    if(action->constraints())
    {
        cout<<"Constraints List: "<<endl;
        auto i = *action->constraints();
        if(i.primaryKey())
            cout<<"\tprimaryKey: "<< *i.primaryKey()->name()<<endl;
        if(i.uniques())
        {
            cout<<"\tUniques: "<<endl;
            for(auto j : *i.uniques())
            {
                cout<<"\t"<<*j<<endl;
            }
        }
    }
    if(action->conditions())
    {
        printCondition(action->conditions());
        cout<<endl;
    }

}
