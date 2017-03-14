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

	void setAttrData(QMap<QString, QString>& data);

	QMap<QString, QString>& getAttrData();

	void setType(SetType type);
public:
	bool _show;

	QMap<QString, QString> attrData2;
	SetType type;
};

#endif