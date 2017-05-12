#include "ramtabledelegate.h"
#include <QLineEdit>

RamTableDelegate::RamTableDelegate(QObject *parent)
	:QStyledItemDelegate(parent)
{

}

QWidget *RamTableDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &, const QModelIndex &) const
{
	QLineEdit *lineEdit = new QLineEdit(parent);
	lineEdit->setInputMask(">HH");
	lineEdit->setAlignment(Qt::AlignCenter);
	return lineEdit;
}

void RamTableDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
	QString value = index.model()->data(index,Qt::DisplayRole).toString();
	QLineEdit *lineEdit = qobject_cast<QLineEdit *>(editor);
	lineEdit->setText(value);
}

void RamTableDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
	QLineEdit *lineEdit = qobject_cast<QLineEdit *>(editor);
	QString value = lineEdit->text();
	model->setData(index,QVariant((unsigned char)value.toInt(nullptr,16)));
}

