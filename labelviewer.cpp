
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QPushButton>
#include <QInputDialog>
#include <QMessageBox>
#include <QIcon>
#include <QDebug>

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

void LabelViewer::onItemPressed(QTreeWidgetItem * item, int column)
{
	if (column != 0)
	{
		return;
	}

	TreeItem *_item = (TreeItem*)item;
	QIcon icon = item->icon(0);
	bool show = _item->show();
	 
	if (show)
	{
		_item->setShow(false);
		icon.addPixmap(QPixmap(":/res/label/label_visibility_off.png"));

		//hide children if item is top item
		if (item == labelItem)
		{
			int cnt = item->childCount();
			while (cnt--)
			{
				TreeItem *childItem = (TreeItem *)item->child(cnt);
				childItem->setShow(false);

				icon.addPixmap(QPixmap(":/res/label/label_visibility_off.png"));
				childItem->setIcon(0, icon);
			}
		}
	}
	else
	{
		_item->setShow(true);
		icon.addPixmap(QPixmap(":/res/label/label_visibility_on.png"));

		//hide children if item is top item
		if (item == labelItem)
		{
			int cnt = item->childCount();
			while (cnt--)
			{
				TreeItem *childItem = (TreeItem *)item->child(cnt);
				childItem->setShow(true);

				icon.addPixmap(QPixmap(":/res/label/label_visibility_on.png"));
				childItem->setIcon(0, icon);
			}
		}
	}
	
	item->setIcon(0, icon);
}

void LabelViewer::onAddLabel(QString &labelName)
{
	QIcon icon;
	icon.addPixmap(QPixmap(":/res/label/label_visibility_off.png"));

	TreeItem *item = new TreeItem();
	item->setShow(false);
	item->setIcon(0, icon);
	item->setText(1, labelName);

	labelItem->addChild(item);
}

void LabelViewer::onAddBtn()
{
	onAddLabel(QString("sdfsdf"));
	return;

	bool ok;
	QString text = QInputDialog::getText(this, tr("input Label name"),	
		tr("Label name:"), QLineEdit::Normal,
		0, &ok);

	if (ok && !text.isEmpty())
	{
		TreeItem *item = new TreeItem();
		item->setText(1,"asdfsd");

		labelItem->addChild(item);
	}
		
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