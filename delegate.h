#ifndef Delegate_H_
#define Delegate_H_

#include <QObject>
#include <QStyledItemDelegate>

#include "table.h"
class  Delegate : public QStyledItemDelegate
{
	Q_OBJECT
public:
	Delegate(QObject *parent = 0);
	~Delegate();

public:
	virtual QWidget * createEditor(QWidget * parent, const QStyleOptionViewItem & option, const QModelIndex & index) const;
	virtual void setEditorData(QWidget * editor, const QModelIndex & index) const;
	virtual void setModelData(QWidget * editor, QAbstractItemModel * model, const QModelIndex & index) const;

private:
	Table *table;
};

#endif