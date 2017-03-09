
#include <QDebug>

#include "operationviewer.h"


OperationViewer::OperationViewer(QWidget *parent)
	:QDockWidget(parent),  caseManager(this)
{
	this->setFeatures(QDockWidget::NoDockWidgetFeatures);

	fatigueWidget = new FatigueWidget();
	this->setWidget(fatigueWidget);
}

OperationViewer::~OperationViewer()
{

}

CaseManager *OperationViewer::getCaseManager()
{
	return &caseManager;
}

