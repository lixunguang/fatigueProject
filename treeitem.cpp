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

void TreeItem::setType(SETTYPE	type)
{
	this->type = type;
}

