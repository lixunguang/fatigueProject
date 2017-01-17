
#include <QVBoxLayout>
#include <QPushButton>
#include <QDebug>

#include "fatiguewidget.h"
#include "analysiswidget.h"
#include "loadWidget.h"

FatigueWidget::FatigueWidget(QWidget *parent)
	:QWidget(parent)
{
	QVBoxLayout *layout = new QVBoxLayout();
	QTabWidget *tab = new QTabWidget();

	AnalysisWidget *analysisWidget = new AnalysisWidget();
	LoadWidget *loadWidget = new LoadWidget();

	QPushButton *finishBtn = new QPushButton("write to file");
	
	tab->addTab(analysisWidget, "analysis settings");
	tab->addTab(loadWidget, "load settings");


	layout->addWidget(tab);
	layout->addWidget(finishBtn);

	connect(finishBtn, SIGNAL(clicked()), this, SLOT(writeToFile()));

	this->setLayout(layout);
}

FatigueWidget::~FatigueWidget()
{

}

void FatigueWidget::writeToFile()
{
	qDebug() << "FatigueWidget::writeToFile";
}

void FatigueWidget::updateUi(QMap<QString,QString> &data)
{//from Data to UI
	//according to object name
}

void FatigueWidget::updateData(QMap<QString, QString> &data)
{//From UI to Data

}