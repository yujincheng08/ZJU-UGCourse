#ifndef CAMERASCENE_H
#define CAMERASCENE_H

#include <QGraphicsView>

class QCameraViewfinder;
class QCamera;
class QCameraInfo;
class CameraView : public QGraphicsView
{
    Q_OBJECT
private:
    QCameraViewfinder *Viewfinder;
    QCamera *Camera;
    Q_PROPERTY(QCamera* Camera READ getCamera WRITE setCamera NOTIFY CameraChanged)
    QPen *RectPen;
    QGraphicsRectItem *FaceRect;
    QGraphicsProxyWidget *ViewfinderWidget;
    CameraView(QGraphicsScene *Scene, QWidget *parent);
public:
    CameraView(QWidget *parent = nullptr)
        :CameraView(new QGraphicsScene,parent){}
    QCameraViewfinder *getViewfinder();
    inline QCamera *getCamera();
    void setCamera(const QCameraInfo &cameraInfo);
    void setCamera(const QByteArray &deviceName);
    virtual ~CameraView() final override;
    QGraphicsItem *getViewfinderWidget();

signals:
    CameraChanged(QCamera *camera);

private:
    void setCamera(QCamera *camera);
protected:
    void resizeEvent(QResizeEvent *event) override;
};

QCamera *CameraView::getCamera()
{
    return Camera;
}

#endif // CAMERASCENE_H
