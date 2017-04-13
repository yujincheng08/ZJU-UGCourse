#ifndef CAMERASCENE_H
#define CAMERASCENE_H

#include <QGraphicsView>
#include <QImage>

class QCameraViewfinder;
class QCamera;
class QCameraInfo;
class FaceDetector;
class QThread;
class Preference;
class AutoCapture;
class CameraView : public QGraphicsView
{
    Q_OBJECT
private:
    QCameraViewfinder *Viewfinder;
    QCamera *Camera;
    Q_PROPERTY(QCamera* Camera READ getCamera WRITE setCamera NOTIFY CameraChanged)
    QPen *RectPen;
    QGraphicsProxyWidget *ViewfinderWidget;
    QGraphicsRectItem *FaceRect;
    FaceDetector *Detector;
    Preference *Settings;
    AutoCapture *Capture;
private:
    CameraView(QGraphicsScene *Scene,Preference *settings, QWidget *parent);
public:
    CameraView(Preference *settings, QWidget *parent = nullptr)
        :CameraView(new QGraphicsScene,settings,parent){}
    QCameraViewfinder *getViewfinder();
    inline QCamera *getCamera();
    inline FaceDetector *getDetector();
    void setCamera(const QCameraInfo &cameraInfo);
    void setCamera(const QByteArray &deviceName);
    QImage capture(bool useFrame);
    virtual ~CameraView() final override;
signals:
    CameraChanged(QCamera *camera);
    Graped(const QPixmap &pixmap);
    Fetched(const QRect &faceRect);
    CountDownSet(int countDown);
    restTime(int rest);
public slots:
    void setFrame(bool enable);
    void setCapture(bool enable);
    void setCountDown(int countDown);
private:
    void setCamera(QCamera *camera);
protected:
    void resizeEvent(QResizeEvent *event) override;
private slots:
    void recognized(QRect rect);
};

QCamera *CameraView::getCamera()
{
    return Camera;
}

FaceDetector *CameraView::getDetector()
{
    return Detector;
}

#endif // CAMERASCENE_H
