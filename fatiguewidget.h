#ifndef fatiguewidget_H_
#define fatiguewidget_H_

#include "QWidget.h"

class  FatigueWidget : public QWidget
{
	Q_OBJECT
public:
	FatigueWidget(QWidget *parent = 0);
	~FatigueWidget();

public slots:
void onFinish();
};

#endif