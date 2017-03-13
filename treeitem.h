#ifndef TREEITEM_H_
#define TREEITEM_H_


#include <QTreeWidgetItem>
#include "mesh.h"

class  TreeItem : public QTreeWidgetItem
{

public:
	TreeItem();
	~TreeItem();

	bool show()
	{
		return _show;
	}

	void setShow(bool show)
	{
		_show = show;
	}

	void setAttrData(QSet<int> &data);
	QSet<int> & getAttrData();

	void setType(SetType type);
public:
	bool _show;
	QSet<int> attrData;
	SetType type;
};

#endif