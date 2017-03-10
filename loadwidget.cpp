#include <QVBoxLayout>
#include <QLabel>
#include <QStringList>
#include <QPushButton>
#include <QTextEdit>

#include "loadwidget.h"

LoadWidget::LoadWidget(QWidget *parent)
	:QWidget(parent)
{
	QVBoxLayout *layout = new QVBoxLayout();
	QGroupBox* grpLoad = new QGroupBox("Define Loading(s)");
	QVBoxLayout *grpLoadVBox = new QVBoxLayout();

	QHBoxLayout* loadTypeHBox = new QHBoxLayout();
	QLabel* loadTypeLabel = new QLabel("Loading Type:    ");
	loadTypeCombo = new QComboBox();
	QStringList items;
	items << "Duty Cycle" << "Loading History" << "Superimposed";
	loadTypeCombo->addItems(items);// #todo
	loadTypeHBox->addWidget(loadTypeLabel);
	loadTypeHBox->addWidget(loadTypeCombo);

	QHBoxLayout* loadEventHBox = new QHBoxLayout();
	QLabel* loadEventLabel = new QLabel("Loading Events:      ");
	loadEventEdit = new QTextEdit();
	QPushButton* loadEventBtn = new QPushButton("...");
	loadEventHBox->addWidget(loadEventLabel);
	loadEventHBox->addWidget(loadEventEdit);
	loadEventHBox->addWidget(loadEventBtn);

	grpLoadVBox->addLayout(loadTypeHBox);
	grpLoadVBox->addLayout(loadEventHBox);
	QSpacerItem *spacer = new QSpacerItem(100, 300, QSizePolicy::Minimum);
	grpLoadVBox->addSpacerItem(spacer);

	grpLoad->setLayout(grpLoadVBox);
	layout->addWidget(grpLoad);

	this->setLayout(layout);

	loadEventDialog = NULL;
 	loadEventDialog_duty = new LoadEventDialog("Duty Cycle", this);
	loadEventDialog_history = new LoadEventDialog("Loading History", this);
	loadEventDialog_super = new LoadEventDialog("Superimposed", this);
//#
	connect(loadEventBtn, SIGNAL(clicked()), this, SLOT(showLoadEventWidget()));
	connect(loadTypeCombo, SIGNAL(currentIndexChanged(const QString & )), this, SLOT(typeChanged(const QString &)));

}

LoadWidget::~LoadWidget()
{

}

void LoadWidget::showLoadEventWidget()
{
	if (loadTypeCombo->currentText() == "Duty Cycle")
	{
		loadEventDialog = loadEventDialog_duty;
	}
	else if (loadTypeCombo->currentText() == "Loading History")
	{
		loadEventDialog = loadEventDialog_history;
	}
	else if (loadTypeCombo->currentText() == "Superimposed")
	{
		loadEventDialog = loadEventDialog_super;
	}

	if (QDialog::Accepted == loadEventDialog->exec())
	{
		QStringList sl = loadEventDialog->tabwidget->model->modelData_();
		sl.append(QString("%1 events").arg(sl.size()));
		loadEventEdit->setText(sl.join("\n"));
	}
}

void LoadWidget::typeChanged(const QString &  str)
{
	loadEventEdit->clear();

}