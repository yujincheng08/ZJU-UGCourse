#include "capturelistwidget.h"
#include <QPushButton>
#include <QAction>
#include <QMenu>
#include <QListWidget>
#include <QListWidgetItem>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QCursor>
#include <QtDebug>
#include <QUrl>
#include <QFile>
#include <QMessageBox>
#include <QDesktopServices>
#include <QSlider>
#include <QKeySequence>
#include <QShortcut>
captureListWidget::captureListWidget(QWidget *parent) :
	QWidget(parent)
{
	createObjects();
	layoutWidgets();
	connectActions();
	createMenus();
	translate();
	setState();
}

void captureListWidget::createObjects()
{
	//delete
	deleteButton = new QPushButton(this);
	deleteAction = new QAction(this);
	deleteIcon = new QIcon(":/icons/delete.png");
	//clear
	clearButton = new QPushButton(this);
	clearAction = new QAction(this);
	clearIcon = new QIcon(":/icons/clear.png");
	//remove
	removeButton = new QPushButton(this);
	removeActon = new QAction(this);
	removeIcon = new QIcon(":/icons/remove.png");
	//open
	openButton = new QPushButton(this);
	openAction = new QAction(this);
	openIcon = new QIcon(":/icons/open.png");

	//selectAll
	selectAllAction = new QAction(this);
	selectAllIcon = new QIcon(":/icons/selectAll.png");

	//menu
	selectedMenu = new QMenu(this);
	noselectedMenu = new QMenu(this);

	//centerWidget
	m_listWidget = new MyListWidget();

	//slider
	slider = new QSlider(Qt::Horizontal);

	//layout
	m_HLayout = new QHBoxLayout();
	m_VLayout = new QVBoxLayout();
}


void captureListWidget::layoutWidgets()
{
	m_HLayout->addWidget(removeButton);
	m_HLayout->addWidget(deleteButton);
	m_HLayout->addWidget(openButton);
	m_HLayout->addWidget(clearButton);

	m_VLayout->addWidget(m_listWidget);
	m_VLayout->addLayout(m_HLayout);
	m_VLayout->addWidget(slider);
	m_VLayout->setContentsMargins(0,0,0,0);
	setLayout(m_VLayout);
}

void captureListWidget::connectActions()
{
	connect(deleteAction,SIGNAL(triggered()),this,SLOT(deleteItem()));
	connect(clearAction,SIGNAL(triggered()),m_listWidget,SLOT(clear()));
	connect(openAction,SIGNAL(triggered()),this,SLOT(openItem()));
	connect(removeActon,SIGNAL(triggered()),this,SLOT(removeItem()));
	connect(deleteButton,SIGNAL(clicked()),deleteAction,SLOT(trigger()));
	connect(clearButton,SIGNAL(clicked()),clearAction,SLOT(trigger()));
	connect(removeButton,SIGNAL(clicked()),removeActon,SLOT(trigger()));
	connect(openButton,SIGNAL(clicked()),this,SLOT(openItem()));
	connect(m_listWidget,SIGNAL(itemSelectionChanged()),this,SLOT(setButtonState()));
	connect(m_listWidget,SIGNAL(itemDoubleClicked(QListWidgetItem*)),this,SLOT(openItem(QListWidgetItem*)));
	connect(slider,SIGNAL(sliderMoved(int)),this,SLOT(reSize()));
	connect(m_listWidget,SIGNAL(reSized()),this,SLOT(reSize()));
	connect(selectAllAction,SIGNAL(triggered()),m_listWidget,SLOT(selectAll()));

	//shortCut
	connect(new QShortcut(QKeySequence(Qt::Key_Delete), this),SIGNAL(activated()),this,SLOT(removeItem()));
	connect(new QShortcut(QKeySequence("Shift+Delete"), this),SIGNAL(activated()),this,SLOT(deleteItem()));
	connect(new QShortcut(QKeySequence(Qt::Key_Enter), this),SIGNAL(activated()),this,SLOT(openItem()));
	connect(new QShortcut(QKeySequence("Ctrl+A"), this),SIGNAL(activated()),m_listWidget,SLOT(selectAll()));
}

void captureListWidget::createMenus()
{
	selectedMenu->addAction(openAction);
	selectedMenu->addAction(clearAction);
	selectedMenu->addAction(deleteAction);
	selectedMenu->addAction(removeActon);
	selectedMenu->addAction(selectAllAction);

	noselectedMenu->addAction(selectAllAction);
	noselectedMenu->addAction(clearAction);
}

