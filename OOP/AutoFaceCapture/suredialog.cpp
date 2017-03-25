#include "suredialog.h"
#include <QImage>
#include <QSize>
#include <QShortcut>
#include <QKeySequence>
sureDialog::sureDialog(const QImage& image,int t,QWidget *parent) :
	QDialog(parent),time(t+1)
{
	setupUi(this);
	text = tr("Sure(%1)(F11)");
	QSize size = image.size();
	imageLabel->resize(size);
	imageLabel->setPixmap(QPixmap::fromImage(image));
	refreshTime();
	connect(&countdown,SIGNAL(timeout()),this,SLOT(refreshTime()));
	connect(sureButton,SIGNAL(clicked()),this,SLOT(accept()));
	connect(retryButton,SIGNAL(clicked()),this,SLOT(reject()));
	connect(new QShortcut(QKeySequence(Qt::Key_F11), this),SIGNAL(activated()),this,SLOT(accept()));
}

void sureDialog::refreshTime()
{
	if(--time == 0)
	{
		accept();
	}
	else
	{
		sureButton->setText(text.arg(time));
	}
}
int sureDialog::exec()
{
	countdown.start(1000);
	return QDialog::exec();
}
