#ifndef FATIGUEWIDGET_H_
#define FATIGUEWIDGET_H_

#include <QWidget>
#include <QMap>

enum OBJECTTYPE
{//2÷÷ ‰»Î£¨edit,comb
	type_NONE,
	type_edit,
	type_combo,
};
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
	void updateData(QMap<QString, QString> &mapData);
};

#endif