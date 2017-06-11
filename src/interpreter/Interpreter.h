#ifndef INTERPRETER_H
#define INTERPRETER_H
#include "BaseInterpreter.h"
#include <QObject>
#include <iostream>
class Scanner;
class Interpreter : public QObject, protected BaseInterpreter
{
    friend class Main;
    friend class Parser;
private:
    size_t LineNo = 0U;
    string ErrorMsg;
    string Near;
    bool Error = false;
    bool isFile = false;
    void *parser = nullptr;
    Scanner *scanner = nullptr;
private:
    Q_OBJECT
    explicit Interpreter(QObject *parent = 0);
    void query();
public:
    virtual ~Interpreter() override final;
protected:
    virtual void error(const string &msg) override final;
    virtual void reset() override final;
signals:
    void parsered(Action);
public slots:
    void run();
    void display(const QString &result);
    void display(const string &result);

};

inline void Interpreter::reset()
{
    ErrorMsg = "";
    Error = false;
    BaseInterpreter::reset();
}

inline void Interpreter::display(const QString &result)
{
    std::cout<<result.toStdString()<<std::endl;
}

inline void Interpreter::display(const Interpreter::string &result)
{
    std::cout<<result<<std::endl;
}

#endif // INTERPRETER_H
