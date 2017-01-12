#ifndef TREEITEM_H_
#define TREEITEM_H_


#include <QTreeWidgetItem>

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
private: 
	bool _show;
};

#endif