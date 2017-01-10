
#include <QLabel>
#include <QTextEdit>
#include <QGroupBox>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QFrame>
#include <QDebug>

#include "operationviewer.h"
#include "fatiguewidget.h"

OperationViewer::OperationViewer(QWidget *parent)
	:QDockWidget(parent)
{
	this->setFeatures(QDockWidget::NoDockWidgetFeatures);

	FatigueWidget *fatigueWidget = new FatigueWidget();
	this->setWidget(fatigueWidget);
}

OperationViewer::~OperationViewer()
{

}