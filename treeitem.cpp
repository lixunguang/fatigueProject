#include "treeitem.h"


TreeItem::TreeItem()
:QTreeWidgetItem(UserType)
{

	_show = false;
}

TreeItem::~TreeItem()
{

}

void TreeItem::setAttrData(QMap<QString, QString>& data)
{
	attrData2 = data;
}



QMap<QString, QString>& TreeItem::getAttrData()
{
	return attrData2;
}

void TreeItem::setType(SetType	t)
{
	this->type = t;
}

