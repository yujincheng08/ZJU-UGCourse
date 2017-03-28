#include "mainwindow.h"
#include "mycamera.h"
//#include <opencv/cv.h>
//#include <opencv/highgui.h>
#include <QAction>
#include <QMessageBox>
#include <QFileDialog>
#include <QDir>
#include <QLabel>
#include <QActionGroup>
#include <QDir>
#include <QStringList>
#include <QSettings>
#include <QCloseEvent>
#include <QSound>
#include <QtDebug>
#include <QKeySequence>
#include <QShortcut>
#include <QListWidget>
#include <QCameraInfo>
#include <QGraphicsView>
#include "capturelistwidget.h"
#include "suredialog.h"
#include "cameraview.h"
MainWindow::MainWindow(QWidget *parent) :
	QMainWindow(parent),prefer(new  Preference(this)),
	m_path("trans"),listWidget(new captureListWidget(this)),
    cam(nullptr),/*camera(nullptr),*/error(nullptr),
    cameraView(new CameraView(this)),
	camCount(0),camConnected(false),curCam(-1)
{
	setupUi(this);
	createObjects();
	createConnect();
	createLangMenu();
	refreshCam();
	readSetting();
	if(!autoCapture->isChecked())
	{
		showMessgae(-1);
	}
    //setCentralWidget(cameraScene);
    //cameraView->setCamera(QCameraInfo::availableCameras().first());
	//camera->show();*/
}

MainWindow::~MainWindow()
{
	if(cam)
		cvReleaseCapture(&cam);
}

void MainWindow::createConnect()
{
    connect(actionExit,SIGNAL(triggered()),this,SLOT(close()));
	connect(actionAbout,SIGNAL(triggered()),this,SLOT(showAbout()));
	connect(actionAbout_Qt,SIGNAL(triggered()),qApp,SLOT(aboutQt()));
	connect(captureButton,SIGNAL(clicked()),this,SLOT(capture()));
	connect(actionPreferences,SIGNAL(triggered()),prefer,SLOT(exec()));
	connect(choseButton,SIGNAL(clicked()),this,SLOT(selectDir()));
	connect(resetButton,SIGNAL(clicked()),this,SLOT(resetCounter()));
	//connect(captureList,SIGNAL(itemSelectionChanged()),this,SLOT(debugOut()));
}

void MainWindow::connectCam()
{
	if(camConnected) return;
    //connect(camera,SIGNAL(goCapture()),this,SLOT(capture()));
    //connect(camera,SIGNAL(showMessage(int)),this,SLOT(showMessgae(int)));
//	connect(autoCapture,SIGNAL(toggled(bool)),camera,SLOT(setAutoCap(bool)));
//	connect(faceCheckBox,SIGNAL(toggled(bool)),camera,SLOT(showFrame(bool)));
	connect(faceCheckBox,SIGNAL(toggled(bool)),autoCapture,SLOT(setEnabled(bool)));
	connect(faceCheckBox,SIGNAL(toggled(bool)),this,SLOT(setAutoCap(bool)));
	connect(new QShortcut(QKeySequence(Qt::Key_Space), this),SIGNAL(activated()),this,SLOT(capture()));
//	connect(camera,SIGNAL(camLost()),this,SLOT(camLost()));
	camConnected = true;
}

void MainWindow::disconnectCam()
{
	if(!camConnected) return;
//	disconnect(camera,SIGNAL(goCapture()),this,SLOT(capture()));
//	disconnect(camera,SIGNAL(showMessage(int)),this,SLOT(showMessgae(int)));
//	disconnect(autoCapture,SIGNAL(toggled(bool)),camera,SLOT(setAutoCap(bool)));
//	disconnect(faceCheckBox,SIGNAL(toggled(bool)),camera,SLOT(showFrame(bool)));
	disconnect(faceCheckBox,SIGNAL(toggled(bool)),autoCapture,SLOT(setEnabled(bool)));
	disconnect(faceCheckBox,SIGNAL(toggled(bool)),this,SLOT(setAutoCap(bool)));
	disconnect(new QShortcut(QKeySequence(Qt::Key_Space), this),SIGNAL(activated()),this,SLOT(capture()));
//	disconnect(camera,SIGNAL(camLost()),this,SLOT(camLost()));
	camConnected = false;
}

