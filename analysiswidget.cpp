
#include <QLabel>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QComboBox>
#include <QPushButton>
#include <QGroupBox>
#include <QLineEdit>
#include <QComboBox>
#include <QDebug>

#include "analysiswidget.h"

AnalysisWidget::AnalysisWidget(QWidget *parent)
:QWidget(parent)
{
	//#build ui
	QVBoxLayout *layout = new QVBoxLayout();

	//##Algorithm
	QHBoxLayout *algorithmHBox = new QHBoxLayout();
	algorithmLabel = new QLabel("Algorithm:");
	algorithmCombo = new QComboBox();
	algorithmCombo->setObjectName("AlgorithmSection_algorithmCombo");
	QStringList items;
	items << "Stress Life(Manson-Coffin)" << "Stress Life(Morrow)" << "Stress Life(BWI Weld)" <<
		"Strain Life(Manson-Coffin)" << "Strain Life(Morrow)" << "Strain Life(S-W-T)" <<
		"Safety Factor(Goodman)" << "Safety Factor(Gerber)";
	algorithmCombo->addItems(items);


	algorithmHBox->addWidget(algorithmLabel);
	algorithmHBox->addWidget(algorithmCombo);
	layout->addLayout(algorithmHBox, 1);

	//##Analysis Definition
	grpAnalysis = new QGroupBox("Analysis Definition");
	QVBoxLayout* grpAnalysisVBox = new QVBoxLayout();


	pathLabel = new QLabel("Path of material:             ");
	pathEdit = new QLineEdit();
	pathEdit->setObjectName("AnalisisDefSection_pathEdit");
	pathEdit->setReadOnly(true);
	pathBtn = new QPushButton("...");
	QHBoxLayout *pathHBox = new QHBoxLayout();


	meanStressLabel = new QLabel("Mean Stress Correction        ");
	meanStressCombo = new QComboBox();
	meanStressCombo->setObjectName("AnalisisDefSection_meanStressCombo");
	items.clear();
	items << "None" << "Goodman" << "Gerber" << "Sodeberg";
	meanStressCombo->addItems(items);
	QHBoxLayout *meanStressHBox = new QHBoxLayout();

	inFluenceFactorsLabel = new QLabel("influence factors             ");
	inFluenceFactorsEdit = new QLineEdit("1.0,1.0,1.0,1.0,1.0");
	inFluenceFactorsEdit->setObjectName("AnalisisDefSection_inFluenceFactorsEdit");
	inFluenceFactorsEdit->setReadOnly(true);
	inFluenceFactorsBtn = new QPushButton("...");
	QHBoxLayout *inFluenceFactorsHBox = new QHBoxLayout();

	weldLabel = new QLabel("Weld Class               ");
	weldCombo = new QComboBox();
	weldCombo->setObjectName("AnalisisDefSection_weldCombo");
	items.clear();
	items << "B" << "C" << "D" << "E" << "F" << "G";
	weldCombo->addItems(items);
	QHBoxLayout* weldHBox = new QHBoxLayout();
	weldHBox->addWidget(weldLabel);
	weldHBox->addWidget(weldCombo);

	sdNumLabel = new QLabel("Num of SD             ");
	sdNumCombo = new QComboBox();
	sdNumCombo->setObjectName("AnalisisDefSection_sdNumCombo");
	items.clear();
	items << "2" << "3" << "4" << "5";
	sdNumCombo->addItems(items);
	QHBoxLayout *sdNumHBox = new QHBoxLayout();
	sdNumHBox->addWidget(sdNumLabel);
	sdNumHBox->addWidget(sdNumCombo);

	pathHBox->addWidget(pathLabel);
	pathHBox->addWidget(pathEdit);
	pathHBox->addWidget(pathBtn);
	meanStressHBox->addWidget(meanStressLabel);
	meanStressHBox->addWidget(meanStressCombo);
	inFluenceFactorsHBox->addWidget(inFluenceFactorsLabel);
	inFluenceFactorsHBox->addWidget(inFluenceFactorsEdit);
	inFluenceFactorsHBox->addWidget(inFluenceFactorsBtn);
	grpAnalysisVBox->addLayout(pathHBox);
	grpAnalysisVBox->addLayout(meanStressHBox);
	grpAnalysisVBox->addLayout(weldHBox);
	grpAnalysisVBox->addLayout(sdNumHBox);
	grpAnalysisVBox->addLayout(inFluenceFactorsHBox);
	grpAnalysis->setLayout(grpAnalysisVBox);
	layout->addWidget(grpAnalysis, 3);

	//##RainFlow Parameters
	grpRainFlow = new QGroupBox("RainFlow Parameters");
	QVBoxLayout* grpRainFlowVBox = new QVBoxLayout();

	QHBoxLayout* peakValleyHBox = new QHBoxLayout();
	peakValleyLabel = new QLabel("Peak-Valley Reduction Factor (Between 0 to 1)");
	peakValleyCombo = new QComboBox();
	peakValleyCombo->setObjectName("RainFlowSection_peakValleyCombo");
	items.clear();
	items << "0.1" << "0.2" << "0.3" << "0.4" << "0.5" << "0.6" << "0.7" << "0.8" << "0.9" << "1.0";
	peakValleyCombo->addItems(items); //#todo
	peakValleyHBox->addWidget(peakValleyLabel);
	peakValleyHBox->addWidget(peakValleyCombo);

	QHBoxLayout* countHBox = new QHBoxLayout();
	countLabel = new QLabel("Number of RAnges for counting(>0)");
	countCombo = new QComboBox();
	countCombo->setObjectName("RainFlowSection_countCombo");
	items.clear();
	items << "10" << "20";
	countCombo->addItems(items);// #todo
	countHBox->addWidget(countLabel);
	countHBox->addWidget(countCombo);

	grpRainFlowVBox->addLayout(peakValleyHBox);
	grpRainFlowVBox->addLayout(countHBox);
	grpRainFlow->setLayout(grpRainFlowVBox);
	layout->addWidget(grpRainFlow, 2);

	//##Entity Selection
	QGroupBox* grpEntitySelection = new QGroupBox("Entity Selection");
	QVBoxLayout* grpEntitySelectionVBox = new QVBoxLayout();

	entityTypeLabel = new QLabel("Entity Type                   ");
	entityTypeCombo = new QComboBox();
	entityTypeCombo->setObjectName("EntitySection_entityTypeCombo");
	items.clear();
	items << "Node" << "Element";
	entityTypeCombo->addItems(items);
	QHBoxLayout* entityTypeHBox = new QHBoxLayout();
	entityTypeHBox->addWidget(entityTypeLabel);
	entityTypeHBox->addWidget(entityTypeCombo);

	entityNumLabel = new QLabel("Selected Entities             ");
	entityNumEdit = new QLineEdit();
	entityNumEdit->setObjectName("EntitySection_entityNumEdit");
	entityNumEdit->setReadOnly(true);

	entityNumBtn = new QPushButton("update");
	QHBoxLayout*entityNumHBox = new QHBoxLayout();
	entityNumHBox->addWidget(entityNumLabel);
	entityNumHBox->addWidget(entityNumEdit);
	entityNumHBox->addWidget(entityNumBtn);

	grpEntitySelectionVBox->addLayout(entityTypeHBox);
	grpEntitySelectionVBox->addLayout(entityNumHBox);
	grpEntitySelection->setLayout(grpEntitySelectionVBox);
	layout->addWidget(grpEntitySelection, 2);

	this->setLayout(layout);

	
//#componet
	influenceFactorDialog = new InfluenceFactorDialog(this);
	fileDialog = new QFileDialog(this, "select material file");


		//signals
		//1 algorithm clicked, change ui
		connect(algorithmCombo, SIGNAL(currentIndexChanged(int)), this, SLOT(algorithmChanged(int)));
//2 path selector dialog
	connect(inFluenceFactorsBtn, SIGNAL(clicked()), this, SLOT(showInfuenceFactor()));
//3 influence ui
	connect(pathBtn, SIGNAL(clicked()), this, SLOT(showFileDialog()));
//4 entity type
	connect(entityTypeCombo, SIGNAL(currentIndexChanged(int)), this, SLOT(entityTypeChanged(int)));
	
//5 entity Selection clicked
	//mesh = GLOBAL_MH
	//connect(entityNumBtn, SIGNAL(clicked()), mesh, SLOT(getSecletion()));
}

