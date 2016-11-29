#include "propertyviewer.h"

#include <QLabel>
#include <QLineEdit>
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

	QLabel *nodeLabel = new QLabel("Nodes: ");
	QLineEdit *nodeEdit = new QLineEdit();

	QLabel *elemLabel = new QLabel("Elements: ");
	QLineEdit *elemEdit = new QLineEdit();

	vLayout->addWidget(nodeLabel);
	vLayout->addWidget(nodeEdit);
	vLayout->addWidget(elemLabel);
	vLayout->addWidget(elemEdit);

	qDebug() << "nodeLabel->parent()";
	qDebug() << nodeLabel->parent();

}

PropertyViewer::~PropertyViewer()
{

}