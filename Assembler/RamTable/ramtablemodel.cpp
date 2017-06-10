#include "RamTableModel.h"
#include <QByteArray>
RamTableModel::RamTableModel(QObject *parent)
	:QAbstractTableModel(parent),Data(1048576,0)
{
    Base = 16;
}

int RamTableModel::rowCount(const QModelIndex &) const
{
	return 262144;
}

int RamTableModel::columnCount(const QModelIndex &) const
{
	return 5;
}

QVariant RamTableModel::data(const QModelIndex &index, int role) const
{
	if(!index.isValid())
		return QVariant();
	switch(role)
	{
	case Qt::DisplayRole:
		if(index.column()==0)
            return QString("%1").arg(4 * index.row(),5,16,QChar('0')).toUpper();
		else
			return getData(index);
	case Qt::TextAlignmentRole:
		return QVariant(Qt::AlignHCenter | Qt::AlignCenter);
	default:
		return QVariant();
	}
}

Qt::ItemFlags RamTableModel::flags(const QModelIndex &index) const
{
	Qt::ItemFlags flags = QAbstractItemModel::flags(index);
	if (index.column())
		flags |= Qt::ItemIsEditable;
	return flags;
}

QVariant RamTableModel::headerData(int section, Qt::Orientation orientation, int role) const
{
	if(role != Qt::DisplayRole)
		return QVariant();
	switch(orientation)
	{
	case Qt::Vertical:
		return QVariant();
	case Qt::Horizontal:
		switch(section)
		{
		case 0:
			return QVariant("Address");
		case 1:
			return "00";
		case 2:
			return "01";
		case 3:
			return "10";
		case 4:
			return "11";
		default:
			return QVariant();
		}
	default:
		return QVariant();
	}
}

bool RamTableModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
	if(index.isValid() && role==Qt::EditRole)
	{
		Data[offset(index)] = value.toChar().cell();
		return true;
	}
	else
		return false;
}

QByteArray &RamTableModel::rawData()
{
	return Data;
}

QModelIndex RamTableModel::expendSelection(const QModelIndex &index)
{
    return createIndex(index.row(),4);
}

void RamTableModel::setBase(int base)
{
    Base = base;
}

const QString RamTableModel::getData(const QModelIndex &index) const
{
	unsigned char cellData = Data.at(offset(index));
    return QString("%1").arg(cellData,
                             Base == 16? 2 : 8 ,Base,QChar('0')).toUpper();
}

int RamTableModel::offset(const QModelIndex &index) const
{
    return index.row() * 4 + 4 - index.column();
}

void RamTableModel::reset()
{
    beginResetModel();
	endResetModel();
}
