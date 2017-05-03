#include "mainwindow.h"
#include "mycamera.h"
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
#include <QKeySequence>
#include <QShortcut>
#include <QListWidget>
#include <QCameraInfo>
#include <QGraphicsView>
#include <QTranslator>
#include <QSound>
#include "capturelistwidget.h"
#include "suredialog.h"
#include "cameraview.h"

MainWindow::MainWindow(QWidget *parent) :
	QMainWindow(parent),prefer(new  Preference(this)),
    listWidget(new captureListWidget(this)),
    camActionGroup(nullptr),error(nullptr),
    cameraView(new CameraView(prefer,this)),
    camConnected(false)
{
	setupUi(this);
	createObjects();
	createConnect();
    createLangMenu();
    createCamMenu();
    readSetting();
	if(!autoCapture->isChecked())
        showMessage(-1);
}

MainWindow::~MainWindow()
{

}

void MainWindow::createConnect()
{
    connect(actionExit,SIGNAL(triggered()),this,SLOT(close()));
	connect(actionAbout,SIGNAL(triggered()),this,SLOT(showAbout()));
	connect(actionAbout_Qt,SIGNAL(triggered()),qApp,SLOT(aboutQt()));
	connect(captureButton,SIGNAL(clicked()),this,SLOT(capture()));
    connect(actionPreferences,actionPreferences->triggered,this,[this](){
        setAutoCapture(false);
        prefer->exec();
        setAutoCapture(true);
    });
	connect(choseButton,SIGNAL(clicked()),this,SLOT(selectDir()));
    connect(resetButton,SIGNAL(clicked()),this,SLOT(resetCounter()));
    connect(faceCheckBox,SIGNAL(toggled(bool)),autoCapture,SLOT(setEnabled(bool)));
    connect(autoCapture,SIGNAL(toggled(bool)),this,SLOT(setAutoCapture(bool)));
    connect(faceCheckBox,SIGNAL(toggled(bool)),this,SLOT(setAutoCapture(bool)));
    connect(faceCheckBox,faceCheckBox->toggled,cameraView,cameraView->setFrame);
    connect(spinBox,SIGNAL(valueChanged(int)),cameraView,SLOT(setCountDown(int)));
    connect(cameraView,cameraView->restTime,this,showMessage);
}

void MainWindow::camLost()
{
	QMessageBox::warning(this,tr("Error"),tr("Camera lost, please check and reconnect, then refresh camera list."));
}


void MainWindow::showMessage(int t)
{
    //stopSound();
	if(t>spinBox->value())
		countdown->setText(tr("Waiting..."));
	else if(t>0)
	{
        //QSound::play(":/sound/Ding.wav");

        countdown->setText(tr("%1 second(s) left").arg(t));
        Ding->play();
	}
	else if(t==0)
	{
        //capture();
        countdown->setText(tr("A photo has been Captured!"));
        //QSound::play(":/sound/Shot.wav");
        Shot->play();
        capture();
	}
	else if(t==-1)
	{
		countdown->setText(tr("AutoCapture is closed."));
    }
}

void MainWindow::setAutoCapture(bool b)
{
    if(!b)
        showMessage(-1);
    cameraView->setCapture(b && autoCapture->isChecked());
}

void MainWindow::showAbout()
{
	QMessageBox::about( this, tr("About"),
                        tr(	"Programed with Qt Framework and OpenCv Libruary.\n"
                           "Creator: Jincheng Yu, Yucong Yuan, Qi Liu, Qiuru Peng.\n"
                           "School: Zhejiang University.\n"
                           "Teacher: Lanqing Hu.\n"
                           "Complie time: %1").arg(__DATE__));
}

void MainWindow::capture()
{
    cameraView->setCapture(false);
    QImage image = cameraView->capture(faceCheckBox->isChecked());
    image.setDotsPerMeterX(prefer->DotsPerMeter());
    image.setDotsPerMeterY(prefer->DotsPerMeter());
    captureButton->setDisabled(true);
    int result = 1;
    if(prefer->waitTime()!=0)
    {
        sureDialog dialog(image,prefer->waitTime(),this);
        result = dialog.exec();
    }
    QDir path(getFileName());
    if(result ==sureDialog::Accepted)
    {
        bool pass = true;
        if(QFile(path.path()).exists())
            if(QMessageBox::question(this,tr("File is already existed."),
                                     tr("File is already existed.Cover it? (You can add %1 into file name "
                                        "in order to change the filename automatically."))==QMessageBox::No)
                pass = false;
        if(pass)
        {
            image.save(path.path());
            QListWidgetItem* item = new QListWidgetItem(QIcon(QPixmap::fromImage(image)),
                                                        path.dirName());
            item->setData(Qt::UserRole,path.path());
            listWidget->addItem(item);
            counter->stepUp();
        }
    }
    captureButton->setDisabled(false);
    cameraView->setCapture(autoCapture->isChecked());
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
    QDir dir;
    auto files = dir.entryInfoList(QStringList("afc.*.qm"));
    dir.setPath(":/translation/");
    files << dir.entryInfoList(QStringList("afc.*.qm"));
    for(auto i = files.begin(); i!=files.end();++i)
    {
        QTranslator trans;
        trans.load(i->absoluteFilePath());
        QString language = trans.translate("MainWindow","English");
        QAction *action = new QAction(QString("&%1").arg(language),this);
        action->setCheckable(true);
        action->setData(i->absoluteFilePath());
        lanMenu->addAction(action);
        langActionGroup->addAction(action);
        if(i->fileName()==QString("afc.%1.qm").arg(QLocale::system().name()))
        {
            action->setChecked(true);
            switchLan(action);
        }
    }
}

