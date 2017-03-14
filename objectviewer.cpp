
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QPushButton>
#include <QInputDialog>
#include <QMessageBox>
#include <QIcon>
#include <QDebug>

#include "mesh.h"
#include "objectviewer.h"
#include "mainwindow.h"
#include "meshviewer.h"

ObjectViewer::ObjectViewer(QWidget *parent)
	:QDockWidget(parent)
{
	QVBoxLayout *vLayout = new QVBoxLayout();
	QWidget *labelBrowser = new QWidget();
	labelBrowser->setLayout(vLayout);

	QTreeWidget *tree = new QTreeWidget();
	vLayout->addWidget(tree);

	QPushButton *addNodeLabelBtn = new QPushButton();
	addNodeLabelBtn->setToolTip("add a node label ");
	addNodeLabelBtn->setIcon(QIcon(QPixmap(":/res/label/label_add.png")));
	connect(addNodeLabelBtn, SIGNAL(clicked()), this, SLOT(onAddNodeLabelBtn()));

	QPushButton *addElemLabelBtn = new QPushButton();
	addElemLabelBtn->setToolTip("add an elem label ");
	addElemLabelBtn->setIcon(QIcon(QPixmap(":/res/label/label_add.png")));
	connect(addElemLabelBtn, SIGNAL(clicked()), this, SLOT(onAddElemLabelBtn()));

	QPushButton *removeBtn = new QPushButton();
	addElemLabelBtn->setToolTip("remove selected label ");
	removeBtn->setIcon(QIcon(QPixmap(":/res/label/label_remove.png")));
	connect(removeBtn, SIGNAL(clicked()), this, SLOT(onRemoveBtn()));

	selectPointBtn = new QPushButton(); selectPointBtn->setCheckable(true);
	selectPointBtn->setToolTip("change status to select node");
	selectPointBtn->setIcon(QIcon(QPixmap(":/res/label/label_select_point.png")));
	connect(selectPointBtn, SIGNAL(clicked()), this, SLOT(onSelectPointBtn()));

	selectCellBtn = new QPushButton(); selectCellBtn->setCheckable(true);
	selectCellBtn->setToolTip("change status to select elem ");
	selectCellBtn->setIcon(QIcon(QPixmap(":/res/label/label_select_cell.png")));
	connect(selectCellBtn, SIGNAL(clicked()), this, SLOT(onSelectCellBtn()));

	QSpacerItem *space = new QSpacerItem(0, 0, QSizePolicy::MinimumExpanding, QSizePolicy::Minimum);

	QHBoxLayout *hLayout = new QHBoxLayout();
	hLayout->addWidget(addNodeLabelBtn);
	hLayout->addWidget(addElemLabelBtn);
	hLayout->addWidget(removeBtn);
	hLayout->addWidget(selectPointBtn);
	hLayout->addWidget(selectCellBtn);
	hLayout->addSpacerItem(space);

	vLayout->addLayout(hLayout);

	this->setWidget(labelBrowser);

	this->setFeatures(QDockWidget::NoDockWidgetFeatures);

	tree->setColumnCount(3);
	tree->setColumnWidth(0, 60);
	tree->setColumnWidth(2, 30);
	tree->setRootIsDecorated(true);

	connect(tree, SIGNAL(itemPressed(QTreeWidgetItem *, int)), this, SLOT(onLabelItemPressed(QTreeWidgetItem *, int)));

	labelItem = new TreeItem();
	labelItem->setIcon(0, QIcon(QPixmap(":/res/label/label_visibility_off.png")));
	labelItem->setText(1, "Labels");
	topItems.append(labelItem);


	OperationItem = new TreeItem();
	OperationItem->setIcon(0, QIcon(QPixmap(":/res/label/label_visibility_off.png")));
	OperationItem->setText(1, "Operation");
	topItems.append(OperationItem);

	tree->addTopLevelItems(topItems);
}

ObjectViewer::~ObjectViewer()
{
	reset();
}

