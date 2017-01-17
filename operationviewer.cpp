
#include <QDebug>

#include "operationviewer.h"


OperationViewer::OperationViewer(QWidget *parent)
	:QDockWidget(parent)
{
	this->setFeatures(QDockWidget::NoDockWidgetFeatures);

	fatigueWidget = new FatigueWidget();
	this->setWidget(fatigueWidget);
}

OperationViewer::~OperationViewer()
{

}