AnalysisWidget::~AnalysisWidget()
{

}

void AnalysisWidget::showInfuenceFactor()
{
	QString resStr;
	if (QDialog::Accepted == influenceFactorDialog->exec())
	{
		resStr = QString("%1,%2,%3,%4,%5").arg(influenceFactorDialog->NotchEdit->text())
			.arg(influenceFactorDialog->SurfaceEdit->text())
			.arg(influenceFactorDialog->SizeEdit->text())
			.arg(influenceFactorDialog->LoadEdit->text())
			.arg(influenceFactorDialog->OtherhEdit->text());

			inFluenceFactorsEdit->setText(resStr);
	}

}



void AnalysisWidget::showFileDialog()
{
	qDebug() << "AnalysisWidget::showFileDialog";
	QString fileName = fileDialog->getOpenFileName(NULL, "select material file", "c:/");
 
	if (!fileName.isEmpty())
	{
		pathEdit->setText(fileName);
	}
	
}



void AnalysisWidget::entityTypeChanged(int index)
{
	//mesh._meshType = entityTypeCombo.currentText()
}



//#when algorithm Changed, change ui

void AnalysisWidget::algorithmChanged(int index)
{
	if (algorithmCombo->currentIndex() == 0 ||
		algorithmCombo->currentIndex() == 1 ||
		algorithmCombo->currentIndex() == 6 || 
		algorithmCombo->currentIndex() == 7)
	{
		showStressUi();
	}
	else if (algorithmCombo->currentIndex() == 2 )
	{
		showBwiUi();
	}
	else if(algorithmCombo->currentIndex() == 3 ||
		algorithmCombo->currentIndex() == 4 ||
		algorithmCombo->currentIndex() == 5 )
	{
		showStrainUi();
	}
	else
	{
	}

}


void AnalysisWidget::showStressUi()
{
	pathLabel->show();
	pathEdit->show();
	pathBtn->show();

	meanStressLabel->show();
	meanStressCombo->show();

	weldLabel->hide();
	weldCombo->hide();
	sdNumLabel->hide();
		sdNumCombo->hide();
}

void AnalysisWidget::showBwiUi()
{
	pathLabel->hide();
	pathEdit->hide();
	pathBtn->hide();

	meanStressLabel->hide();
	meanStressCombo->hide();

	weldLabel->show();
	weldCombo->show();
	sdNumLabel->show();
	sdNumCombo->show();
}

void AnalysisWidget::showStrainUi()
{
	pathLabel->show();
	pathEdit->show();
	pathBtn->show();

	meanStressLabel->hide();
	meanStressCombo->hide();

	weldLabel->hide();
	weldCombo->hide();
	sdNumLabel->hide();
	sdNumCombo->hide();
}


void AnalysisWidget::writeToFile(QString fileName)
{
	//fw = InputFileWriter()
//#todo :
}
