#include "mycamera.h"
#include <opencv/cv.h>
#include <opencv/highgui.h>
#include <QVBoxLayout>
#include <QLabel>
#include <QPixmap>
#include <QImage>
#include <QtDebug>
#include <QMessageBox>
#include "preference.h"
MyCamera::MyCamera(CvCapture *cam, Preference* setting,QSpinBox* sb, QWidget *parent) :
	QWidget(parent),m_cam(cam),m_layout(new QVBoxLayout(this)),
	m_imagelabel(new QLabel(this)),m_settings(setting),m_showFrame(true),m_scale(1),
#ifdef Q_OS_MAC
    m_cascade((CvHaarClassifierCascade*)cvLoad(QString(QCoreApplication::applicationDirPath()+"/"+"faceRecognize.xml").toStdString().c_str(), 0, 0, 0 )),
#else
	m_cascade((CvHaarClassifierCascade*)cvLoad("faceRecognize.xml", 0, 0, 0 )),
#endif
	m_storage(cvCreateMemStorage(0)),m_face(QRect(0,0,0,0)),
	c_thread(new CaptureThread(sb,m_settings)),m_img(nullptr)
{	
	if(!m_cascade)
		m_cascadeloaded = false;
	else
		m_cascadeloaded = true;
	m_layout->addWidget(m_imagelabel,Qt::AlignCenter);
	m_layout->setContentsMargins(0,0,0,0);
	createCam();
	m_thread = new RecognizeThread(m_cascade,m_scale,this);
	setLayout(m_layout);
	connect(&m_refreshTimer,SIGNAL(timeout()),this,SLOT(m_refresh()));
	connect(&m_recognizeTimer,SIGNAL(timeout()),this,SLOT(m_recognizeFace()));
	connect(&m_captureTimer,SIGNAL(timeout()),this,SLOT(m_capture()));
	connect(m_thread,SIGNAL(allDone(QRect)),this,SLOT(setFace(const QRect&)));
	connect(c_thread,SIGNAL(rest(int)),this,SLOT(m_rest(int)));
	toggleRefresh(true);
	setRecognize(true);
	//m_captureTimer.start(1000);
}

void MyCamera::m_refresh()
{
	IplImage* cvimage = m_refreshFrame();
	m_drawFace(cvimage);
	QImage img;
	m_IplToQImg(cvimage,img);
	m_imagelabel->setPixmap(QPixmap::fromImage(img));
}

void MyCamera::setAutoCap(bool b)
{
	if(b&&!m_captureTimer.isActive())
		m_captureTimer.start(1000);
	else if(!b&&m_captureTimer.isActive())
	{
		m_captureTimer.stop();
		emit showMessage(-1);
	}
}

void MyCamera::toggleRefresh(bool b)
{
	if(b)
	{
		if(!m_recognizeTimer.isActive())
			m_refreshTimer.start(33);
		setRecognize(m_showFrame);
	}
	else
	{
		setRecognize(false);
		m_refreshTimer.stop();
	}
}

void MyCamera::showFrame(bool b)
{
	m_showFrame = b;
	setRecognize(m_showFrame);
}

void MyCamera::setRecognize(bool b)
{
	if(b)
		m_recognizeTimer.start(100);
	else
		m_recognizeTimer.stop();
}

IplImage* MyCamera::m_getFrame()
{
	return m_img;
}

IplImage*  MyCamera::m_refreshFrame()
{
	return m_img = cvQueryFrame(m_cam);
}
void MyCamera::m_rest(int t)
{
	emit showMessage(t);
	if(t==0)
	{
		emit goCapture();
	}
}

void MyCamera::m_capture()
{
	if(!c_thread->isRunning())
	{
		c_thread->update(m_face);
		c_thread->start();
	}
}

