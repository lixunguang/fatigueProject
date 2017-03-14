#ifndef OBJECTVIEWER_H_
#define OBJECTVIEWER_H_

#include <QTreeWidget>
#include <QDockWidget>
#include <QPushButton>

#include "treeitem.h"
#include "typedef.h"
#include "projectManager.h"

class  ObjectViewer : public QDockWidget
{
	Q_OBJECT
public:
	ObjectViewer(QWidget *parent = 0);
	~ObjectViewer();

public:
	void updateUi(ProjectManager &projectManager);
	void reset();
public:
	void addLabelToData(QString &labelName, QSet<int>& attrdata, SetType type);
	void addNodeLabelToData(QString &labelName, QSet<int>& attrdata);
	void addElemLabelToData(QString &labelName, QSet<int>& attrdata);

	void updateUiLabel(QMap<QString, QString> &modelData);
 	void updateUiOperator(QMap<QString, QString>& solverData);

	void addObjectToUI(QString &labelName, QMap<QString, QString>& attrData, SetType type);

public slots:
	void onAddNodeLabelBtn();
	void onAddElemLabelBtn();
	void onRemoveBtn();
	void onSelectPointBtn();
	void onSelectCellBtn();

	void add(SetType labelType);
	void onLabelItemPressed(QTreeWidgetItem * item, int column);
signals:
	void showNodeLabel(QTreeWidgetItem* item);
	void hideNodeLabel(QTreeWidgetItem* item);
	void showElemLabel(QTreeWidgetItem* item);
	void hideElemLabel(QTreeWidgetItem* item);

	void selectStatus(SelectType);

	void resetActor();
private:
	QList<QTreeWidgetItem*> topItems;
	QTreeWidgetItem *labelItem;
	QTreeWidgetItem *OperationItem;

	QPushButton *selectPointBtn;
	QPushButton *selectCellBtn;
};

#endif