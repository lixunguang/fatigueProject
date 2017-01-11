#ifndef OPERATIONVIEW_H_
#define OPERATIONVIEW_H_

#include <QDockWidget>

class  OperationViewer : public QDockWidget
{
	Q_OBJECT
public:
	OperationViewer(QWidget *parent = 0);
	~OperationViewer();

};

#endif