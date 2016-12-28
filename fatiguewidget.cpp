#include "fatiguewidget.h"

#include <QLabel>
#include <QTextEdit>
#include <QGroupBox>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QFrame>
#include <QTableWidget>
#include <QPushButton>
#include <QDebug>

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

	connect(finishBtn, SIGNAL(clicked()), this, SLOT(onFinish()));

	this->setLayout(layout);
}

FatigueWidget::~FatigueWidget()
{

}

void FatigueWidget::onFinish()
{
	qDebug() << "FatigueWidget::onFinish";
}