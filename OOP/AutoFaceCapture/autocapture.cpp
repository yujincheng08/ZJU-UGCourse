#include "autocapture.h"
#include <QRect>
#include <QSpinBox>
#include <QSound>
#include "preference.h"
AutoCapture::AutoCapture(Preference *settings)
    :CountDown(0),Count(0),Settings(settings),
      Rect(new QRect)
{
    setTimeout(1000);
}

AutoCapture::~AutoCapture()
{
    delete Rect;
}

bool AutoCapture::compare(const QRect &faceRect)
{
    auto tolerance = Settings->tolernace();
    return faceRect.x()-Rect->x()<tolerance &&
           Rect->x()-faceRect.x()<tolerance &&
           faceRect.y()-Rect->y()<tolerance &&
           Rect->y()-faceRect.x()<tolerance &&
           faceRect.width()-Rect->width()<tolerance &&
           Rect->width()-faceRect.width()<tolerance &&
           faceRect.height()-Rect->height()<tolerance &&
           Rect->height()-faceRect.height()<tolerance;
}

void AutoCapture::autoCapture(const QRect &faceRect)
{
    //*Rect = faceRect;
    if(!Rect->isNull()&&compare(faceRect)&&CountDown)
        ++Count;
    else
    {
        *Rect = faceRect;
        Count = 0;
    }
    int rest = CountDown - Count + 1;
    if(rest==0)
        Count = 0;
    emit restTime(rest);

}

void AutoCapture::setCountDown(int countDown)
{
    CountDown = countDown;
}

void AutoCapture::pause(bool enabled)
{
    Count = 0;
    Thread::pause(enabled);
}

void AutoCapture::timeout()
{
    emit fetchRect();
}
