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

	void addLabel(QString &labelName, QString &attrdata, SETTYPE type);
	void addNodeLabel(QString &labelName, QString &attrdata);
	void addElemLabel(QString &labelName, QString &attrdata);

	void onItemPressed(QTreeWidgetItem * item, int column);
signals:
	void showNodeLabel(QTreeWidgetItem* item);
	void hideNodeLabel(QTreeWidgetItem* item);
	void showElemLabel(QTreeWidgetItem* item);
	void hideElemLabel(QTreeWidgetItem* item);
	void resetActor();
private:
	QList<QTreeWidgetItem*> topItems;
	QTreeWidgetItem *labelItem;
	QTreeWidgetItem *OperationItem;
};

#endif