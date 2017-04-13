#include "preference.h"
#include <QSettings>
#include <QColorDialog>
Preference::Preference(QWidget *parent) :
	QDialog(parent)
{
	isCustom = false;
	setupUi(this);
	colorLineEdit->setValidator(new QRegExpValidator(QRegExp("\\#[0-9a-fA-F]{6}"),this));
	setData();
	makeConnection();
	calSize();
	isCustom = true;
}

QColor  Preference::color() const
{
	return colorLineEdit->text();
}
int  Preference::thickness() const
{
	return thicknessSpinBox->value();
}
QPoint  Preference::offset() const
{
	return QPoint(offsetSpinBox_x->value(),offsetSpinBox_y->value());
}
bool  Preference::aspect() const
{
	return aspectBox->isChecked();
}
bool  Preference::aspect_scaled() const
{
	return scaledButton->isChecked();
}
QSize  Preference::aspect_val ()const
{
    return ssize;
}
bool  Preference::margin() const
{
	return marginBox->isChecked();
}
bool  Preference::margin_scaled() const
{
	return scaledButton_2->isChecked();
}
QSize  Preference::margin_val()const
{
    return smargin;
}
int  Preference::tolernace()const
{
	return toleranceSpinBox->value();
}

bool Preference::addZero()
{
	return addZeroBox->isChecked();
}

int Preference::addZero_digit()
{
	return digitSpinBox->value();
}

int Preference::waitTime()
{
	return waitSpinBox->value();
}

int Preference::dpi() const
{
	return dpiBox->value();
}

int Preference::DotsPerMeter() const
{
	return dpiBox->value() / unitBox->itemData(0).toDouble();
}

int Preference::unit() const
{
	return unitBox->currentIndex();
}

int Preference::defaultIndex() const
{
	return defaultBox->currentIndex();
}

void Preference::setDpi(int t)
{
	dpiBox->setValue(t);
}

void Preference::setUnit(int t)
{
	unitBox->setCurrentIndex(t);
}

void Preference::setDefaultIndex(int t)
{
	defaultBox->setCurrentIndex(t);
}

void Preference::translate()
{
	retranslateUi(this);
}

void Preference::setColor(const QColor& t)
{
	colorLineEdit->setText(t.name());
}

void Preference::setThickness(int t)
{
	thicknessSpinBox->setValue(t);
}

void Preference::setOffset(const QPoint& t)
{
	offsetSpinBox_x->setValue(t.x());
	offsetSpinBox_y->setValue(t.y());
}

void Preference::setAspect(bool t)
{
	aspectBox->setChecked(t);
}
void Preference::setMax(int x, int y)
{
	aspectSpinBox_x->setMaximum(x);
	marginSpinBox_x->setMaximum(x);
	aspectSpinBox_y->setMaximum(y);
	marginSpinBox_y->setMaximum(y);
}

void Preference::setWaitTime(int t)
{
	waitSpinBox->setValue(t);
}

void Preference::setAspect_sacled(bool t)
{
	if(t)
		scaledButton->setChecked(true);
	else
		fixedButton->setChecked(true);
	changeSuffix();
}

