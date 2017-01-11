#ifndef FATIGUEWIDGET_H_
#define FATIGUEWIDGET_H_

#include <QWidget>

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