void MainWindow::toggleCam(bool b)
{
	verticalLayout->removeWidget(error);
    //verticalLayout->removeWidget(camera);
	if(b)
	{
		/*if(camera->isValid())
		{
			verticalLayout->removeWidget(error);
			captureButton->setDisabled(false);
			autoCapture->setDisabled(false);
			delete error;
			error = nullptr;
		}
		if(camera)
		{
			camera = new QCamera(cam,prefer,spinBox,this);
			camera->setObjectName(QStringLiteral("camera"));
		}
		if(camera->isValid())
			return true;
		else
		{
			camera->setValid(true);
			connectCam();
			return true;
		}*/
//		camera->setVisible(true);
		error->setVisible(false);
//		verticalLayout->insertWidget(0,camera);
		connectCam();
		adjustSize();
		captureButton->setDisabled(false);
		autoCapture->setDisabled(false);
	}
	else
	{
		/*if(camera->isValid())
		{
			disconnectCam();
			verticalLayout->removeWidget(camera);
			camera->setValid(false);
		}
		if(error)
			return false;
		else
		{
			curCam = -1;

			error = new QLabel(tr("No camera detected. Please connect a camera and then restart this appliaction."),this);
			error->setAlignment(Qt::AlignCenter);
			verticalLayout->insertWidget(0,error);
			captureButton->setDisabled(true);
			autoCapture->setDisabled(true);
			return false;
		}*/
		error->setVisible(true);
//		camera->setVisible(false);
		verticalLayout->insertWidget(0,error);
		disconnectCam();
		adjustSize();
		captureButton->setDisabled(true);
		autoCapture->setDisabled(true);
	}
}

void MainWindow::setAutoCap(bool b)
{
    //if(b)
    //	camera->setAutoCap(faceCheckBox->isChecked()&&autoCapture->isChecked());
    //else
    //	camera->setAutoCap(false);
}

void MainWindow::camLost()
{
	QMessageBox::warning(this,tr("Error"),tr("Camera lost, please check and reconnect, then refresh camera list."));
	refreshCam();
	refreshCam();
}


void MainWindow::showMessgae(int t)
{
    //stopSound();
	if(t>spinBox->value())
		countdown->setText(tr("Waiting..."));
	else if(t>0)
	{
		countdown->setText(tr("%1 second(s) left").arg(t));
        QSound::play(":/sound/Ding.wav");
	}
	else if(t==0)
	{
        countdown->setText(tr("A photo has been Captured!"));
        QSound::play(":/sound/Shot.wav");
	}
	else if(t==-1)
	{
		countdown->setText(tr("AutoCapture is closed."));
	}
}

void MainWindow::showAbout()
{
	QMessageBox::about( this, tr("About"),
						tr(	"Programed with Qt5 Framework and OpenCv Libruary.\n"
						   "Creator: Yu Jincheng.\n"
						   "School: Xinhui No.1 Middle School.\n"
						   "Teacher: Tan Liangyou.\n"
						   "Complie time: 201403"));
}

void MainWindow::capture()
{
//	QImage image = camera->capture(faceCheckBox->isChecked());
//	image.setDotsPerMeterX(prefer->DotsPerMeter());
//	image.setDotsPerMeterY(prefer->DotsPerMeter());
//	captureButton->setDisabled(true);
//	camera->setAutoCap(false);
//	int result = 1;
//	if(prefer->waitTime()!=0)
//	{
//		sureDialog dialog(image,prefer->waitTime(),this);
//		result = dialog.exec();
//	}
//	QDir path(getFileName());
//	if(result ==sureDialog::Accepted)
//	{
//		bool pass = true;
//		if(QFile(path.path()).exists())
//			if(QMessageBox::question(this,tr("File is already existed."),
//									 tr("File is already existed.Cover it? (You can add %1 into file name "
//										"in order to change the filename automatically."))==QMessageBox::No)
//				pass = false;
//		if(pass)
//		{
//			image.save(path.path());
//			QListWidgetItem* item = new QListWidgetItem(QIcon(QPixmap::fromImage(image)),
//														path.dirName());
//			item->setData(Qt::UserRole,path.path());
//			listWidget->addItem(item);
//			counter->stepUp();
//		}
//	}
//	captureButton->setDisabled(false);
//    camera->setAutoCap(true && autoCapture->isChecked());
}

