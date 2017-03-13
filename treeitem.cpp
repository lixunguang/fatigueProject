#include "treeitem.h"


TreeItem::TreeItem()
:QTreeWidgetItem(UserType)
{

	_show = false;
}

TreeItem::~TreeItem()
{

}

void TreeItem::setAttrData(QSet<int>& data)
{
	attrData = data;
}

QSet<int> & TreeItem::getAttrData()
{
	return attrData;
}

void TreeItem::setType(SetType	t)
{
	this->type = t;
}

