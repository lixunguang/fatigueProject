#ifndef LOADWIDGET_H_
#define LOADWIDGET_H_

#include <QComboBox>
#include <QLineEdit>

#include <QDebug>

#include "loadeventdialog.h"

class  LoadWidget : public QWidget
{
	Q_OBJECT
public:
	LoadWidget(QWidget *parent = 0);
	~LoadWidget();

public slots:
	void showLoadEventWidget();
	void typeChanged(const QString &  str);
private:
	LoadEventDialog *loadEventDialog;
	LoadEventDialog *loadEventDialog_duty;
	LoadEventDialog *loadEventDialog_history;
	LoadEventDialog *loadEventDialog_super;
 
	QComboBox* loadTypeCombo;
	QTextEdit* loadEventEdit;

};

#endif