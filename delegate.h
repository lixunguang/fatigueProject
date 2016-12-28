#ifndef Delegate_H_
#define Delegate_H_

#include <QObject>
#include <QStyledItemDelegate>

class  Delegate : public QStyledItemDelegate
{
	Q_OBJECT
public:
	Delegate(QObject *parent = 0);
	~Delegate();

public slots:

};

#endif