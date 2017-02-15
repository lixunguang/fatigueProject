
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QPushButton>
#include <QInputDialog>
#include <QMessageBox>
#include <QIcon>
#include <QDebug>

#include "mesh.h"
#include "labelviewer.h"
#include "mainwindow.h"
#include "meshviewer.h"

LabelViewer::LabelViewer(QWidget *parent)
:QDockWidget(parent)
{
	QVBoxLayout *vLayout = new QVBoxLayout();
	QWidget *labelBrowser = new QWidget();
	labelBrowser->setLayout(vLayout);

	QTreeWidget *tree = new QTreeWidget();
	vLayout->addWidget(tree);

	QPushButton *addBtn = new QPushButton();
 
	addBtn->setIcon(QIcon(QPixmap(":/res/label/label_add.png")));
	connect(addBtn, SIGNAL(clicked()), this, SLOT(onAddBtn()));

	QPushButton *addBtn2 = new QPushButton();
	
	addBtn2->setIcon(QIcon(QPixmap(":/res/label/label_add.png")));
	connect(addBtn2, SIGNAL(clicked()), this, SLOT(onAddBtn2()));

	QPushButton *removeBtn = new QPushButton();
	removeBtn->setIcon(QIcon(QPixmap(":/res/label/label_remove.png")));
	connect(removeBtn, SIGNAL(clicked()), this, SLOT(onRemoveBtn()));

	selectPointBtn = new QPushButton(); selectPointBtn->setCheckable(true);
	selectPointBtn->setToolTip("change select status ");
	selectPointBtn->setIcon(QIcon(QPixmap(":/res/label/label_select_point.png")));
	connect(selectPointBtn, SIGNAL(clicked()), this, SLOT(onSelectPointBtn()));

	selectCellBtn = new QPushButton(); selectCellBtn->setCheckable(true);
	selectCellBtn->setToolTip("change select status ");
	selectCellBtn->setIcon(QIcon(QPixmap(":/res/label/label_select_cell.png")));
	connect(selectCellBtn, SIGNAL(clicked()), this, SLOT(onSelectCellBtn()));

	QSpacerItem *space = new QSpacerItem(0, 0, QSizePolicy::MinimumExpanding, QSizePolicy::Minimum);

	QHBoxLayout *hLayout = new QHBoxLayout();
	hLayout->addWidget(addBtn);
	hLayout->addWidget(addBtn2);//todo:
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

LabelViewer::~LabelViewer()
{
	QTreeWidgetItem *item;
	for each (item in topItems)
	{
		int cnt = item->childCount();
		while (cnt--)
		{
			TreeItem *childItem = (TreeItem *)item->child(cnt);
			delete childItem;
		}
		delete item;
	}

	topItems.clear();
}

void LabelViewer::updateUi(QMap<QString, QString> &mapData)
{
	QMap<QString, QString> NodeLabel;
	QMap<QString, QString> ElementLabel;

	for each (QString key in mapData.keys())
	{
		QStringList keyList = key.split("@");
		QStringList valList = mapData[key].split("@");

		if (keyList[0] == "NodeLabel")
		{
			NodeLabel[valList[0]] = valList[1];
		}
		else if (keyList[0] == "ElementLabel")
		{
			ElementLabel[valList[0]] = valList[1];
		}
	}

	// update labelViewer ui
	for each (QString labelName in NodeLabel.keys())
	{
		QSet<int> nodes;
		QString nodesStr = NodeLabel[labelName];
		QStringList sl = nodesStr.split(" ");
		for each (QString var in sl)
		{
			nodes.insert(var.toInt());
		}
		this->addNodeLabelToUI(labelName, nodes);
	}

	for each (QString labelName in ElementLabel.keys())
	{
		QSet<int> elems;
		QString elemStr = ElementLabel[labelName];
		QStringList sl = elemStr.split(" ");
		for each (QString var in sl)
		{
			elems.insert(var.toInt());
		}
		this->addElemLabelToUI(labelName, elems);
	}
}

void LabelViewer::onLabelItemPressed(QTreeWidgetItem *item, int column)
{//点击标签项
	if (column != 0)
	{
		return;
	}

	TreeItem *_item = (TreeItem*)item;
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
				TreeItem *childItem = (TreeItem *)_item->child(cnt);
				childItem->setShow(false);
				if (childItem->type == SETTYPE_NODE)
				{
					emit hideNodeLabel(childItem);
				}
				else if (childItem->type == SETTYPE_ELEM)
				{
					emit hideElemLabel(childItem);
				}

				icon.addPixmap(QPixmap(":/res/label/label_visibility_off.png"));
				childItem->setIcon(0, icon);
			}
		}
		else
		{

			if (_item->type == SETTYPE_NODE)
			{
				emit hideNodeLabel(_item);
			}
			else if (_item->type == SETTYPE_ELEM)
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
				TreeItem *childItem = (TreeItem *)_item->child(cnt);
				childItem->setShow(true);
				if (childItem->type == SETTYPE_NODE)
				{
					emit showNodeLabel(childItem);
				}
				else if (childItem->type == SETTYPE_ELEM)
				{
					emit showElemLabel(childItem);
				}

				icon.addPixmap(QPixmap(":/res/label/label_visibility_on.png"));
				childItem->setIcon(0, icon);
			}
		}
		else
		{
			if (_item->type == SETTYPE_NODE)
			{
				emit showNodeLabel(_item);
			}
			else if (_item->type == SETTYPE_ELEM)
			{
				emit showElemLabel(_item);
			}
		}
	}

	item->setIcon(0, icon);
}

