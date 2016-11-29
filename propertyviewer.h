#ifndef propertyview_H_
#define propertyview_H_

#include "QDockWidget.h"

class  PropertyViewer : public QDockWidget
{
	Q_OBJECT
public:
	PropertyViewer(QWidget *parent = 0);
	~PropertyViewer();

};

#endif