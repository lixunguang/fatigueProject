
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QPushButton>
#include <QInputDialog>
#include <QMessageBox>
#include <QIcon>
#include <QDebug>

#include "mesh.h"
#include "labelviewer.h"

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

	QPushButton *removeBtn = new QPushButton();
	removeBtn->setIcon(QIcon(QPixmap(":/res/label/label_remove.png")));
	connect(removeBtn, SIGNAL(clicked()), this, SLOT(onRemoveBtn()));

	QSpacerItem *space = new QSpacerItem(0, 0, QSizePolicy::MinimumExpanding, QSizePolicy::Minimum);

	QHBoxLayout *hLayout = new QHBoxLayout();
	hLayout->addWidget(addBtn);
	hLayout->addWidget(removeBtn);
	hLayout->addSpacerItem(space);

	vLayout->addLayout(hLayout);

	this->setWidget(labelBrowser);

	this->setFeatures(QDockWidget::NoDockWidgetFeatures);

	tree->setColumnCount(3);
	tree->setColumnWidth(0, 60);
	tree->setColumnWidth(2, 30);
	tree->setRootIsDecorated(true);

	connect(tree, SIGNAL(itemPressed(QTreeWidgetItem *, int)), this, SLOT(onItemPressed(QTreeWidgetItem *, int)));

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

void LabelViewer::onItemPressed(QTreeWidgetItem *item, int column)
{
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

void LabelViewer::addNodeLabel(QString &labelName, QString &attrdata)
{
	addLabel(labelName, attrdata, SETTYPE_NODE);
}

void LabelViewer::addElemLabel(QString &labelName, QString &attrdata)
{
	addLabel(labelName, attrdata, SETTYPE_ELEM);
}

void LabelViewer::addLabel(QString &labelName, QString &attrdata, SETTYPE type)
{
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

void LabelViewer::onAddBtn()
{


}

void LabelViewer::onRemoveBtn()
{
	bool ok;
	QString text = QInputDialog::getText(this, tr("input Label name"),
		tr("Label name:"), QLineEdit::Normal,
		0, &ok);

	if (ok && !text.isEmpty())
	{
		TreeItem *item = new TreeItem();
		item->setText(1, "asdfsd");

		OperationItem->addChild(item);
	}
}