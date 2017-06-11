#ifndef API_H
#define API_H

#include <QObject>

class API : public QObject
{
    Q_OBJECT
public:
    explicit API(QObject *parent = 0);

signals:

public slots:
};

#endif // API_H