void MainWindow::selectDir()
{
	QString fileName = QFileDialog::getExistingDirectory(this,
														 tr("Select save directory"),pathEditor->text());
	if(!fileName.isEmpty())
		pathEditor->setText(fileName);
}

void MainWindow::resetCounter()
{
	counter->setValue(1);
}

void MainWindow::createLangMenu()
{
	lanMenu = new QMenu("Language",this);
	lanMenu->setIcon(QIcon(":/icons/language.png"));
	langActionGroup = new QActionGroup(this);
	connect(langActionGroup,SIGNAL(triggered(QAction*)),this,SLOT(switchLan(QAction*)));
	menuSetting->addMenu(lanMenu);
#ifdef Q_OS_MAC
    QDir dir(QCoreApplication::applicationDirPath());
    dir.cd(m_path);
    m_path = dir.absolutePath();
#else
	QDir dir(m_path);
#endif
	QStringList fileNames = dir.entryList(QStringList("my_*.qm"));
	for(int i = 0; i!= fileNames.length();i++)
	{
		QString locale = fileNames[i];
		locale.remove(0,locale.indexOf('_')+1);
		locale.chop(3);

		QTranslator translator;
		translator.load(fileNames[i],m_path);
		QString language = translator.translate("MainWindow","English");
		QAction *action = new QAction(QString("&%1 %2").arg(i+1).arg(language),this);
		action->setCheckable(true);
		action->setData(locale);

		lanMenu->addAction(action);
		langActionGroup->addAction(action);

		if(locale == QLocale::system().name())
		{
			action->setChecked(true);
			switchLan(action);
		}
	}
}

void MainWindow::createObjects()
{

    //creat cam
    //camera = new MyCamera(cam,prefer,spinBox,this);
    //camera->setObjectName(QStringLiteral("camera"));
    error = new QLabel();
    error->setAlignment(Qt::AlignCenter);
    /*cam = cvCreateCameraCapture(0);
    if(cam)
    {
        camera = new MyCamera(cam,prefer,spinBox,this);
        camera->setObjectName(QStringLiteral("camera"));
        verticalLayout->insertWidget(0,camera);
    }
    else
    {
        error = new QLabel(this);
        error->setAlignment(Qt::AlignCenter);
        verticalLayout->insertWidget(0,error);
        captureButton->setDisabled(true);
        autoCapture->setDisabled(true);
    }*/

	//creat listWidget
	verticalLayout_2->addWidget(listWidget);
    verticalLayout->insertWidget(0,cameraView);
	//creat countdown
	countdown = new QLabel(this);
	statusBar()->addPermanentWidget(countdown);

	//creat camMenu
	camMenu = new QMenu("Camera",this);
	camMenu->setIcon(QIcon(":/icons/camera.png"));

	//initialize

	formatBox->addItem(".jpg",".jpg");
	formatBox->addItem(".png",".png");

	pathEditor->setText(QDir::currentPath()/*+"/Untile %1.jpg"*/);
	fileNameEdit->setText(tr("Untitled %1"));

    //sound

}

QDir MainWindow::getFileName()
{
	QString count;
	if(prefer->addZero())
		count = QString("%1").arg(counter->value(),
										  prefer->addZero_digit(), 10, QChar('0'));
	else
		count = QString::number(counter->value());
	QString fileName(fileNameEdit->text().arg(count));
	fileName += formatBox->currentData().toString();
    return pathEditor->text()+"/"+fileName;
}

void MainWindow::switchLan(QAction* action)
{
	QString locale = action->data().toString();
	my_translate.load("my_"+locale,m_path);
	qt_translate.load("qt_"+locale,m_path);
	qApp->installTranslator(&my_translate);
	qApp->installTranslator(&qt_translate);
	translate();
}

void MainWindow::translate()
{
	lanMenu->setTitle(QApplication::translate("MainWindow","language"));
	camMenu->setTitle(QApplication::translate("MainWindow","camera"));
	retranslateUi(this);
	prefer->translate();
	listWidget->translate();
	error->setText(tr("No camera detected. Please connect a camera and then refresh camera list."));
	/*if(cam)
	{
		QString tip(tr("The camera."));
		camera->setStatusTip(tip);
		camera->setToolTip(tip);
	}
	else
	{
		error->setText(tr("No camera detected. Please connect a camera and then restart this appliaction."));
	}
	camMenu->clear();
	createCamMenu();*/
}

