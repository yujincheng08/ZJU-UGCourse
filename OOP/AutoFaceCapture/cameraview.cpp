#include "cameraview.h"
#include <QCameraViewfinder>
#include <QCamera>
#include <QCameraInfo>
#include <QGraphicsRectItem>
#include <QGraphicsProxyWidget>
#include <QThread>
#include "facedetector.h"
#include "preference.h"
#include "autocapture.h"
CameraView::CameraView(QGraphicsScene *Scene, Preference *settings, QWidget *parent)
    : QGraphicsView(Scene,parent),
      Viewfinder(new QCameraViewfinder),Camera(nullptr),
      RectPen(new QPen(Qt::red)),
      ViewfinderWidget(scene()->addWidget(Viewfinder)),
      FaceRect(scene()->addRect(0,0,0,0,*RectPen)),
      Detector(new FaceDetector()),
      Settings(settings),Capture(new AutoCapture(Settings))
{
    setViewportUpdateMode(QGraphicsView::FullViewportUpdate);
    scene()->setSceneRect(Viewfinder->rect());
    connect(this,Graped,Detector,Detector->recognize);
    connect(this,CountDownSet,Capture,Capture->setCountDown);
    connect(this,Fetched,Capture,Capture->autoCapture);
    connect(Detector,Detector->fetchImage,this,[this](){emit Graped(Viewfinder->grab());});
    connect(Detector,Detector->recognized,this,recognized);
    connect(Capture,Capture->fetchRect,this,[this](){emit Fetched(FaceRect->rect().toRect());});
    connect(Capture,Capture->restTime,this,[this](int rest){emit restTime(rest);});
    Detector->run();
    Capture->run();
}

QCameraViewfinder *CameraView::getViewfinder()
{
    return Viewfinder;
}

void CameraView::setCamera(const QCameraInfo &cameraInfo)
{
    if(cameraInfo.isNull())
        return;
    setCamera(new QCamera(cameraInfo,this));
}

void CameraView::setCamera(const QByteArray &deviceName)
{
    if(deviceName.isNull())
        return;
    setCamera(new QCamera(deviceName,this));
}

QImage CameraView::capture(bool useFrame)
{
    if(useFrame&&FaceRect->rect().width()*FaceRect->rect().height()!=0)
    {
        auto rect = FaceRect->rect().toRect();
        auto ratio = devicePixelRatio();
        rect.setRect(rect.x()*ratio,rect.y()*ratio,rect.width()*ratio,rect.height()*ratio);
        return Viewfinder->grab().toImage().copy(rect);
    }
    return Viewfinder->grab().toImage();
}

CameraView::~CameraView()
{
    Detector->quit();
    delete RectPen;
}

void CameraView::setFrame(bool visual)
{
    Detector->pause(visual);
    FaceRect->setVisible(visual);
}

void CameraView::setCapture(bool enable)
{
    Capture->pause(enable);
}

void CameraView::setCountDown(int countDown)
{
    emit CountDownSet(countDown);
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

void CameraView::recognized(QRect rect)
{
    auto center = rect.center();
    center+= Settings->offset();
    auto size = Viewfinder->size();
    if(Settings->aspect()&&!rect.isNull())
    {
        auto aspect = Settings->aspect_val();
        if(Settings->aspect_scaled())
        {
            if(aspect.width()*rect.height()>
                    aspect.height()*rect.width())
                rect.setWidth(rect.height()*aspect.width()/aspect.height());
            else
                rect.setHeight(rect.width()*aspect.height()/aspect.width());
        }
        else
        {
            rect.setWidth(aspect.width());
            rect.setHeight(aspect.height());
        }
    }
    if(Settings->margin())
    {
        auto margin = Settings->margin_val();
        QMargins margins;
        if(Settings->margin_scaled())
        {
            rect.setWidth(rect.width()*margin.width()/100);
            rect.setHeight(rect.height()*margin.height()/100);
        }
        else
        {
            rect.setWidth(rect.width()+margin.width());
            rect.setHeight(rect.height()+margin.height());
        }
    }
    rect.moveCenter(center);
    if(rect.width()<=size.width())
    {
        if(rect.left()<0)
            rect.moveLeft(0);
        if(rect.right()>size.width())
            rect.moveRight(size.width());
    }
    else
    {
        rect.setWidth(size.width());
        rect.moveLeft(0);
    }
    if(rect.height()<=size.height())
    {
        if(rect.top()<0)
            rect.moveTop(0);
        if(rect.bottom()>size.height())
            rect.moveBottom(size.height());
    }
    else
    {
        rect.setHeight(size.height());
        rect.moveTop(0);
    }

    FaceRect->setRect(rect);
    RectPen->setColor(Settings->color());
    RectPen->setWidth(Settings->thickness());
    FaceRect->setPen(*RectPen);
}
