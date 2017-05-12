#ifndef INSTRUCTIONLIST_H
#define INSTRUCTIONLIST_H

#include <QString>
class Instruction;
template<typename T>
class QList;
typedef QList<Instruction> InstructionList;

class Instruction
{
private:
    QString Symbol;
    uchar OpCode;
public:
    enum InstructionType
    {Type11 = 0xC0, Type00 = 0x00, Type01 = 0x40, Type10 = 0x80};

    Instruction(const QString & symbol, const uchar opCode)
        :Symbol(symbol),OpCode(opCode){}
    const QString &symbol() const {return Symbol;}
    const uchar &opCode() const {return OpCode;}
    static const InstructionList getInstuctionList();
};


#define instructionList Instruction::getInstuctionList()

#endif // INSTRUCTIONLIST_H
