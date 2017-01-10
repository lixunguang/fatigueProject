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