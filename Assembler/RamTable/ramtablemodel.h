#ifndef RAMRamTableModel_H
#define RAMRamTableModel_H

#include <QAbstractTableModel>

class RamTableModel : public QAbstractTableModel
{
	Q_OBJECT
private:
	QByteArray Data;
    int Base;
public:
	RamTableModel(QObject *parent = nullptr);
	int rowCount(const QModelIndex &parent) const override;
	int columnCount(const QModelIndex &parent) const override;
	QVariant data(const QModelIndex &index, int role) const override;
	Qt::ItemFlags flags(const QModelIndex &index) const override;
	QVariant headerData(int section, Qt::Orientation orientation, int role) const override;
	bool setData(const QModelIndex &index, const QVariant &value, int role) override;
	QByteArray &rawData();
	QModelIndex expendSelection(const QModelIndex& index);
    void setBase(int base = 16);
private:
	const QString getData(const QModelIndex &index) const;
	int offset(const QModelIndex &index) const;
public slots:
	void reset();
};


#endif // RAMRamTableModel_H