void MyCamera::m_IplToQImg(IplImage *cvimage, QImage &image)
{
	if(!cvimage)
	{
		toggleRefresh(false);
		m_captureTimer.stop();
		emit camLost();
		return;
	}
	switch(cvimage->depth)
	{
	case IPL_DEPTH_8U:
		switch(cvimage->nChannels)
		{
		case 3:
            //image = QImage((const uchar*)cvimage->imageData, cvimage->width, cvimage->height,
            //			   QImage::Format_RGB888).rgbSwapped();
            if ( (cvimage->width != image.width()) || (cvimage->height != image.height()) )
			{
				QImage temp(cvimage->width, cvimage->height, QImage::Format_RGB32);
				image = temp;
			}
            int cvIndex = 0; int cvLineStart = 0;
			for (int y = 0; y < cvimage->height; y++) {
				unsigned char red,green,blue;
				cvIndex = cvLineStart;
				for (int x = 0; x < cvimage->width; x++) {
					// DO it
					red = cvimage->imageData[cvIndex+2];
					green = cvimage->imageData[cvIndex+1];
					blue = cvimage->imageData[cvIndex+0];

					image.setPixel(x,y,qRgb(red, green, blue));
					cvIndex += 3;
				}
				cvLineStart += cvimage->widthStep;
            }
			break;
		}
		break;
	default:
		break;
	}
}

QImage MyCamera::capture(bool useFace)
{
	QRect face = m_getRect();
	IplImage *img = m_refreshFrame();
	QImage out;
	m_IplToQImg(img,out);
	if(useFace&&face.width()*face.height()!=0)
	{
		out = out.copy(face);
	}
	return out;
}

void MyCamera::m_recognizeFace()
{
	if(m_cascadeloaded)
	{
		if(!m_thread->isRunning())
		{
			m_thread->setImg(m_getFrame());
			m_thread->start();
		}
	}
}

void MyCamera::setFace(const QRect &face)
{
	m_face = face;
}

void MyCamera::m_drawFace(IplImage *img)
{
	if(!m_showFrame)
		return;
	QRect face = m_getRect();
	if(face.width()*face.height()==0)
		return;
	QColor color = m_settings->color();
	CvScalar colors = CV_RGB(color.red(),color.green(),color.blue());
	if(m_settings->shape())
	{
		cvRectangle(img, cvPoint(face.x(),face.y()),
					cvPoint(face.width()+face.x(),face.height()+face.y()),colors,
					m_settings->thickness());
	}
	else
	{
		CvSize size;size.height = face.height()/2;size.width = face.width()/2;
		cvEllipse(img,cvPoint(face.x()+face.width()/2,face.y()+face.height()/2),
				  size,0,0,360,colors,m_settings->thickness());
	}
}

void MyCamera::createCam()
{
	if(m_cam)
	{
        double width = cvGetCaptureProperty(m_cam,CV_CAP_PROP_FRAME_WIDTH);
        double height = cvGetCaptureProperty(m_cam,CV_CAP_PROP_FRAME_HEIGHT);
		m_settings->setMax(width,height);
		m_scale = width/100;
		if(m_scale<1) m_scale=1;
		m_thread->setScale(m_scale);
		setValid(true);
	}
	else
	{
		setValid(false);
	}

}

void MyCamera::changeCam(CvCapture * cam)
{
	if(cam)
	{
		m_cam = cam;
	}
	createCam();
}

bool MyCamera::isValid()
{
	return valid;
}

void MyCamera::setValid(bool b)
{
	valid = b;
	toggleRefresh(b);
}