void LabelViewer::addNodeLabelToUI(QString &labelName, QSet<int>& attrdata)
{
	addLabelToUI(labelName, attrdata, SETTYPE_NODE);
}

void LabelViewer::addElemLabelToUI(QString &labelName, QSet<int>& attrdata)
{
	addLabelToUI(labelName, attrdata, SETTYPE_ELEM);
}


void LabelViewer::addLabelToUI(QString &labelName, QSet<int>& attrdata, SETTYPE type)
{
	//界面上增加
	QIcon icon;
	icon.addPixmap(QPixmap(":/res/label/label_visibility_off.png"));

	TreeItem *item = new TreeItem();
	item->setShow(false);
	item->setIcon(0, icon);
	item->setText(1, labelName);
	item->setAttrData(attrdata);
	item->setType(type);

	labelItem->addChild(item);


}
void LabelViewer::addNodeLabelToData(QString &labelName, QSet<int>& attrdata)
{

}

void LabelViewer::addElemLabelToData(QString &labelName, QSet<int>& attrdata)
{

}

void LabelViewer::addLabelToData(QString &labelName, QSet<int>& attrdata, SETTYPE type)
{
	//ModelConfigmap 这个内部数据结构里面增加
	QString varStr;
	for each (int var in attrdata)
	{
		varStr += QString("%1 ").arg(var);
	}
	MainWindow *mainWindow = (MainWindow *) parent();
	if (type == SETTYPE_ELEM)
	{
		mainWindow->projectManager.addElemLabelToModelConfig(labelName, varStr);
	}
	else if (type == SETTYPE_NODE)
	{
		mainWindow->projectManager.addNodeLabelToModelConfig(labelName, varStr);
	}
}

void LabelViewer::add(SETTYPE labelType)
{
	bool ok;
	QString text = QInputDialog::getText(this, tr("input Label name"),
		tr("Label name:"), QLineEdit::Normal,
		0, &ok);

	if (ok && !text.isEmpty())
	{
		MainWindow *mainWindow = (MainWindow *) parent();

		if (labelType == SETTYPE_NODE)
		{
			QSet<int> nodes = mainWindow->meshViewer->getSelectNodes();
			if (nodes.empty())
			{
				QMessageBox::warning(mainWindow, "empty", "please select nodes first!");
				return;
			}
			addNodeLabelToUI(text, nodes);
			addLabelToData(text, nodes, SETTYPE_NODE);
		}
		else if (labelType == SETTYPE_ELEM)
		{
			QSet<int> elems = mainWindow->meshViewer->getSelectElems();
			if (elems.empty())
			{
				QMessageBox::warning(mainWindow, "empty", "please select elements first!");
				return;
			}
			addElemLabelToUI(text, elems);
			addLabelToData(text, elems, SETTYPE_ELEM);
		}
	}
}

void LabelViewer::onAddBtn()
{
	add(SETTYPE_NODE);
}

void LabelViewer::onAddBtn2()
{
	add(SETTYPE_ELEM);
}

void LabelViewer::onRemoveBtn()
{

}

void LabelViewer::onSelectPointBtn()
{
	if (selectPointBtn->isChecked())
	{
		selectPointBtn->setChecked(true);
		emit selectStatus(Select_Type_Point);
	}
	else
	{
		selectPointBtn->setChecked(false);
		emit selectStatus(Select_Type_None);
	}
	
}

void LabelViewer::onSelectCellBtn()
{
	if (selectCellBtn->isChecked())
	{
		selectCellBtn->setChecked(true);
		emit selectStatus(Select_Type_Cell);
	}
	else
	{
		selectCellBtn->setChecked(false);
		emit selectStatus(Select_Type_None);
	}

}