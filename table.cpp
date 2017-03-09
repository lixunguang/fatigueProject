#include "table.h"

#include <QGroupBox>
#include <QTableView>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QPushButton>
#include <QDebug>
#include <QSpacerItem>
#include <QModelIndex>
#include <QHeaderView>

#include <model.h>
#include <delegate.h>

Table::Table(QString type, QWidget *parent)
:QWidget(parent)
{
	QVBoxLayout *layout = new QVBoxLayout();

	grp = new QGroupBox();
	QVBoxLayout *grpLayout = new QVBoxLayout(grp);

	QPushButton *toolAddBtn = new QPushButton("add");
//	QPushButton *toolRemoveBtn = new QPushButton("remove");
	QPushButton *toolUpBtn = new QPushButton("up");
	QPushButton *toolDownBtn = new QPushButton("down");
	QHBoxLayout *toolLayout = new QHBoxLayout();

	toolLayout->addWidget(toolAddBtn);
//	toolLayout->addWidget(toolRemoveBtn);
	toolLayout->addWidget(toolUpBtn);
	toolLayout->addWidget(toolDownBtn);

	QSpacerItem *spacer = new QSpacerItem(100, 5, QSizePolicy::Minimum);
	toolLayout->addSpacerItem(spacer);

	table = new QTableView(this);
	model = new Model(type,this);

	table->setModel(model);

	grpLayout->addLayout(toolLayout);
	grpLayout->addWidget(table);

	table->setItemDelegate(new Delegate(this));
	layout->addWidget(grp);

	this->setLayout(layout);

	QHeaderView *headerView = table->verticalHeader();
	//#tool button  clicked
	connect(toolAddBtn, SIGNAL(clicked()), this, SLOT(onAddClicked()));

	connect(headerView, SIGNAL(sectionClicked(int)), this, SLOT(onRemove(int)));
	connect(toolUpBtn, SIGNAL(clicked()), this, SLOT(onUpClicked()));
	connect(toolDownBtn, SIGNAL(clicked()), this, SLOT(onDownClicked()));
}

Table::~Table()
{

}

void Table::setText(QString text)
{
	grp->setTitle(text);
}

QString Table::text()
{
	return grp->title();
}

void Table::onAddClicked()
{
	int cnt = model->modelData().size();
	model->insertRows(cnt, 1, QModelIndex());

}

void Table::onRemove(int idx)
{
	model->removeRows(idx, 1, QModelIndex());

}

void Table::onUpClicked()
{
	QModelIndex midx = table->currentIndex();
	int r = midx.row();

	int r1 = 0;
	if( r - 1 >= 0)
	{
		r1 = r - 1;
	}
	else
	{
		r1 = 0;
	}

	if (r > 0 && r1 != r)
		model->moveRows(QModelIndex(), r,1, QModelIndex(),r1 );
}

void Table::onDownClicked()
{

	QModelIndex midx = table->currentIndex();
	int r = midx.row();

	int cnt = model->modelData_().size();

	int r1 = r+1;
	if (r <= cnt)
	{
	}
	else
	{
		r1 = r;
	}

	if (r1 <=cnt-1 && r1 != r)
		model->moveRows(QModelIndex(), r1, 1, QModelIndex(), r);
}
