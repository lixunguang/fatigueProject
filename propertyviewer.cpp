#include "propertyviewer.h"

#include <QLabel>
#include <QTextEdit>
#include <QGroupBox>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QFrame>
#include <QDebug>


PropertyViewer::PropertyViewer(QWidget *parent)
	:QDockWidget(parent)
{
	QFrame *dockWidget = new QFrame();
	this->setWidget(dockWidget);

	QVBoxLayout *vLayout = new QVBoxLayout();
	dockWidget->setLayout(vLayout);

	QVBoxLayout *pointLayout = new QVBoxLayout();
	QVBoxLayout *cellLayout = new QVBoxLayout();

	QTextEdit *nodeEdit = new QTextEdit();
	QTextEdit *elemEdit = new QTextEdit();

	QGroupBox *selectPointGroup = new QGroupBox();
	selectPointGroup->setTitle("Nodes: ");
	selectPointGroup->setCheckable(true);

	pointLayout->addWidget(nodeEdit);
	selectPointGroup->setLayout(pointLayout);


	QGroupBox *selectCellGroup = new QGroupBox();
	selectCellGroup->setTitle("Elements: ");
	selectCellGroup->setCheckable(true);
 
	cellLayout->addWidget(elemEdit);
	selectCellGroup->setLayout(cellLayout);


	vLayout->addWidget(selectPointGroup);
	vLayout->addWidget(selectCellGroup);
}

PropertyViewer::~PropertyViewer()
{

}