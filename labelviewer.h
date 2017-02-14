#ifndef labelview_H_
#define labelview_H_

#include <QTreeWidget>
#include <QDockWidget>
#include <QPushButton>

#include "treeitem.h"
#include "typedef.h"

class  LabelViewer : public QDockWidget
{
	Q_OBJECT
public:
	LabelViewer(QWidget *parent = 0);
	~LabelViewer();
public slots:
	void onAddBtn();
	void onAddBtn2();
	void onRemoveBtn();
	void onSelectPointBtn();
	void onSelectCellBtn();

	void add(SETTYPE labelType);

	void addLabel(QString &labelName, QSet<int>& attrdata, SETTYPE type);
	void addNodeLabel(QString &labelName, QSet<int>& attrdata);
	void addElemLabel(QString &labelName, QSet<int>& attrdata);

	void onLabelItemPressed(QTreeWidgetItem * item, int column);
signals:
	void showNodeLabel(QTreeWidgetItem* item);
	void hideNodeLabel(QTreeWidgetItem* item);
	void showElemLabel(QTreeWidgetItem* item);
	void hideElemLabel(QTreeWidgetItem* item);

	void selectStatus(Select_Type);

	void resetActor();
private:
	QList<QTreeWidgetItem*> topItems;
	QTreeWidgetItem *labelItem;
	QTreeWidgetItem *OperationItem;

	QPushButton *selectPointBtn;
	QPushButton *selectCellBtn;
};

#endif