void Preference::setAspect(const QPoint& t)
{
	aspectSpinBox_x->setValue(t.x());
	aspectSpinBox_y->setValue(t.y());
}
void Preference::makeConnection()
{
	connect(fixedButton,SIGNAL(clicked()),this,SLOT(changeSuffix()));
	connect(fixedButton_2,SIGNAL(clicked()),this,SLOT(changeSuffix()));
	connect(scaledButton,SIGNAL(clicked()),this,SLOT(changeSuffix()));
	connect(scaledButton_2,SIGNAL(clicked()),this,SLOT(changeSuffix()));
	connect(colorButton,SIGNAL(clicked()),this,SLOT(getLineColor()));
	connect(unitBox,SIGNAL(currentIndexChanged(int)),this,SLOT(changeSuffix()));
	connect(defaultBox,SIGNAL(currentIndexChanged(int)),this,SLOT(changeDefault(int)));
	connect(dpiBox,SIGNAL(valueChanged(int)),this,SLOT(calSize()));
	connect(aspectSpinBox_x,SIGNAL(valueChanged(int)),this,SLOT(changeStatus()));
	connect(aspectSpinBox_y,SIGNAL(valueChanged(int)),this,SLOT(changeStatus()));
	connect(marginSpinBox_x,SIGNAL(valueChanged(int)),this,SLOT(changeStatus()));
	connect(marginSpinBox_y,SIGNAL(valueChanged(int)),this,SLOT(changeStatus()));
	connect(aspectBox,SIGNAL(toggled(bool)),this,SLOT(changeStatus()));
	connect(marginBox,SIGNAL(toggled(bool)),this,SLOT(changeStatus()));
	connect(scaledButton,SIGNAL(clicked()),this,SLOT(changeStatus()));
}

void Preference::setData()
{
	unitBox->setItemData(0,0.0254);
	unitBox->setItemData(1,.01);
	unitBox->setItemData(2,.001);
	unitBox->setItemData(3,1.0/3.0);
	unitBox->setItemData(4,0.0);
}

void Preference::changeStatus()
{
	if(isCustom == true)
		defaultBox->setCurrentIndex(0);
	calSize();
}

void Preference::getLineColor()
{
	colorLineEdit->setText(
			QColorDialog::getColor(QColor(colorLineEdit->text()),this, tr("Select your color")).name());
}


void Preference::setpersent(QSpinBox *t)
{
	t->setSuffix(" %");
}

void Preference::changeSuffix()
{
	QString unit(QString(" %1").arg(unitBox->currentText()));
	if(scaledButton->isChecked())
	{
		aspectSpinBox_x->setSuffix(tr(" to"));
		aspectSpinBox_y->setSuffix("");
	}
	else
	{
		aspectSpinBox_x->setSuffix(unit);
		aspectSpinBox_y->setSuffix(unit);
	}
	if(scaledButton_2->isChecked())
	{
		marginSpinBox_x->setSuffix(" %");
		marginSpinBox_y->setSuffix(" %");
	}
	else
	{
		marginSpinBox_x->setSuffix(unit);
		marginSpinBox_y->setSuffix(unit);
	}
	calSize();
}

void Preference::changeDefault(int t)
{
	if(t<=0) return;
	isCustom = false;
	setAspect_sacled(false);
	marginBox->setChecked(false);
    //setDpi(300);
	switch (t) {
	case 1:
		aspectSpinBox_x->setValue(25);
		aspectSpinBox_y->setValue(35);
		unitBox->setCurrentIndex(2);
		break;
	case 2:
		aspectSpinBox_x->setValue(26);
		aspectSpinBox_y->setValue(36);
		unitBox->setCurrentIndex(2);
		break;
	case 3:
		aspectSpinBox_x->setValue(26);
		aspectSpinBox_y->setValue(36);
		unitBox->setCurrentIndex(2);
		break;
	case 4:
		aspectSpinBox_x->setValue(35);
		aspectSpinBox_y->setValue(53);
		unitBox->setCurrentIndex(2);
		break;
	case 5:
		aspectSpinBox_x->setValue(26);
		aspectSpinBox_y->setValue(32);
        //setDpi(350);
		unitBox->setCurrentIndex(2);
		break;
	case 6:
		aspectSpinBox_x->setValue(168);
		aspectSpinBox_y->setValue(240);
		unitBox->setCurrentIndex(4);
		break;
	case 7:
		aspectSpinBox_x->setValue(26);
		aspectSpinBox_y->setValue(32);
		unitBox->setCurrentIndex(2);
		break;
	default:
		break;
	}
	isCustom = true;
}

