#ifndef operationview_H_
#define operationview_H_

#include "QDockWidget.h"

class  OperationViewer : public QDockWidget
{
	Q_OBJECT
public:
	OperationViewer(QWidget *parent = 0);
	~OperationViewer();

};

#endif