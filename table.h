#ifndef Table_H_
#define Table_H_

#include <QWidget>
#include <QLabel>
#include <QTextEdit>
#include <QGroupBox>
#include <QTableView>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QPushButton>
#include <QDebug>
#include <QSpacerItem>
#include <QModelIndex>
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