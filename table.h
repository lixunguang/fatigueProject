#ifndef TABLE_H_
#define TABLE_H_

#include <QWidget>
#include <QGroupBox>
#include <QTableView>

#include "model.h"

class  Table : public QWidget
{
	Q_OBJECT
public:
	Table(QString type, QWidget *parent = 0);
	~Table();

	public slots:
	void setText(QString text);
	QString text();
	void onAddClicked();
	void onRemove(int idx);
	void onUpClicked();
	void onDownClicked();

	public:
	QGroupBox *grp;
	QTableView *table;
	Model *model;
};

#endif