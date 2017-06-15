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
    size_t Count;
public:
	explicit Phraser(QObject *parent = 0);
    void setDocument(const QTextDocument *document);
    const size_t &count();
signals:
    void BuildDone();
public slots:
    void Build(QByteArray &data);
    //void
};

inline const size_t &Phraser::count()
{
    return Count;
}

#endif // PHRASER_H
