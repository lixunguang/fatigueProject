#ifndef FATIGUEWIDGET_H_
#define FATIGUEWIDGET_H_

#include <QWidget>
#include <QMap>
#include <QLineEdit>
#include "typedef.h"

class  FatigueWidget : public QWidget
{
	Q_OBJECT
public:
	FatigueWidget(QWidget *parent = 0);
	~FatigueWidget();

public slots:
void writeToFile();

public:
	void writeToMap(QMap<QString, QString> &mapData, QString objName , QString val,int type,int count);
	void updateUi(QMap<QString, QString> &mapData);
	void updateMapData(QMap<QString, QString> &mapData);
private:
	QLineEdit *filePathEdit;

};

#endif