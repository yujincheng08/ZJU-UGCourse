#ifndef PHRASER_H
#define PHRASER_H

#include <QObject>
#include <QMap>
class QTextDocument;
class Phraser : public QObject
{
	Q_OBJECT
private:
    const QTextDocument *Document;
    QMap<QString,unsigned> MarkList;
public:
	explicit Phraser(QObject *parent = 0);
    void setDocument(const QTextDocument *document);

signals:
    void BuildDone();
public slots:
    void Build(QByteArray &data);
    //void
};

#endif // PHRASER_H
