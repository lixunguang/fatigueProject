#include "treeitem.h"


TreeItem::TreeItem()
:QTreeWidgetItem(UserType)
{

	_show = false;
}

TreeItem::~TreeItem()
{

}

void TreeItem::setAttrData(QString& data)
{
	attrData = data;
}

QString& TreeItem::getAttrData()
{
	return attrData;
}

void TreeItem::setType(SETTYPE	t)
{
	this->type = t;
}