QRect MyCamera::m_getRect()
{
	QRect face = m_face;
	if(face.width()*face.height()==0)
		return QRect(0,0,0,0);
	int x1,y1,x2,y2;
	double width = cvGetCaptureProperty(m_cam,CV_CAP_PROP_FRAME_WIDTH);
	double height = cvGetCaptureProperty(m_cam,CV_CAP_PROP_FRAME_HEIGHT);
	QSize margin(m_settings->margin_val()),
			aspect(m_settings->aspect_val());
	QPoint offset(m_settings->offset());
	int mx(0),my(0);
	if(m_settings->margin())
	{
		if(m_settings->margin_scaled())
		{
			mx = margin.width()*m_face.width()/100;
			my = margin.height()*m_face.height()/100;
		}
		else
		{
			mx = margin.width();
			my = margin.height();
		}
	}
	if(m_settings->aspect())
	{
		QPoint cent(face.x()+face.width()/2,face.y()+face.height()/2);
		if(m_settings->aspect_scaled())
		{
			double scale = aspect.width()/aspect.height(),
					o_scale = face.width()/face.height();
			if(scale>o_scale)//放大x
			{
				int width = face.height()*scale;
				face.setX(face.x()-(width-face.width())/2);
				face.setWidth(width);
				if(face.x()<0)
					face.setX(0);
			}
			else
			{
				int height = face.width()*scale;
				face.setY(face.y()-(height-face.height())/2);
				face.setHeight(height);
				if(face.y()<0)
					face.setY(0);
			}
		}
		else
		{
			//qDebug()<<aspect.width();
			face.setX(cent.x()-aspect.width()/2);
			face.setY(cent.y()-aspect.height()/2);
			face.setWidth(aspect.width());
			face.setHeight(aspect.height());
			if(face.x()<0) face.setX(0);
			if(face.y()<0) face.setY(0);
		}
	}
	x1 = face.x() - mx + offset.x();
	x1 = x1>0?x1:0;
	x2 = face.x() +face.width() + mx + offset.x();
	x2 = x2<width?x2:width;
	y1 = face.y() - my + offset.y();
	y1 = y1>0?y1:0;
	y2 = face.y() + face.height() + my+ offset.y();
	y2 = y2<height?y2:height;
	return QRect(x1,y1,x2-x1,y2-y1);
}

RecognizeThread::RecognizeThread(CvHaarClassifierCascade *cascade,
								 double scale, QObject *parent)
	:QThread(parent),m_img(nullptr),m_scale(scale),m_cascade(cascade),
	  m_storage(cvCreateMemStorage(0))
{
}

void RecognizeThread::run()
{
	if(!m_img)
		return;
	CvRect *max = nullptr;
	IplImage* gray = cvCreateImage(cvSize(m_img->width,m_img->height),8,1);
	IplImage* small_img=cvCreateImage(cvSize(cvRound(m_img->width/m_scale)
											 ,cvRound(m_img->height/m_scale)),8,1);
	cvCvtColor(m_img,gray, CV_BGR2GRAY);
	cvResize(gray, small_img, CV_INTER_LINEAR);
	cvEqualizeHist(small_img,small_img); //直方图均衡
	cvClearMemStorage(m_storage);
	//double t = (double)cvGetTickCount();//计时
	CvSeq* face = cvHaarDetectObjects(small_img,
									  m_cascade,m_storage,1.1,2,0,//CV_HAAR_DO_CANNY_PRUNING
									  cvSize(30,30));
	//t = (double)cvGetTickCount() - t;
	//qDebug()<<t/((double)cvGetTickFrequency()*1000.) ;
	int total = face? face->total:0;
	if(total>0)
	{
		max = (CvRect*)cvGetSeqElem(face,0);
	}
	for(int i=1;i<total;i++)
	{
		CvRect* r=(CvRect*)cvGetSeqElem(face,i);
		if(r->width*r->height>max->width*max->height)
			max = r;
	}
	QRect m_face;
	if(max)
		m_face = QRect(max->x*m_scale,max->y*m_scale,
					   max->width*m_scale,max->height*m_scale);
	else
		m_face = QRect(0,0,0,0);
	cvReleaseImage(&gray);
	cvReleaseImage(&small_img);
	emit allDone(m_face);
}

void RecognizeThread::setImg(IplImage *img)
{
	m_img = img;
}

void RecognizeThread::setScale(double scale)
{
	m_scale = scale;
}

CaptureThread::CaptureThread(QSpinBox* sb,Preference* s, QObject *parent)
	:QThread(parent),m_sb(sb),m_setting(s),m_count(0)
{
}
void CaptureThread::run()
{
	int m_sec = m_sb->value();
	if(m_face.height()*m_face.width()!=0&&
			compare(o_face.x(),m_face.x())&&compare(o_face.y(),m_face.y())&&
			compare(o_face.width(),m_face.width())&&compare(o_face.height(),m_face.height()))
	{
		++m_count;
	}
	else
	{
		o_face = m_face;
		m_count = 0;
	}
	int rest_sec = m_sec-m_count+1;
	if(rest_sec==0) m_count = 0;
	emit rest(rest_sec);
}

void CaptureThread::update(const QRect &r)
{
	m_face = r;
}

bool CaptureThread::compare(int a, int b)
{
	return (a-b<m_setting->tolernace())&&(a-b>-m_setting->tolernace());
}
