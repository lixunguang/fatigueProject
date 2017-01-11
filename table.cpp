#include "table.h"


#include <QGroupBox>
#include <QTableView>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QPushButton>
#include <QDebug>
#include <QSpacerItem>
#include <QModelIndex>

#include <model.h>
#include <delegate.h>

Table::Table(QString type, QWidget *parent)
:QWidget(parent)
{
	QVBoxLayout *layout = new QVBoxLayout();

	grp = new QGroupBox();
	QVBoxLayout *grpLayout = new QVBoxLayout(grp);

	QPushButton *toolAddBtn = new QPushButton("add");
	QPushButton *toolRemoveBtn = new QPushButton("remove");
	QPushButton *toolUpBtn = new QPushButton("up");
	QPushButton *toolDownBtn = new QPushButton("down");
	QHBoxLayout *toolLayout = new QHBoxLayout();

	toolLayout->addWidget(toolAddBtn);
	toolLayout->addWidget(toolRemoveBtn);
	toolLayout->addWidget(toolUpBtn);
	toolLayout->addWidget(toolDownBtn);

	QSpacerItem *spacer = new QSpacerItem(100, 5, QSizePolicy::Minimum);
	toolLayout->addSpacerItem(spacer);

	table = new QTableView();
	model = new Model(type);

	table->setModel(model);

	grpLayout->addLayout(toolLayout);
	grpLayout->addWidget(table);

	table->setItemDelegate(new Delegate(this));
	layout->addWidget(grp);

	this->setLayout(layout);

	//#tool button  clicked
	//self.connect(self._toolAddBtn, qtc.SIGNAL('clicked()'), self.onAddClicked);
	//self.connect(self._table.verticalHeader(), qtc.SIGNAL('sectionClicked(int)'), self.onRemove);
	//self.connect(self._toolUpBtn, qtc.SIGNAL('clicked()'), self.onUpClicked);
	//self.connect(self._toolDownBtn, qtc.SIGNAL('clicked()'), self.onDownClicked);
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
	model->insertColumns(cnt, 1, QModelIndex());

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
	/*QModelIndex midx = table->currentIndex();
	int r = midx.row();

	int r1 = r + 1;

	if (r1 <= model->modelData().size()) - 1)
	{
	}
	else
	{
		r1 = r;
	}


	if (r1 <= model->modelData().size() - 1 && r1 != r)
	{
		model->beginMoveRows(qtc.QModelIndex(), r1, r1, qtc.QModelIndex(), r);
		//#change
		first = model->modelData()[r];
		second = model->modelData()[r1];
		model->modelData()[r] = second;
		model->modelData()[r1] = first;

		model->endMoveRows();
	}
	*/

	QModelIndex midx = table->currentIndex();
	int r = midx.row();

	int r1 = 0;
	if (r - 1 >= 0)
	{
		r1 = r - 1;
	}
	else
	{
		r1 = 0;
	}

	if (r > 0 && r1 != r)
		model->moveRows(QModelIndex(), r, 1, QModelIndex(), r1);
}
