#include "instructionlist.h"
#include "register.h"
#include "assembler.h"

#define getAndCheckReg(a,b) b = Register::getIndex(a);\
if(b >= Register::RegNum)\
	return 0

Assembler::Assembler()
{
    for(auto i = instructionList.begin();
        i != instructionList.end(); ++i)
        InstList.insert(i->symbol(),i->opCode());
}

QByteArray Assembler::build(
		const QString &inst,
		const QString &arg1,
		const QString &arg2,
        const QString &arg3,
        const quint32 &pc,
        const QMap<QString,unsigned> &markList) const
{
	uchar OpCode = InstList[inst];
	Instruction::InstructionType type = getType(OpCode);
	OpCode = getRawCode(OpCode);
	quint32 code = 0;
	switch(type)
	{
	case Instruction::Type00:
        if(OpCode < 4U)
			code = buildSwitch(OpCode,arg1,arg2,arg3);
		else if(OpCode < 8U)
			code = buildSwitchV(OpCode,arg1,arg2,arg3);
		else if(OpCode < 12U)
		{
			if(arg2.isEmpty())
                code = buildJR(OpCode,arg1);
			else
                code = buildJR(OpCode,arg1,arg2);
		}
		else if(OpCode < 16U)
			code = buildSys(OpCode);
		else if(OpCode < 24U)
		{
            if(OpCode == 16U || OpCode == 18U)
				code = buildMF(OpCode,arg1);
			else if(OpCode == 17U || OpCode == 19U)
				code = buildMT(OpCode,arg1);
		}
		else if(OpCode < 32U)
			code = buildTimes(OpCode,arg1,arg2);
        else if(OpCode < 44U)
			code = buildROp(OpCode,arg1,arg2,arg3);
		break;
	case Instruction::Type01:
        code = buildBZ(OpCode,arg1,arg2,pc,markList);
		break;
	case Instruction::Type10:
		if(OpCode == 0U || OpCode == 4U)
			code = buildMC(OpCode,arg1,arg2);
		else if(OpCode == 24U)
			code = buildERET();
        break;
	case Instruction::Type11:
		if(OpCode < 4U)
            code = buildJ(OpCode,arg1,markList);
		else if(OpCode < 8U)
		{
			if(arg3.isEmpty())
                code = buildB(OpCode,arg1,arg2,pc,markList);
			else
                code = buildB(OpCode,arg1,arg2,arg3,pc,markList);
		}
        else if(OpCode == 15U)
            code=buildRI(OpCode,arg1,"$0",arg2);
        else if(OpCode < 15U)
            code = buildRI(OpCode,arg1,arg2,arg3);
		else if(OpCode)
            code = buildLS(OpCode,arg1,arg2,arg3);
	}
    return toByteArray(code);
}

const Assembler &Assembler::getAssembler()
{
    static Assembler ass;
    return ass;
}

QByteArray Assembler::toByteArray(const quint32 &code) const
{
    return QByteArray((const char *)&code,4);
}

Instruction::InstructionType Assembler::getType(uchar OpCode) const
{
    switch(OpCode>>6 & 0x03)
	{
	case 0:
		return Instruction::Type00;
	case 1:
		return Instruction::Type01;
	case 2:
		return Instruction::Type10;
	case 3:
		return Instruction::Type11;
	}
	return Instruction::Type00;
}

quint32 Assembler::buildSwitch(
		const uchar &OpCode,
		const QString &rd,
		const QString &rt,
		const QString &s) const
{
	quint32 result = 0;
	unsigned regIndex;
	getAndCheckReg(rd,regIndex);
	result |= regIndex << 11;
	getAndCheckReg(rt,regIndex);
	result |= regIndex << 16;
	result |= s.toUInt() << 6;
	result |= (quint32)OpCode;
	return result;
}

quint32 Assembler::buildSwitchV(
		const uchar &OpCode,
		const QString &rd,
        const QString &rt,
        const QString &rs) const
{
    return buildROp(OpCode,rd,rs,rt);
}

quint32 Assembler::buildLS(
		const uchar &OpCode,
		const QString &rt,
		const QString &offset,
		const QString &base) const
{
	quint32 result = 0;
	unsigned regIndex;
	getAndCheckReg(base,regIndex);
	result |= regIndex << 21;
	getAndCheckReg(rt,regIndex);
	result |= regIndex << 16;
	result |= (quint16)offset.toUInt();
	result |= (quint32)OpCode << 26;
	return result;
}

