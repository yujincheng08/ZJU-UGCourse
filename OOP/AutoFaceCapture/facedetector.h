#ifndef FACEDETECTOR_H
#define FACEDETECTOR_H

#include "thread.h"

namespace cv
{
    class CascadeClassifier;
    class Mat;
}

class QCameraViewfinder;
class QTimer;

class FaceDetector : public Thread
{
    Q_OBJECT
private:
    cv::CascadeClassifier *Classifier;
public:
    FaceDetector();
protected:
    virtual void timeout() override final;
public slots:
    void recognize(const QPixmap &pixmap);
    //void setRecognizeImage(const QPixmap &pixmap);
signals:
    void recognized(const QRect &faceRect);
    void fetchImage();
};

#endif // FACEDETECTOR_H
