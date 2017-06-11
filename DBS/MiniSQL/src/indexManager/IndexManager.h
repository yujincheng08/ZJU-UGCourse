#ifndef INDEXMANAGER_H
#define INDEXMANAGER_H

#include <QObject>

class IndexManager : public QObject
{
    Q_OBJECT
public:
    explicit IndexManager(QObject *parent = 0);

signals:

public slots:
};

#endif // INDEXMANAGER_H