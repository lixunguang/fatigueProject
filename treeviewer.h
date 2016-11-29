#ifndef treeview_H_
#define treeview_H_

#include <QTreeWidget>
#include <QDockWidget>

class  TreeViewer : public QDockWidget
{
	Q_OBJECT
public:
	TreeViewer(QWidget *parent = 0);
	~TreeViewer();

private:
	QTreeWidget *tree;
};

#endif