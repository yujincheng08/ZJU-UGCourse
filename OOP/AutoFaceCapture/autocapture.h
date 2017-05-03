#ifndef AUTOCAPTURE_H
#define AUTOCAPTURE_H

#include <thread.h>

class Preference;
class AutoCapture : public Thread
{
    Q_OBJECT
private:
    int CountDown;
    int Count;
    Preference *Settings;
    QRect *Rect;
protected:
    virtual void timeout() override final;
public:
    AutoCapture(Preference *settings);
    ~AutoCapture();
private:
    bool compare(const QRect &faceRect);
public slots:
    void autoCapture(const QRect &faceRect);
    void setCountDown(int countDown);
    virtual void pause(bool enabled) override final;
signals:
    void restTime(int sec);
    void fetchRect();
};

#endif // AUTOCAPTURE_H
