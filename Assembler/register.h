#ifndef REGISTER_H
#define REGISTER_H

#include <QObject>
#include <QByteArray>
#include <QString>
#include <QMap>
class Register : public QObject
{
    Q_OBJECT
public:
	static const unsigned RegNum = 32;
private:
	typedef QMap<QString, unsigned> RegIndex;
	static const RegIndex RegName;
	QByteArray Regs[RegNum];
public:
    explicit Register(QObject *parent = 0);
    QByteArray *getReg(const QString &name);
    QByteArray *getReg(const unsigned index);
    bool set(const QString &name, const QByteArray &value);
    bool set(const unsigned index, const QByteArray &value);

	static unsigned getIndex(const QString &name) ;
signals:

public slots:
};

#endif // REGISTER_H
