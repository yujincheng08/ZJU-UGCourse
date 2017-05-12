#ifndef RAMTABLEDELEGATE_H
#define RAMTABLEDELEGATE_H

#include <QStyledItemDelegate>

class RamTableDelegate : public QStyledItemDelegate
{
public:
	RamTableDelegate(QObject *parent = nullptr);
	QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const override;
	void setEditorData(QWidget *editor, const QModelIndex &index) const override;
	void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const override;
};

#endif // RAMTABLEDELEGATE_H
