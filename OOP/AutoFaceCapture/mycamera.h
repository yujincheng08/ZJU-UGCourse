//#ifndef MYCAMERA_H
//#define MYCAMERA_H

//#include <QWidget>
//#include <QTimer>
//#include <opencv/cv.hpp>
//#include <opencv/highgui.h>
//#include <QThread>
//#include <QTime>
//class QLabel;
//class RecognizeThread;
//class QVBoxLayout;
//class Preference;
//class QSpinBox;
//class CaptureThread;
//class MyCamera : public QWidget
//{
//	Q_OBJECT
//	QLabel *m_imagelabel;
//	QVBoxLayout *m_layout;
//	CvMemStorage *m_storage;
//	CvHaarClassifierCascade *m_cascade;
//	CvCapture *m_cam;
//	Preference* m_settings;
//	QTimer m_refreshTimer,m_recognizeTimer,m_captureTimer;
//	RecognizeThread *m_thread;
//	CaptureThread *c_thread;
//	bool m_cascadeloaded;
//	QRect m_face;
//	bool m_showFrame;
//	double m_scale;
//	IplImage* m_img;
//	bool valid;
//public:
//	MyCamera(CvCapture *,Preference*,QSpinBox*,QWidget* = nullptr);
//	//MyCamera(CvCapture*,int,int);
//	void setMargin(int);
//	void setOffset(const QPointF&);
//	QImage capture(bool);
//	void changeCam(CvCapture *);
//	bool isValid();
//signals:
//	void goCapture();
//	void showMessage(int);
//	void camLost();
//public slots:
//	void setAutoCap(bool);
//	void toggleRefresh(bool);
//	void setValid(bool);
//private slots:
//	void m_refresh();
//	void m_IplToQImg(IplImage *,QImage&);
//	void m_recognizeFace();
//	void setFace(const QRect&);
//	void m_rest(int);
//	void m_capture();
//	void showFrame(bool);
//	void setRecognize(bool);
//private:
//	void m_drawFace(IplImage*);
//	void createCam();
//	IplImage* m_getFrame();
//	IplImage *m_refreshFrame();
//	QRect m_getRect();
//};

////class RecognizeThread:public QThread
////{
////	Q_OBJECT
////	IplImage* m_img;
////	CvHaarClassifierCascade* m_cascade;
////	double m_scale;
////	CvMemStorage *m_storage;
////public:
////	RecognizeThread(CvHaarClassifierCascade*,double,QObject* =0);
////	RecognizeThread(double,QObject* =0);
////	void setImg(IplImage *);
////	void setScale(double);
////signals:
////	void allDone(QRect);
////protected:
////	void run() Q_DECL_OVERRIDE;
////};

//class CaptureThread:public QThread
//{
//	Q_OBJECT
//	QRect m_face;
//	QRect o_face;
//	QSpinBox* m_sb;
//	Preference* m_setting;
//	int m_count;
//public:
//	CaptureThread(QSpinBox*,Preference*,QObject* =0);
//	void update(const QRect&);
//	void setSec(int);
//signals:
//	void rest(int);
//protected:
//	void run() Q_DECL_OVERRIDE;
//private:
//	bool compare(int,int);
//};
//#endif // MYCAMERA_H
