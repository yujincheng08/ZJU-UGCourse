#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "ui_mainwindow.h"
#include <QString>
#include <QTranslator>
#include "preference.h"
#include <QThread>
class CvCapture;
class MyCamera;
class captureListWidget;
class QDir;
class QSound;
class CameraView;
class QGraphicsView;
class MainWindow : public QMainWindow, private Ui::MainWindow
{
	Q_OBJECT
	CvCapture* cam;
    //MyCamera *camera;
	Preference *prefer;
	QActionGroup *langActionGroup;
	QMenu *lanMenu;
	QTranslator my_translate,qt_translate;
	QLabel *countdown;
	QString m_path;
	QLabel *error;
	QActionGroup *camActionGroup;
	QMenu *camMenu;
	captureListWidget* listWidget;
    CameraView *cameraView;
    int curCam;
	int camCount;
	bool camConnected;
public:
	MainWindow(QWidget *parent = 0);
	~MainWindow();
private slots:
	void showAbout();
	void capture();
	void selectDir();
	void resetCounter();
	void switchLan(QAction*);
	void switchCam(QAction*);
    //void switchCam(int);
	void refreshCam();
	void translate();
	void showMessgae(int);
	void setAutoCap(bool);
public slots:
	void camLost();
private:
	void readSetting();
	void writeSetting();
    //int countCamera();
	void createCamMenu();
	void createLangMenu();
	void createObjects();
	void createConnect();
	void connectCam();
	void disconnectCam();
	void toggleCam(bool);
    QDir getFileName();
protected:
	void closeEvent(QCloseEvent *) override;
};


#endif // MAINWINDOW_H
