#ifndef LoadEventDialog_H_
#define LoadEventDialog_H_

#include "QDialog.h"
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