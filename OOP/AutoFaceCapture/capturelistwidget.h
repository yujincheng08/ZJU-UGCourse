#ifndef CAPTURELISTWIDGET_H
#define CAPTURELISTWIDGET_H

#include <QWidget>
#include <QListWidget>
class QPushButton;
class QAction;
class QMenu;
class QListWidget;
class QHBoxLayout;
class QVBoxLayout;
class QListWidgetItem;
class QIcon;
class QSlider;
class MyListWidget;
class captureListWidget : public QWidget
{
	Q_OBJECT
private:
	QPushButton* deleteButton,*clearButton,*removeButton,*openButton;
	QIcon* deleteIcon,*clearIcon,*removeIcon,*openIcon,*selectAllIcon;
	QAction* deleteAction,*clearAction,*removeActon,*openAction,*selectAllAction;
	QMenu* selectedMenu,*noselectedMenu;
	QSlider* slider;
	MyListWidget* m_listWidget;
	QHBoxLayout* m_HLayout;
	QVBoxLayout* m_VLayout;
public:
	explicit captureListWidget(QWidget* = 0);
	void translate();
	void addItem(QListWidgetItem *);
	int itemSize();
private:
	void createObjects();
	void createMenus();
	void connectActions();
	void layoutWidgets();
	void setState();
	int selectedNum();
	bool forSelectedItems(bool);
signals:

public slots:
	void setItemSize(int);
private slots:
	void deleteItem();
	void removeItem();
	void setButtonState();
	void openItem(QListWidgetItem*);
	void openItem();
	void reSize();
protected:
	void contextMenuEvent(QContextMenuEvent*) override;
};

class MyListWidget : public QListWidget
{
	Q_OBJECT
public:
	explicit MyListWidget(QWidget* = 0);
signals:
	void reSized();
protected:
	void resizeEvent(QResizeEvent *) override;
};

#endif // CAPTURELISTWIDGET_H
