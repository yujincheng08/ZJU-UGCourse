#include "cameraview.h"
#include <QCameraViewfinder>
#include <QCamera>
#include <QCameraInfo>
#include <QGraphicsRectItem>
#include <QGraphicsProxyWidget>
CameraView::CameraView(QGraphicsScene *Scene, QWidget *parent)
    : QGraphicsView(Scene,parent),
      Viewfinder(new QCameraViewfinder),Camera(nullptr),
      RectPen(new QPen),FaceRect(scene()->addRect(0,0,0,0,*RectPen)),
      ViewfinderWidget(scene()->addWidget(Viewfinder))
{
    scene()->setSceneRect(Viewfinder->rect());
}

QCameraViewfinder *CameraView::getViewfinder()
{
    return Viewfinder;
}

void CameraView::setCamera(const QCameraInfo &cameraInfo)
{
    if(cameraInfo.isNull())
        return;
    //Camera = new QCamera(cameraInfo);
    //Camera->setViewfinder(Viewfinder);
    //Camera->start();
    setCamera(new QCamera(cameraInfo,this));
}

void CameraView::setCamera(const QByteArray &deviceName)
{
    if(deviceName.isNull())
        return;
    setCamera(new QCamera(deviceName,this));
}

CameraView::~CameraView()
{
    delete RectPen;
}

QGraphicsItem *CameraView::getViewfinderWidget()
{
    return ViewfinderWidget;
}

void CameraView::setCamera(QCamera *camera)
{
    if(Camera && Camera->isAvailable())
    {
        Camera->stop();

        delete Camera;
    }
    Camera = camera;
    if(!Camera->isAvailable()) return;
    Camera->setViewfinder(Viewfinder);
    Camera->start();
    emit CameraChanged(Camera);
}

void CameraView::resizeEvent(QResizeEvent *event)
{
    QGraphicsView::resizeEvent(event);
    fitInView(ViewfinderWidget,Qt::KeepAspectRatio);
}
