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

	void setAttrData(QString& data);
	void setType(SETTYPE type);
private: 
	bool _show;
	QString attrData;
	SETTYPE type;
};

#endif