void MainWindow::readSetting()
{
	QSettings setting("AutoFaceCapture","AutoFaceCapture",this);
	prefer->loadSettings(setting);
	pathEditor->setText(setting.value("savePath",pathEditor->text()).toString());
	fileNameEdit->setText(setting.value("fileName",fileNameEdit->text()).toString());
	autoCapture->setChecked(setting.value("AutoCapture",autoCapture->isChecked()).toBool());
	spinBox->setValue(setting.value("AutoCaptureTime",spinBox->value()).toInt());
	faceCheckBox->setChecked(setting.value("RecognizeFace",faceCheckBox->isChecked()).toBool());
	counter->setValue(setting.value("counter",counter->value()).toInt());
	formatBox->setCurrentIndex(setting.value("format",formatBox->currentIndex()).toInt());
	listWidget->setItemSize(setting.value("itemSize",listWidget->itemSize()).toInt());
	restoreGeometry(setting.value("geometry",saveGeometry()).toByteArray());
    writeSetting();
}
void MainWindow::writeSetting()
{
	QSettings setting("AutoFaceCapture","AutoFaceCapture",this);
	prefer->toSettings(setting);
	setting.setValue("savePath",pathEditor->text());
	setting.setValue("fileName",fileNameEdit->text());
	setting.setValue("AutoCapture",autoCapture->isChecked());
	setting.setValue("AutoCaptureTime",spinBox->value());
	setting.setValue("RecognizeFace",faceCheckBox->isChecked());
	setting.setValue("counter",counter->value());
	setting.setValue("format",formatBox->currentIndex());
	setting.setValue("itemSize",listWidget->itemSize());
	setting.setValue("geometry", saveGeometry());
}

void MainWindow::closeEvent(QCloseEvent *event)
{
		writeSetting();
		event->accept();
}

//int MainWindow::countCamera()
//{
//	for(int i=0;;i++)
//    {
//        CvCapture *p = cvCreateCameraCapture(i);
//        bool result = (bool)p;
//        cvReleaseCapture(&p);
//        if(!result)
//		{
//            return i;
//        }

//	}
    //return 0;
//}

void MainWindow::createCamMenu()
{
	camMenu->clear();
	camActionGroup = new QActionGroup(this);
    connect(camActionGroup,SIGNAL(triggered(QAction*)),this,SLOT(switchCam(QAction*)));
    menuSetting->addMenu(camMenu);
    auto cameraList = QCameraInfo::availableCameras();
    auto defaultCamera = QCameraInfo::defaultCamera();
    for(auto i = cameraList.begin(); i!= cameraList.end(); ++i)
	{
        //QAction *action = new QAction(QString("%1#%2").arg(tr("Camera")).arg(i+1),this);
        auto action = new QAction(i->description(),this);
		action->setCheckable(true);
        action->setData(i->deviceName().toLatin1());
		camMenu->addAction(action);
		camActionGroup->addAction(action);
        if(*i == defaultCamera)
        {
            action->setChecked(true);
            switchCam(action);
        }
	}
    camMenu->addSeparator();
	QAction *refreshCam = new QAction(tr("Refresh camera list"),this);
	connect(refreshCam,SIGNAL(triggered()),this,SLOT(refreshCam()));
	camMenu->addAction(refreshCam);
}


void MainWindow::switchCam(QAction* action)
{
    cameraView->setCamera(action->data().toByteArray());
    //int camnum = action->data().toInt();
    //switchCam(camnum);
}

//void MainWindow::switchCam(int num)
//{

    //cameraView->setCamera(QCameraInfo::availableCameras().first());

//	if(num!= curCam && num < camCount)
//	{
//		cvReleaseCapture(&cam);
//		CvCapture *ncam = cvCreateCameraCapture(num);
//		if(ncam)
//		{
//			cam = ncam;
//			curCam = num;
//			camActionGroup->actions().at(num)->setChecked(true);
//			camera->changeCam(ncam);
//			toggleCam(true);
//		}
//	}
//}

void MainWindow::refreshCam()
{
//	camera->setValid(false);
	curCam = -1;
    //camCount = countCamera();
    //qDebug()<<camCount;
	createCamMenu();
    //if(camCount==0)
    //{
        //toggleCam(false);
    //}
    //else
        //switchCam(0);
}

