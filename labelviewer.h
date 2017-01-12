#ifndef labelview_H_
#define labelview_H_

#include <QTreeWidget>
#include <QDockWidget>

#include "treeitem.h"

class  LabelViewer : public QDockWidget
{
	Q_OBJECT
public:
	LabelViewer(QWidget *parent = 0);
	~LabelViewer();
public slots:
	void onAddBtn();
	void onRemoveBtn();

	void onAddLabel(QString &labelName);

	void onItemPressed(QTreeWidgetItem * item, int column);
private:
	QList<QTreeWidgetItem*> topItems;
	QTreeWidgetItem *labelItem;
	QTreeWidgetItem *OperationItem;
};

#endif