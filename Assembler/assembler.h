#ifndef ASSEMBLER_H
#define ASSEMBLER_H

#include <QMap>
#include <instructionlist.h>
class Assembler
{
private:
	QMap<QString, uchar> InstList;
public:
	Assembler();
    QByteArray build(const QString & inst,
                     const QString & arg1,
                     const QString & arg2,
                     const QString & arg3,
                     const quint32 &pc,
                     const QMap<QString,unsigned> &markList
                     ) const;
    static const Assembler &getAssembler();
private:
	QByteArray toByteArray(const quint32 & code) const;
	Instruction::InstructionType getType(uchar OpCode) const;
	uchar getRawCode(const uchar &OpCode) const;
	quint32 buildSwitch(const uchar &OpCode,
					   const QString &rd,
					   const QString &rt,
					   const QString &s) const;
	quint32 buildSwitchV(const uchar &OpCode,
						const QString &rd,
						const QString &rt,
						const QString &rs) const;
	quint32 buildLS(const uchar &OpCode,
				   const QString &rt,
				   const QString &offset,
				   const QString &base) const;
	quint32 buildTimes(const uchar &OpCode,
					  const QString &rs,
					  const QString &rt) const;
	quint32 buildROp(const uchar &OpCode,
					const QString &rs,
					const QString &rt,
					const QString &rd) const;
	quint32 buildRI(const uchar &OpCode,
					const QString &rt,
					const QString &rs,
					const QString &imme) const;
	quint32 buildB(const uchar &OpCode,
				   const QString &rs,
				   const QString &rt,
                   const QString &offset,
                   const quint32 &pc,
                   const QMap<QString,unsigned> &markList) const;
	quint32 buildB(const uchar &OpCode,
				   const QString &rs,
				   const uchar &code,
                   const QString &offset,
                   const quint32 &pc,
                   const QMap<QString,unsigned> &markList) const;
	quint32 buildB(const uchar &OpCode,
				   const QString &rs,
                   const QString &offset,
                   const quint32 &pc,
                   const QMap<QString, unsigned> &markList) const;
    quint32 buildBZ(const uchar &OpCode,
                    const QString &rs,
                    const QString &offset,
                    const quint32 &pc,
                    const QMap<QString, unsigned> &markList) const;
	quint32 buildJ(const uchar &OpCode,
                   const QString &index,
                   const QMap<QString,unsigned> &markList) const;
    quint32 buildJR(const uchar &OpCode,
                    const QString &rd,
                    const QString &rs) const;
	quint32 buildJR(const uchar &OpCode,
                    const QString &rs) const;
	quint32 buildMF(const uchar &OpCode,
					const QString &rd) const;
	quint32 buildMT(const uchar &OpCode,
					const QString &rs) const;
	quint32 buildERET(const uchar &OpCode = 0x18) const;
	quint32 buildMC(const uchar &OpCode,
					const QString &rt,
					const QString &rd) const;
	quint32 buildSys(const uchar &OpCode) const;
};

#define assembler Assembler::getAssembler()
//extern const Assembler assembler;
#endif // ASSEMBLER_H
