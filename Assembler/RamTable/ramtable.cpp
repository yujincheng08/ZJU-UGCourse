#include "ramtable.h"
#include "ramtablemodel.h"
#include "ramtabledelegate.h"
#include <QHeaderView>
#include <QItemSelection>
#include <QListIterator>
#include <QtDebug>
RamTable::RamTable(QWidget *parent)
	:QTableView(parent)
{
	setAlternatingRowColors(true);
	createObjects();
	createConnections();
    resizeColumnsToContents();
    resize(sizeHint());
}

QByteArray &RamTable::rawData() const
{
    return model->rawData();
}

void RamTable::setBase(int base)
{
    model->setBase(base);
    refresh();
    resizeColumnsToContents();

}

void RamTable::refresh()
{
    model->reset();
    resizeColumnsToContents();
}

void RamTable::createObjects()
{
	model = new RamTableModel(this);
	setModel(model);
	tableDelegate = new RamTableDelegate(this);
	setItemDelegate(tableDelegate);
	verticalHeader()->setVisible(false);
}

void RamTable::createConnections()
{
	connect(this,SIGNAL(activated(const QModelIndex &)),this,SLOT(changeSelection(const QModelIndex &)));
	connect(this,SIGNAL(clicked(const QModelIndex &)),this,SLOT(changeSelection(const QModelIndex &)));
}

void RamTable::changeSelection(const QModelIndex &index)
{
	if(!index.isValid())
		return;
	if(!index.column())
		selectRow(index.row());
}
