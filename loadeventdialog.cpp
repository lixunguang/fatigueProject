#include "loadeventdialog.h"

#include <QLabel>
#include <QTextEdit>
#include <QGroupBox>
#include <QHBoxLayout>
#include <QVBoxLayout>

#include <QTableWidget>
#include <QPushButton>
#include <QDebug>

#include <table.h>

LoadEventDialog::LoadEventDialog(QString type, QWidget *parent)
:QDialog(parent)
{
	QHBoxLayout *buttonLayout = new QHBoxLayout();
	QPushButton *cancelBtn = new QPushButton("cancel");
	QPushButton *okBtn = new QPushButton("ok");
	buttonLayout->addWidget(cancelBtn);
	buttonLayout->addWidget(okBtn);

	QVBoxLayout *layout = new QVBoxLayout();
	Table *tabwidget = new Table(type,this );
	layout->addWidget(tabwidget);
	layout->addLayout(buttonLayout);

	this->setLayout(layout);

	//	qtc.QObject.connect(self.okBtn, qtc.SIGNAL('clicked()'), self.accept)
	//	qtc.QObject.connect(self.cancelBtn, qtc.SIGNAL('clicked()'), self.reject)
}

LoadEventDialog::~LoadEventDialog()
{

}

 