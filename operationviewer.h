#ifndef OPERATIONVIEW_H_
#define OPERATIONVIEW_H_

#include <QDockWidget>
#include "fatiguewidget.h"

class  OperationViewer : public QDockWidget
{
	Q_OBJECT
public:
	OperationViewer(QWidget *parent = 0);
	~OperationViewer();

private:
	FatigueWidget *fatigueWidget;
};

#endif