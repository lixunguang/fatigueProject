#include "table.h"

#include <QLabel>
#include <QTextEdit>
#include <QGroupBox>
#include <QTableView>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QPushButton>
#include <QDebug>
#include <QSpacerItem>

#include <model.h>
#include <delegate.h>

Table::Table(QString type, QWidget *parent)
:QWidget(parent)
{
	QVBoxLayout *layout = new QVBoxLayout();

	QGroupBox *grp = new QGroupBox();
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

	QTableView *table = new QTableView();
	Model *model = new Model(type);

	table->setModel(model);

	grpLayout->addLayout(toolLayout);
	grpLayout->addWidget(table);

	table->setItemDelegate(new Delegate(this));
	layout->addWidget(grp);

	//#tool button  clicked
	//self.connect(self._toolAddBtn, qtc.SIGNAL('clicked()'), self.onAddClicked);
	//self.connect(self._table.verticalHeader(), qtc.SIGNAL('sectionClicked(int)'), self.onRemove);
	//self.connect(self._toolUpBtn, qtc.SIGNAL('clicked()'), self.onUpClicked);
	//self.connect(self._toolDownBtn, qtc.SIGNAL('clicked()'), self.onDownClicked);
}

Table::~Table()
{

}