quint32 Assembler::buildTimes(
		const uchar &OpCode,
		const QString &rs,
		const QString &rt) const
{
	quint32 result = 0;
	unsigned regIndex;
	getAndCheckReg(rs,regIndex);
	result |= regIndex << 21;
	getAndCheckReg(rt,regIndex);
	result |= regIndex << 16;
	result |= (quint32)OpCode;
	return result;
}

quint32 Assembler::buildROp(
		const uchar &OpCode,
        const QString &rd,
        const QString &rs,
        const QString &rt) const
{
	quint32 result = 0;
	unsigned regIndex;
	getAndCheckReg(rs,regIndex);
	result |= regIndex << 21;
	getAndCheckReg(rt,regIndex);
	result |= regIndex << 16;
	getAndCheckReg(rd,regIndex);
	result |= regIndex << 11;
	result |= (quint32)OpCode;
	return result;
}

quint32 Assembler::buildRI(
		const uchar &OpCode,
		const QString &rt,
		const QString &rs,
		const QString &imme) const
{
	quint32 result = 0;
	unsigned regIndex;
	getAndCheckReg(rs,regIndex);
	result |= regIndex << 21;
	getAndCheckReg(rt,regIndex);
    result |= regIndex << 16;
    if(imme.startsWith("0x"))
            result |= (quint16)imme.toInt(nullptr, 16);
        else
        result |= (quint16)imme.toUInt();
	result |= (quint32)OpCode << 26;
	return result;
}

quint32 Assembler::buildB(
		const uchar &OpCode,
		const QString &rs,
		const QString &rt,
        const QString &offset,
        const quint32 &pc,
        const QMap<QString,unsigned> &markList) const
{
	unsigned regIndex;
	getAndCheckReg(rt,regIndex);
    return buildB(OpCode,rs,regIndex,offset,pc,markList);
}

quint32 Assembler::buildB(
        const uchar &OpCode,
        const QString &rs,
        const uchar &code,
        const QString &offset,
        const quint32 &pc,
        const QMap<QString, unsigned> &markList) const
{
	quint32 result = 0;
	unsigned regIndex;
	getAndCheckReg(rs,regIndex);
	result |= regIndex << 21;
	regIndex = code;
	result |= regIndex << 16;
    result |= (quint16)(markList.value(offset)-pc-1);
	result |= (quint32)OpCode << 26;
	return result;
}

quint32 Assembler::buildB(
		const uchar &OpCode,
		const QString &rs,
        const QString &offset,
        const quint32 &pc,
        const QMap<QString, unsigned> &markList) const
{
    return buildB(OpCode,rs,0U,offset,pc,markList);
}

quint32 Assembler::buildBZ(
		const uchar &OpCode,
		const QString &rs,
        const QString &offset,
        const quint32 &pc,
        const QMap<QString, unsigned> &markList) const
{
    return buildB(1U,rs,OpCode,offset,pc,markList);
}

quint32 Assembler::buildJ(
		const uchar &OpCode,
        const QString &index,
        const QMap<QString,unsigned> &markList) const
{
	quint32 result = 0;
    result |= (quint32)markList.value(index);
	result |= (quint32)OpCode << 26;
	return result;
}

quint32 Assembler::buildJR(
        const uchar &OpCode,
        const QString &rd,
        const QString &rs) const
{
    return buildROp(OpCode,rd,rs,"$0");
}

quint32 Assembler::buildJR(
		const uchar &OpCode,
        const QString &rs) const
{
    return buildJR(OpCode,"$0",rs);
}

quint32 Assembler::buildMF(
		const uchar &OpCode,
		const QString &rd) const
{
    return buildROp(OpCode,rd,"$0","$0");
}

quint32 Assembler::buildMT(
		const uchar &OpCode,
		const QString &rs) const
{
    return buildROp(OpCode,"$0",rs,"$0");
}

quint32 Assembler::buildERET(const uchar &) const
{
	return 0x42000018;
}

quint32 Assembler::buildMC(
		const uchar &OpCode,
		const QString &rt,
		const QString &rd) const
{
	quint32 result = 0x40000000;
	unsigned regIndex;
	getAndCheckReg(rt,regIndex);
	result |= regIndex << 16;
	getAndCheckReg(rd,regIndex);
	result |= regIndex << 11;
	result |= (quint32)OpCode << 21;
	return result;
}

quint32 Assembler::buildSys(const uchar &OpCode) const
{
	return (quint32)OpCode;
}

uchar Assembler::getRawCode(const uchar& OpCode) const
{
	return OpCode & 0x3F;
}

