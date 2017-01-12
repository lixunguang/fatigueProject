
#include <QLabel>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QDebug>
#include <QPushButton>

#include "loadeventdialog.h"

LoadEventDialog::LoadEventDialog(QString type, QWidget *parent)
:QDialog(parent)
{
	QHBoxLayout *buttonLayout = new QHBoxLayout();
	QPushButton *cancelBtn = new QPushButton("cancel");
	QPushButton *okBtn = new QPushButton("ok");
	buttonLayout->addWidget(cancelBtn);
	buttonLayout->addWidget(okBtn);

	QVBoxLayout *layout = new QVBoxLayout();
	tabwidget = new Table(type,this );
	layout->addWidget(tabwidget);
	layout->addLayout(buttonLayout);

	this->setLayout(layout);

	connect(okBtn, SIGNAL(clicked()), this, SLOT(accept()));
	connect(cancelBtn, SIGNAL(clicked()), SLOT(reject()));
}

LoadEventDialog::~LoadEventDialog()
{

}

void LoadEventDialog::setText(QString text)
{
	tabwidget->setText(text);
}


 