void ObjectViewer::reset()
{
	QTreeWidgetItem *item;
	for each (item in topItems)
	{
		int cnt = item->childCount();
		while (cnt--)
		{
			TreeItem *childItem = (TreeItem *) item->child(cnt);
			delete childItem;
		}
	}

}

void ObjectViewer::updateUi(ProjectManager &projectManager)
{
	QMap<QString, QString> modelData = projectManager.getModelData();
	updateUiLabel(modelData);

	// update solve config
	QMap<QString, QString> solverData = projectManager.getSolverData();
	qDebug() << "solverData: " << solverData;
	updateUiOperator(solverData);
}

void ObjectViewer::updateUiLabel(QMap<QString, QString> &modelData)
{
	QMap<QString, QString> NodeLabel;
	QMap<QString, QString> ElementLabel;

	for each (QString key in modelData.keys())
	{
		QStringList keyList = key.split("@");
		QStringList valList = modelData[key].split("@");

		if (keyList[0] == "NodeLabel")
		{
			NodeLabel[valList[0]] = valList[1];
		}
		else if (keyList[0] == "ElementLabel")
		{
			ElementLabel[valList[0]] = valList[1];
		}
	}

	// update label
	for each (QString labelName in NodeLabel.keys())
	{
		addObjectToUI(labelName, NodeLabel, SetType_NODE);
	}

	for each (QString labelName in ElementLabel.keys())
	{
		addObjectToUI(labelName, ElementLabel, SetType_ELEM);
	}

}

void ObjectViewer::updateUiOperator(QMap<QString, QString>& solverData)
{
	addObjectToUI(QString("operxxx"),solverData,SetType_OPER);
}

void ObjectViewer::addObjectToUI(QString &labelName, QMap<QString, QString>& attrData, SetType type)
{
	//界面上增加
	QIcon icon;
	icon.addPixmap(QPixmap(":/res/label/label_visibility_off.png"));

	TreeItem *item = new TreeItem();
	item->setShow(false);
	item->setIcon(0, icon);
	item->setText(1, labelName);
	item->setAttrData(attrData);
	item->setType(type);

	QTreeWidgetItem *parent = labelItem;
	if (type == SetType_OPER)
	{
		parent = OperationItem;
	}

	parent->addChild(item);
}

void ObjectViewer::onLabelItemPressed(QTreeWidgetItem *item, int column)
{//点击标签项
	if (column != 0)
	{
		return;
	}

	TreeItem *_item = (TreeItem*) item;
	QIcon icon = _item->icon(0);
	bool show = _item->show();

	if (show)
	{
		emit resetActor();

		_item->setShow(false);
		icon.addPixmap(QPixmap(":/res/label/label_visibility_off.png"));

		//hide children if item is top item
		if (_item == labelItem)
		{
			int cnt = _item->childCount();
			while (cnt--)
			{
				TreeItem *childItem = (TreeItem *) _item->child(cnt);
				childItem->setShow(false);
				if (childItem->type == SetType_NODE)
				{
					emit hideNodeLabel(childItem);
				}
				else if (childItem->type == SetType_ELEM)
				{
					emit hideElemLabel(childItem);
				}

				icon.addPixmap(QPixmap(":/res/label/label_visibility_off.png"));
				childItem->setIcon(0, icon);
			}
		}
		else
		{

			if (_item->type == SetType_NODE)
			{
				emit hideNodeLabel(_item);
			}
			else if (_item->type == SetType_ELEM)
			{
				emit hideElemLabel(_item);
			}
		}
	}
	else
	{
		icon.addPixmap(QPixmap(":/res/label/label_visibility_on.png"));
		_item->setShow(true);

		//hide children if item is top item
		if (_item == labelItem)
		{
			int cnt = _item->childCount();
			while (cnt--)
			{
				TreeItem *childItem = (TreeItem *) _item->child(cnt);
				childItem->setShow(true);
				if (childItem->type == SetType_NODE)
				{
					emit showNodeLabel(childItem);
				}
				else if (childItem->type == SetType_ELEM)
				{
					emit showElemLabel(childItem);
				}

				icon.addPixmap(QPixmap(":/res/label/label_visibility_on.png"));
				childItem->setIcon(0, icon);
			}
		}
		else
		{
			if (_item->type == SetType_NODE)
			{
				emit showNodeLabel(_item);
			}
			else if (_item->type == SetType_ELEM)
			{
				emit showElemLabel(_item);
			}
		}
	}

	item->setIcon(0, icon);
}






