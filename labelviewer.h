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
	void updateUi(QMap<QString, QString> &mapData);
	void reset();
public slots:
	void onAddBtn();
	void onAddBtn2();
	void onRemoveBtn();
	void onSelectPointBtn();
	void onSelectCellBtn();

	void add(SETTYPE labelType);

	void addLabelToUI(QString &labelName, QSet<int>& attrdata, SETTYPE type);
	void addNodeLabelToUI(QString &labelName, QSet<int>& attrdata);
	void addElemLabelToUI(QString &labelName, QSet<int>& attrdata);

	void addLabelToData(QString &labelName, QSet<int>& attrdata, SETTYPE type);
	void addNodeLabelToData(QString &labelName, QSet<int>& attrdata);
	void addElemLabelToData(QString &labelName, QSet<int>& attrdata);

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