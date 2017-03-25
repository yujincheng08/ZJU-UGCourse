#ifndef SUREDIALOG_H
#define SUREDIALOG_H

#include "ui_suredialog.h"
#include <QTimer>
class QImage;

class sureDialog : public QDialog, private Ui::sureDialog
{
	Q_OBJECT
private:
	QTimer countdown;
	int time;
	QString text;
public:
	explicit sureDialog(const QImage&,int,QWidget* = 0);
private slots:
	void refreshTime();
public slots:
	int exec() override;
};

#endif // SUREDIALOG_H
