#ifndef Table_H_
#define Table_H_

#include "QWidget.h"

class  Table : public QWidget
{
	Q_OBJECT
public:
	Table(QString type,QWidget *parent = 0);
	~Table();

public slots:

};

#endif