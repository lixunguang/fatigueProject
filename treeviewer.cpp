#include "treeviewer.h"

#include <QVBoxLayout>
#include <QDebug>

TreeViewer::TreeViewer(QWidget *parent)
	:QDockWidget(parent)
{
	 
	tree = new QTreeWidget();
	this->setWidget(tree);

	qDebug() << "tree->parent()";
	qDebug() << tree->parent();
}

TreeViewer::~TreeViewer()
{

}