void Preference::calSize()
{
	setData();
	bool isPx = unitBox->currentData().toDouble()==0.0;
	double scale =  unitBox->currentData().toDouble() / unitBox->itemData(0).toDouble();
	if(aspect_scaled()||isPx)
	{
        ssize.setWidth(aspectSpinBox_x->value());
        ssize.setHeight(aspectSpinBox_y->value());
	}
	else
	{
        ssize.setHeight(aspectSpinBox_y->value() * scale * dpiBox->value());
        ssize.setWidth(aspectSpinBox_x->value() * scale * dpiBox->value());
	}
	if(margin_scaled()||isPx)
    {
        smargin.setWidth(marginSpinBox_x->value());
        smargin.setHeight(marginSpinBox_y->value());
	}
	else
    {
        smargin.setWidth(marginSpinBox_x->value() * scale * dpiBox->value());
        smargin.setHeight(marginSpinBox_y->value() * scale * dpiBox->value());
	}
}

void Preference::setMargin(bool t)
{
	marginBox->setChecked(t);
}

void Preference::setMargin(const QPoint& t)
{
	marginSpinBox_x->setValue(t.x());
	marginSpinBox_y->setValue(t.y());
}

void Preference::setMargin_scaled(bool t)
{
	if(t)
		scaledButton_2->setChecked(true);
	else
		fixedButton_2->setChecked(true);
	changeSuffix();
}

void Preference::setAddZero(bool t)
{
	addZeroBox->setChecked(t);
}

void Preference::setAddZero_digit(int t)
{
	digitSpinBox->setValue(t);
}

void Preference::reject()
{
	loadSettings(QSettings("AutoFaceCapture","AutoFaceCapture"));
	QDialog::reject();
}

void Preference::accept()
{
	QSettings s("AutoFaceCapture","AutoFaceCapture");
	toSettings(s);
	QDialog::accept();
}

void Preference::setTolernace(int t)
{
	toleranceSpinBox->setValue(t);
}

void Preference::loadSettings(const QSettings & s)
{
	setColor(s.value("color",color().name()).toString());
	setThickness(s.value("thickness",thickness()).toInt());
	setOffset(s.value("offset",offset()).toPoint());
	setAspect(s.value("aspect",aspect()).toBool());
	setAspect_sacled(s.value("aspect_scaled",aspect_scaled()).toBool());
	setAspect(s.value("aspect_val",aspect_val()).toPoint());
	setMargin(s.value("margin",margin()).toBool());
	setMargin(s.value("margin_val",margin_scaled()).toPoint());
	setMargin_scaled(s.value("margin_scaled",margin_val()).toBool());
	setTolernace(s.value("tolernace",tolernace()).toInt());
	setAddZero(s.value("addZero",addZero()).toBool());
	setAddZero_digit(s.value("addZero_digit",addZero_digit()).toInt());
	setWaitTime(s.value("waitTime",waitTime()).toInt());
	setDefaultIndex(s.value("default",defaultIndex()).toInt());
	setUnit(s.value("unit",unit()).toInt());
	setDpi(s.value("dpi",dpi()).toInt());
    resize(s.value("setting",size()).toSize());
}
void Preference::toSettings(QSettings & s)
{
	s.setValue("color",color().name());
	s.setValue("thickness",thickness());
	s.setValue("offset",offset());
	s.setValue("aspect",aspect());
	s.setValue("aspect_scaled",aspect_scaled());
	s.setValue("aspect_val",aspect_val());
	s.setValue("margin",margin());
	s.setValue("margin_scaled",margin_scaled());
	s.setValue("margin_val",margin_val());
	s.setValue("tolernace",tolernace());
	s.setValue("addZero",addZero());
	s.setValue("addZero_digit",addZero_digit());
	s.setValue("waitTime",waitTime());
	s.setValue("unit",unit());
	s.setValue("dpi",dpi());
	s.setValue("default",defaultIndex());
    s.setValue("setting",size());
}
