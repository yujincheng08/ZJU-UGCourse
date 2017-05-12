#ifndef LINENUMWIDGET_H
#define LINENUMWIDGET_H

#include <QWidget>

class Editor;

class LineNumWidget : public QWidget
{
	Q_OBJECT
private:
	Editor *editor;
public:
	explicit LineNumWidget(Editor *parent);
	QSize sizeHint() const override;
protected:
	void paintEvent(QPaintEvent *event) override;
signals:

public slots:
};

#endif // LINENUMWIDGET_H
