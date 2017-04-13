#include "facedetector.h"
#include <opencv2/objdetect.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <QImage>
#include <QTimer>
#include <QPixmap>
#include <QCameraViewfinder>
#include <vector>
#include <QThread>
#include <QFile>
#include <QTemporaryFile>
FaceDetector::FaceDetector()
    :Classifier(new cv::CascadeClassifier)
{
    auto xml = QTemporaryFile::createNativeFile(":/xml/faceRecognize.xml");
    setTimeout(100);
    //bool result = Classifier->load("faceRecognize.xml");
    bool result = Classifier->load(xml->fileName().toStdString().c_str());
    xml->remove();
    if(!result)
        exit(EXIT_FAILURE);
}

void FaceDetector::timeout()
{
    emit fetchImage();
}

void FaceDetector::recognize(const QPixmap &pixmap)
{
    using namespace cv;
    using namespace std;
    const static int scale = pixmap.width()/pixmap.devicePixelRatio()/200;

    QImage image = pixmap.toImage().scaledToWidth(pixmap.width()/(pixmap.devicePixelRatio()*scale)).
            convertToFormat(QImage::Format_Indexed8);
    Mat mat(image.height(),image.width(),
                CV_8UC1,const_cast<uchar*>(image.bits()),
                static_cast<size_t>(image.bytesPerLine()));
    equalizeHist(mat,mat);
    vector<Rect> faces;
    Classifier->detectMultiScale(mat,faces,1.1,2,CASCADE_SCALE_IMAGE,cv::Size(20, 20));
    QRect rect(0,0,0,0);

    if(!faces.empty())
    {
        for(auto i = faces.begin();i!=faces.end();++i)
        {
            if(i->width*i->height > rect.height()*rect.width())
                rect.setRect(i->x,i->y,i->width,i->height);
        }
        rect.setRect(rect.x()*scale,rect.y()*scale,rect.width()*scale,rect.height()*scale);
    }
    emit recognized(rect);
}