void ObjectViewer::addNodeLabelToData(QString &labelName, QSet<int>& attrdata)
{

}

void ObjectViewer::addElemLabelToData(QString &labelName, QSet<int>& attrdata)
{

}

void ObjectViewer::addLabelToData(QString &labelName, QSet<int>& attrdata, SetType type)
{
	//ModelConfigmap 这个内部数据结构里面增加
	QString varStr;
	for each (int var in attrdata)
	{
		varStr += QString("%1 ").arg(var);
	}

	//todo:实际上，一个对象如果非要和另外一个对象打交道，类似这样的问题应该这样来解决
	//1）在创建这个对象时，记录另外一个对象的指针，或者2）发送消息，这样可以降低耦合性
	//如果要写可重用的对象，那么一定是这样的
	//项目中，这样的写法是不是坏味道呢？

	MainWindow *mw = MainWindow::getMainWindow();
	if (type == SetType_ELEM)
	{
		mw->getProjectManager()->addElemLabelToModelConfig(labelName, varStr);
	}
	else if (type == SetType_NODE)
	{
		mw->getProjectManager()->addNodeLabelToModelConfig(labelName, varStr);
	}
}

void ObjectViewer::add(SetType labelType)
{
	bool ok;
	QString text = QInputDialog::getText(this, tr("input Label name"),
		tr("Label name:"), QLineEdit::Normal,
		0, &ok);

	if (ok && !text.isEmpty())
	{
		MainWindow *mw = MainWindow::getMainWindow();

		if (labelType == SetType_NODE)
		{
			QMap<QString, QString> attrdata;
			QSet<int> nodes = mw->getMeshViewer()->getSelectNodes();
			if (nodes.empty())
			{
				QMessageBox::warning(mw, "empty", "please select nodes first!");
				return;
			}
			QString nodeStr;
			for each (int var in nodes)
			{
				nodeStr += QString("%1 ").arg(var);

			}
		 
			attrdata[text] = nodeStr;
			addObjectToUI(text, attrdata,SetType_NODE);
			addLabelToData(text, nodes, SetType_NODE);
		}
		else if (labelType == SetType_ELEM)
		{
			QMap<QString, QString> attrdata;
			QSet<int> elems = mw->getMeshViewer()->getSelectElems();
			if (elems.empty())
			{
				QMessageBox::warning(mw, "empty", "please select elements first!");
				return;
			}
			QString elemStr;
			for each (int var in elems)
			{
				elemStr += QString("%1 ").arg(var);
			}
			attrdata[text] = elemStr;
			addObjectToUI(text, attrdata, SetType_ELEM);
			addLabelToData(text, elems, SetType_ELEM);
		}
	}
}

void ObjectViewer::onAddNodeLabelBtn()
{
	add(SetType_NODE);
}

void ObjectViewer::onAddElemLabelBtn()
{
	add(SetType_ELEM);
}

void ObjectViewer::onRemoveBtn()
{

}

void ObjectViewer::onSelectPointBtn()
{
	if (selectPointBtn->isChecked())
	{
		selectPointBtn->setChecked(true);
		emit selectStatus(SelectType_Point);
	}
	else
	{
		selectPointBtn->setChecked(false);
		emit selectStatus(SelectType_None);
	}

}

void ObjectViewer::onSelectCellBtn()
{
	if (selectCellBtn->isChecked())
	{
		selectCellBtn->setChecked(true);
		emit selectStatus(SelectType_Cell);
	}
	else
	{
		selectCellBtn->setChecked(false);
		emit selectStatus(SelectType_None);
	}

}