#ifndef RAMTABLE_H
#define RAMTABLE_H

#include <QTableView>

class RamTableModel;
class RamTableDelegate;
class RamTable : public QTableView
{
	Q_OBJECT
private:
	RamTableModel *model;
	RamTableDelegate *tableDelegate;
public:
	RamTable(QWidget *parent = nullptr);
	QByteArray &rawData() const;
    void setBase(int base = 16);
public slots:
	void refresh();
private:
	void createObjects();
	void createConnections();
private slots:
	void changeSelection(const QModelIndex & index);
};

#endif // RAMTABLE_H
