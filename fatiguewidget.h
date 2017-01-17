#ifndef FATIGUEWIDGET_H_
#define FATIGUEWIDGET_H_

#include <QWidget>
#include <QMap>

class  FatigueWidget : public QWidget
{
	Q_OBJECT
public:
	FatigueWidget(QWidget *parent = 0);
	~FatigueWidget();

public slots:
void writeToFile();

public:
	void updateUi(QMap<QString, QString> &data);
	void updateData(QMap<QString, QString> &data);
};

#endif