void MainWindow::createObjects()
{

    //creat cam
    cameraView->setObjectName(QStringLiteral("camera"));
    cameraView->setToolTip(tr("The camera"));
    cameraView->setStatusTip(tr("The Camera"));
    error = new QLabel();
    error->setAlignment(Qt::AlignCenter);

	//creat listWidget
	verticalLayout_2->addWidget(listWidget);
    //verticalLayout->insertWidget(0,cameraView);
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
    Ding = new QSound(":/sound/Ding.wav",this);
    Shot = new QSound(":/sound/Shot.wav",this);

    translator = new QTranslator(this);

    language = new QString;
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
    switchLan(action->data().toString());
}

void MainWindow::switchLan(QString path)
{
    if(path.isEmpty())
        return;
    *language = path;
    translator->load(path);
    if(translator->isEmpty())
        return;
    qApp->installTranslator(translator);
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
}

void MainWindow::readSetting()
{
	QSettings setting("AutoFaceCapture","AutoFaceCapture",this);
	prefer->loadSettings(setting);
	pathEditor->setText(setting.value("savePath",pathEditor->text()).toString());
	fileNameEdit->setText(setting.value("fileName",fileNameEdit->text()).toString());
	autoCapture->setChecked(setting.value("AutoCapture",autoCapture->isChecked()).toBool());

	spinBox->setValue(setting.value("AutoCaptureTime",spinBox->value()).toInt());
    cameraView->setCountDown(spinBox->value());
	faceCheckBox->setChecked(setting.value("RecognizeFace",faceCheckBox->isChecked()).toBool());
	counter->setValue(setting.value("counter",counter->value()).toInt());
	formatBox->setCurrentIndex(setting.value("format",formatBox->currentIndex()).toInt());
	listWidget->setItemSize(setting.value("itemSize",listWidget->itemSize()).toInt());
    restoreGeometry(setting.value("geometry",saveGeometry()).toByteArray());
    if(!setting.value("language").isNull())
    {
        auto lan = setting.value("language").toString();
        auto menu = lanMenu->actions();
        for(auto action : menu)
        {
            if(action->data().toString()==lan && QFile::exists(lan))
            {
                action->setChecked(true);
                switchLan(lan);
            }
        }
    }
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
    setting.setValue("language",*language);
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    writeSetting();
    event->accept();
}

void MainWindow::createCamMenu()
{
	camMenu->clear();
    if(camActionGroup)
        delete camActionGroup;
    camActionGroup = new QActionGroup(this);
    menuSetting->addMenu(camMenu);
    auto cameraList = QCameraInfo::availableCameras();
    auto defaultCamera = QCameraInfo::defaultCamera();
    for(auto i = cameraList.begin(); i!= cameraList.end(); ++i)
	{
        auto action = new QAction(i->description(),this);
		action->setCheckable(true);
        action->setData(i->deviceName().toLatin1());
		camMenu->addAction(action);
		camActionGroup->addAction(action);
        connect(action,action->triggered,this,[this,action]
        {
            cameraView->setCamera(action->data().toByteArray());
        });
        if(*i == defaultCamera)
        {
            action->setChecked(true);
            cameraView->setCamera(*i);
        }
	}
    camMenu->addSeparator();
	QAction *refreshCam = new QAction(tr("Refresh camera list"),this);
    connect(refreshCam,refreshCam->triggered,this,[this]{createCamMenu();});
	camMenu->addAction(refreshCam);
    if(camActionGroup->actions().isEmpty())
    {
        verticalLayout->removeWidget(cameraView);
        verticalLayout->insertWidget(0,error);
    }
    else
    {
        verticalLayout->removeWidget(error);
        verticalLayout->insertWidget(0,cameraView);
    }
}

