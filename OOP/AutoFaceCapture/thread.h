#ifndef THREAD_H
#define THREAD_H

#include <QObject>

class QTimer;
class QThread;

class Thread : public QObject
{
    Q_OBJECT
private:
    bool Enabled;
    QTimer *Timer;
    QThread *MyThread;
public:
    Thread();
    void setTimeout(int msec);
public slots:
    virtual void run();
    virtual void quit();
    virtual void pause(bool enabled);
private slots:
    virtual void start();
    virtual void stop();
protected:
    virtual void timeout() = 0;

};

#endif // THREAD_H
