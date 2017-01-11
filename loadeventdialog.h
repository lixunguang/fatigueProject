#ifndef LOADEVENTDIALOG_H_
#define LOADEVENTDIALOG_H_

#include <QDialog>

#include "table.h"

class  LoadEventDialog : public QDialog
{
	Q_OBJECT
public:
	LoadEventDialog(QString type, QWidget *parent = 0);
	~LoadEventDialog();
public:
	void setText(QString text);
public:
	Table *tabwidget;
 
};

#endif