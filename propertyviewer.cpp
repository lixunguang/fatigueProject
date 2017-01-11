
#include <QTextEdit>
#include <QVBoxLayout>
#include <QFrame>
#include <QDebug>

#include "propertyviewer.h"

PropertyViewer::PropertyViewer(QWidget *parent)
	:QDockWidget(parent)
{
	this->setFeatures(QDockWidget::NoDockWidgetFeatures);

	QFrame *dockWidget = new QFrame();
	this->setWidget(dockWidget);

	QVBoxLayout *vLayout = new QVBoxLayout();
	dockWidget->setLayout(vLayout);


	QTextEdit *nodeEdit = new QTextEdit();
 
	vLayout->addWidget(nodeEdit);
 
}

PropertyViewer::~PropertyViewer()
{

}