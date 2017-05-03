#ifndef PREFERENCE_H
#define PREFERENCE_H

#include "ui_preference.h"
class QSettings;

class Preference : public QDialog, private Ui::Preference
{
	Q_OBJECT
private:
    QSize ssize,smargin;
    bool isCustom;
public:
	Preference(QWidget* = 0);
	//int exec() Q_DECL_OVERRIDE;
	void loadSettings(const QSettings&);
	void toSettings(QSettings&);
private:
	void makeConnection();
	void setpersent(QSpinBox*);
private slots:
	void getLineColor();
	void changeSuffix();
	void calSize();
	void changeDefault(int);
	void setData();
	void changeStatus();
public slots:
    void reject() Q_DECL_OVERRIDE;
    void accept() Q_DECL_OVERRIDE;
public:
    QColor  color() const;
	int  thickness() const;
    QPoint  offset() const;
	bool  aspect() const;
	bool  aspect_scaled() const;
    QSize aspect_val()const;
	bool  margin() const;
	bool  margin_scaled() const;
    QSize margin_val()const;
	int  tolernace()const;
	bool addZero();
	int addZero_digit();
	int waitTime();
	int dpi() const;
	int DotsPerMeter()const;
	int unit()const;
	int defaultIndex()const;
	void setDpi(int);
	void setUnit(int);
	void setDefaultIndex(int);
	void setColor(const QColor&);
	void setThickness(int);
	void setOffset(const QPoint&);
	void setAspect(bool);
	void setAspect_sacled(bool);
	void setAspect(const QPoint&);
	void setMargin(bool);
	void setMargin(const QPoint&);
    void setMargin_scaled(bool);
	void setAddZero(bool);
	void setAddZero_digit(int);
	void setTolernace(int);
	void setMax(int,int);
	void setWaitTime(int);
	void translate();
};



#endif // PREFERENCE_H
