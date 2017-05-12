#include "register.h"
#include <QRegExp>
using std::make_pair;
const Register::RegIndex Register::RegName =
{
	make_pair(QString("zero"), 0x00),
	make_pair(QString("at"),   0x01),
	make_pair(QString("v0"),   0x02),
	make_pair(QString("v1"),   0x03),
	make_pair(QString("a0"),   0x04),
	make_pair(QString("a1"),   0x05),
	make_pair(QString("a2"),   0x06),
	make_pair(QString("a3"),   0x07),
	make_pair(QString("t0"),   0x08),
	make_pair(QString("t1"),   0x09),
	make_pair(QString("t2"),   0x0A),
	make_pair(QString("t3"),   0x0B),
	make_pair(QString("t4"),   0x0C),
	make_pair(QString("t5"),   0x0D),
	make_pair(QString("t6"),   0x0E),
	make_pair(QString("t7"),   0x0F),
	make_pair(QString("s0"),   0x10),
	make_pair(QString("s1"),   0x11),
	make_pair(QString("s2"),   0x12),
	make_pair(QString("s3"),   0x13),
	make_pair(QString("s4"),   0x14),
	make_pair(QString("s5"),   0x15),
	make_pair(QString("s6"),   0x16),
	make_pair(QString("s7"),   0x17),
	make_pair(QString("t8"),   0x18),
	make_pair(QString("t9"),   0x19),
	make_pair(QString("k0"),   0x1A),
	make_pair(QString("k1"),   0x1B),
	make_pair(QString("gp"),   0x1C),
	make_pair(QString("sp"),   0x1D),
	make_pair(QString("fp"),   0x1E),
	make_pair(QString("ra"),   0x1F)
	};
Register::Register(QObject *parent) :
	QObject(parent)
{
    for(unsigned i = 0; i < RegNum; i++)
        Regs[i].resize(4);
}

QByteArray *Register::getReg(const QString &name)
{
    return getReg(getIndex(name));
}

QByteArray *Register::getReg(const unsigned index)
{
	if(index >= RegNum)
        return nullptr;
    return Regs + index;
}

bool Register::set(const QString &name, const QByteArray &value)
{
    return set(getIndex(name), value);
}

bool Register::set(const unsigned index, const QByteArray &value)
{
	if(index >= RegNum || (index == 0 && value.toInt() != 0))
        return false;
    Regs[index] = value;
    return true;
}

unsigned Register::getIndex(const QString &rawName)
{
    if(rawName[0]!='$'&&rawName[0]!='r'&&rawName[0]!='R')
        return RegNum;
    QString name = rawName.mid(1);
    QRegExp regExp("\\d+");
    if(regExp.exactMatch(name))
    {
        unsigned index = name.toUInt();
        if(index < RegNum)
            return index;
        else
            return RegNum;
    }
    if(RegName.contains(name))
        return RegName.value(name);
    else
        return RegNum;
}
