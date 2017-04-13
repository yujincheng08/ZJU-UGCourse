#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "ui_mainwindow.h"
#include "preference.h"

class MyCamera;
class captureListWidget;
class QDir;
class QSound;
class CameraView;
class QGraphicsView;
class MainWindow : public QMainWindow, private Ui::MainWindow
{
	Q_OBJECT
    QSound *Ding, *Shot;
	Preference *prefer;
	QActionGroup *langActionGroup;
	QMenu *lanMenu;
    QTranslator *translator;
	QLabel *countdown;
	QLabel *error;
	QActionGroup *camActionGroup;
	QMenu *camMenu;
	captureListWidget* listWidget;
    CameraView *cameraView;
	bool camConnected;
    QString *language;
public:
	MainWindow(QWidget *parent = 0);
	~MainWindow();
private slots:
	void showAbout();
	void capture();
	void selectDir();
	void resetCounter();
	void switchLan(QAction*);
    void switchLan(QString path);
    //void switchCam(int);
	void translate();
    void showMessage(int);
    void setAutoCapture(bool);
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
    QDir getFileName();
protected:
	void closeEvent(QCloseEvent *) override;
};


#endif // MAINWINDOW_H
