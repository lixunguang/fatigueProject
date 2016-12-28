#include "loadwidget.h"

#include <QLabel>
#include <QTextEdit>
#include <QGroupBox>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QComboBox>
#include <QLineEdit>
#include <QPushButton>
#include <QDebug>


LoadWidget::LoadWidget(QWidget *parent)
	:QWidget(parent)
{
	QVBoxLayout *layout = new QVBoxLayout();
	QGroupBox* grpLoad = new QGroupBox("Define Loading(s)");
	QVBoxLayout *grpLoadVBox = new QVBoxLayout();

	QHBoxLayout* loadTypeHBox = new QHBoxLayout();
	QLabel* loadTypeLabel = new QLabel("Loading Type:    ");
	QComboBox* loadTypeCombo = new QComboBox();
	QStringList items;
	items << "Duty Cycle" << "Loading Hidstory" << "Superimposed";
	loadTypeCombo->addItems(items);// #todo
	loadTypeHBox->addWidget(loadTypeLabel);
	loadTypeHBox->addWidget(loadTypeCombo);

	QHBoxLayout* loadEventHBox = new QHBoxLayout();
	QLabel* loadEventLabel = new QLabel("Loading Events:      ");
	QLineEdit* loadEventEdit = new QLineEdit();
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

// 	loadEventDialog = None
// 		loadEventDialog_duty = LoadEventDialog("Duty Cycle");
// 	loadEventDialog_history = LoadEventDialog("Loading Hidstory");
// 	loadEventDialog_super = LoadEventDialog("Superimposed");
//#
	//qtc.QObject.connect(loadEventBtn, qtc.SIGNAL('clicked()'), self, qtc.SLOT('showLoadEventWidget()'));
	//qtc.QObject.connect(loadTypeCombo, qtc.SIGNAL('currentIndexChanged(QString)'), self.typeChanged);

}

LoadWidget::~LoadWidget()
{

}

 