void captureListWidget::translate()
{
	QString tDelete(tr("delete"));
	QString sDelete(tr("Delete selected items from disk."));
	QString tOpen(tr("open"));
	QString sOpen(tr("Open the only one selected item."));
	QString tClear(tr("clear"));
	QString sClear(tr("Clear all items without deleting from disk."));
	QString tRemove(tr("remove"));
	QString sRemove(tr("Remove selected items from table but not from disk."));
	QString sSlider(tr("Set the size of an item."));
	QString tSelectAll(tr("Select All"));
	QString sSelectAll(tr("Select all items"));
	deleteButton->setText(tDelete);
	deleteButton->setStatusTip(sDelete);
	deleteButton->setToolTip(sDelete);
	deleteAction->setText(tDelete);
	deleteAction->setStatusTip(sDelete);
	deleteAction->setToolTip(sDelete);
	openButton->setText(tOpen);
	openButton->setStatusTip(sOpen);
	openButton->setToolTip(sOpen);
	removeButton->setText(tRemove);
	removeButton->setStatusTip(sRemove);
	removeButton->setToolTip(sRemove);
	clearButton->setText(tClear);
	clearButton->setStatusTip(sClear);
	clearButton->setToolTip(sClear);
	openAction->setText(tOpen);
	openAction->setStatusTip(sOpen);
	openAction->setToolTip(tOpen);
	removeActon->setText(tRemove);
	removeActon->setStatusTip(sRemove);
	removeActon->setToolTip(sRemove);
	clearAction->setText(tClear);
	clearAction->setStatusTip(sClear);
	clearAction->setToolTip(sClear);
	slider->setStatusTip(sSlider);
	slider->setToolTip(sSlider);
	selectAllAction->setText(tSelectAll);
	selectAllAction->setStatusTip(sSelectAll);
	selectAllAction->setToolTip(sSelectAll);
}

void captureListWidget::setState()
{
	setButtonState();
	deleteButton->setIcon(*deleteIcon);
	deleteAction->setIcon(*deleteIcon);
	clearButton->setIcon(*clearIcon);
	clearAction->setIcon(*clearIcon);
	removeButton->setIcon(*removeIcon);
	removeActon->setIcon(*removeIcon);
	openButton->setIcon(*openIcon);
	openAction->setIcon(*openIcon);
	selectAllAction->setIcon(*selectAllIcon);
	m_listWidget->setViewMode(QListWidget::IconMode);
	m_listWidget->setSelectionMode(QListWidget::ExtendedSelection);
	m_listWidget->setDragDropMode(QListWidget::NoDragDrop);
	slider->setMaximum(100);
	slider->setMinimum(1);
	slider->setSingleStep(1);
	slider->setValue(30);
}
void captureListWidget::contextMenuEvent(QContextMenuEvent*)
{
	QCursor cur=this->cursor();
	if(selectedNum()==0)
		noselectedMenu->exec(cur.pos());
	else
		selectedMenu->exec(cur.pos());
}

void captureListWidget::setButtonState()
{
	if(selectedNum()==1)
	{
		openButton->setEnabled(true);
		openAction->setEnabled(true);
	}
	else
	{
		openButton->setDisabled(true);
		openAction->setDisabled(true);
	}
	if(selectedNum()==0)
	{
		deleteButton->setDisabled(true);
		removeButton->setDisabled(true);
	}
	else
	{
		deleteButton->setEnabled(true);
		removeButton->setEnabled(true);
	}
}

int captureListWidget::selectedNum()
{
	return m_listWidget->selectedItems().length();
}

void captureListWidget::deleteItem()
{
	QMessageBox ask (this);
	ask.setWindowTitle(tr("Really?"));
	ask.setText(tr("Really delete?"));
	ask.addButton(new QPushButton(QIcon(":/icons/sure.png"),
								  tr("Ok")),QMessageBox::AcceptRole);
	ask.addButton(new QPushButton(QIcon(":/icons/retry.png"),
								  tr("Cancel")),QMessageBox::RejectRole);
	if(ask.exec()==QMessageBox::Rejected)
		if(!forSelectedItems(true))
			QMessageBox::warning(this,tr("Error"),
							 tr("Cannot delete serval files of selected."));
}
void captureListWidget::removeItem()
{
	forSelectedItems(false);
}

bool captureListWidget::forSelectedItems(bool deleted)
{
	bool result = true;
	if(selectedNum()==0)	return false;
	QList<QListWidgetItem*> selectedItems =
			m_listWidget->selectedItems();
	for(auto i = selectedItems.begin();i != selectedItems.end();i++)
	{
		QListWidgetItem* item = *i;
		m_listWidget->takeItem(m_listWidget->row(item));
		if(deleted)
		{
			QString path = item->data(Qt::UserRole).toString();
			if(path.isEmpty())
			{
				result= false;
				continue;
			}
			result = QFile::remove(path) && result;
		}
	}
	return result;
}

void captureListWidget::setItemSize(int t)
{
	if(t>100 || t<1)
		return;
	slider->setValue(t);
}

void captureListWidget::reSize()
{
	int width = m_listWidget->width();
	int size = width * slider->value()/100;
	m_listWidget->setIconSize(QSize(size/1.2,size/1.2));
	m_listWidget->setGridSize(QSize(size,size));
}

void captureListWidget::addItem(QListWidgetItem *item)
{
	m_listWidget->addItem(item);
}

int captureListWidget::itemSize()
{
	return slider->value();
}

void captureListWidget::openItem(QListWidgetItem* item)
{
	QString path(item->data(Qt::UserRole).toString());
	QUrl path_url = QUrl::fromLocalFile(path);
	QDesktopServices::openUrl(path_url);
}

void captureListWidget::openItem()
{
	if(selectedNum()!=1) return;
	QListWidgetItem* item = m_listWidget->selectedItems().at(0);
	QString path(item->data(Qt::UserRole).toString());
	QUrl path_url = QUrl::fromLocalFile(path);
	QDesktopServices::openUrl(path_url);
}


MyListWidget::MyListWidget(QWidget * parent)
	:QListWidget(parent)
{
}

void MyListWidget::resizeEvent(QResizeEvent *e)
{
	emit reSized();
	return QListWidget::resizeEvent(e);
}
