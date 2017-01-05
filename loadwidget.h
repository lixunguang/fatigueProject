#ifndef loadwidget_H_
#define loadwidget_H_

#include <QLabel>
#include <QTextEdit>
#include <QGroupBox>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QComboBox>
#include <QLineEdit>
#include <QPushButton>
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
private:
	LoadEventDialog *loadEventDialog;
	LoadEventDialog *loadEventDialog_duty;
	LoadEventDialog *loadEventDialog_history;
	LoadEventDialog *loadEventDialog_super;
 
	QComboBox* loadTypeCombo;
	QLineEdit* loadEventEdit;

};

#endif