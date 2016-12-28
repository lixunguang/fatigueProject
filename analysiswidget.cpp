#include "analysiswidget.h"

#include <QLabel>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QComboBox>
#include <QPushButton>
#include <QGroupBox>
#include <QLineEdit>
#include <QComboBox>


#include <QDebug>


AnalysisWidget::AnalysisWidget(QWidget *parent)
:QWidget(parent)
{
	//#build ui
	QVBoxLayout *layout = new QVBoxLayout();

	//##Algorithm
	QHBoxLayout *algorithmHBox = new QHBoxLayout();
	QLabel * algorithmLabel = new QLabel("Algorithm:");
	QComboBox *algorithmCombo = new QComboBox();
	QStringList items;
	items << "Stress Life(Manson-Coffin)" << "Stress Life(Morrow)" << "Stress Life(BWI Weld)" <<
		"Strain Life(Manson-Coffin)" << "Strain Life(Morrow)" << "Strain Life(S-W-T)" <<
		"Safety Factor(Goodman)" << "Safety Factor(Gerber)";
	algorithmCombo->addItems(items);


	algorithmHBox->addWidget(algorithmLabel);
	algorithmHBox->addWidget(algorithmCombo);
	layout->addLayout(algorithmHBox, 1);

	//##Analysis Definition
	QGroupBox *grpAnalysis = new QGroupBox("Analysis Definition");
	QVBoxLayout* grpAnalysisVBox = new QVBoxLayout();


	QLabel *pathLabel = new QLabel("Path of material:             ");
	QLineEdit *pathEdit = new QLineEdit();
	pathEdit->setReadOnly(true);
	QPushButton *pathBtn = new QPushButton("...");
	QHBoxLayout *pathHBox = new QHBoxLayout();


	QLabel *meanStressLabel = new QLabel("Mean Stress Correction        ");
	QComboBox *meanStressCombo = new QComboBox();
	items.clear();
	items << "None" << "Goodman" << "Gerber" << "Sodeberg";
	meanStressCombo->addItems(items);
	QHBoxLayout *meanStressHBox = new QHBoxLayout();

	QLabel *inFluenceFactorsLabel = new QLabel("influence factors             ");
	QLineEdit *inFluenceFactorsEdit = new QLineEdit("1.0,1.0,1.0,1.0,1.0");
	inFluenceFactorsEdit->setReadOnly(true);
	QPushButton *inFluenceFactorsBtn = new QPushButton("...");
	QHBoxLayout *inFluenceFactorsHBox = new QHBoxLayout();

	QLabel *weldLabel = new QLabel("Weld Class               ");
	QComboBox *weldCombo = new QComboBox();
	items.clear();
	items << "B" << "C" << "D" << "E" << "F" << "G";
	weldCombo->addItems(items);
	QHBoxLayout* weldHBox = new QHBoxLayout();
	weldHBox->addWidget(weldLabel);
	weldHBox->addWidget(weldCombo);

	QLabel *sdNumLabel = new QLabel("Num of SD             ");
	QComboBox *sdNumCombo = new QComboBox();
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
	QGroupBox* grpRainFlow = new QGroupBox("RainFlow Parameters");
	QVBoxLayout* grpRainFlowVBox = new QVBoxLayout();

	QHBoxLayout* peakValleyHBox = new QHBoxLayout();
	QLabel* peakValleyLabel = new QLabel("Peak-Valley Reduction Factor (Between 0 to 1)");
	QComboBox *peakValleyCombo = new QComboBox();
	items.clear();
	items << "0.1" << "0.2" << "0.3" << "0.4" << "0.5" << "0.6" << "0.7" << "0.8" << "0.9" << "1.0";
	peakValleyCombo->addItems(items); //#todo
	peakValleyHBox->addWidget(peakValleyLabel);
	peakValleyHBox->addWidget(peakValleyCombo);

	QHBoxLayout* countHBox = new QHBoxLayout();
	QLabel* countLabel = new QLabel("Number of RAnges for counting(>0)");
	QComboBox* countCombo = new QComboBox();
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

	QLabel* entityTypeLabel = new QLabel("Entity Type                   ");
	QComboBox* entityTypeCombo = new QComboBox();
	items.clear();
	items << "Node" << "Element";
	entityTypeCombo->addItems(items);
	QHBoxLayout* entityTypeHBox = new QHBoxLayout();
	entityTypeHBox->addWidget(entityTypeLabel);
	entityTypeHBox->addWidget(entityTypeCombo);

	QLabel* entityNumLabel = new QLabel("Selected Entities             ");
	QLineEdit* entityNumEdit = new QLineEdit();
	entityNumEdit->setReadOnly(true);

	QPushButton*entityNumBtn = new QPushButton("update");
	QHBoxLayout*entityNumHBox = new QHBoxLayout();
	entityNumHBox->addWidget(entityNumLabel);
	entityNumHBox->addWidget(entityNumEdit);
	entityNumHBox->addWidget(entityNumBtn);

	grpEntitySelectionVBox->addLayout(entityTypeHBox);
	grpEntitySelectionVBox->addLayout(entityNumHBox);
	grpEntitySelection->setLayout(grpEntitySelectionVBox);
	layout->addWidget(grpEntitySelection, 2);

	this->setLayout(layout);

	/*
//#componet
self._InfluenceFactorDialog = InfluenceFactorDialog(self)
self._fileDialog = qt.QFileDialog(self, "select material file")


//signals
//1 algorthm clicked, change ui
self.connect(self._algorithmCombo, qtc.SIGNAL('currentIndexChanged(QString)'), self.algorithmChanged)
//2 path selector dialog
qtc.QObject.connect(self._inFluenceFactorsBtn, qtc.SIGNAL('clicked()'), self, qtc.SLOT('showInfuenceFactor()'))
//3 influence ui
qtc.QObject.connect(self._pathBtn, qtc.SIGNAL('clicked()'), self, qtc.SLOT('showFileDialog()'))
//4 entity type
#x = qtc.QObject.connect(self._entityTypeCombo, qtc.SIGNAL('currentIndexChanged(str)'), self, qtc.SLOT('entityTypeChanged(str)'))
self.connect(self._entityTypeCombo, qtc.SIGNAL('currentIndexChanged(QString)'), self.entityTypeChanged)
//5 entity Selection clicked
self._mesh = GLOBAL_MH
qtc.QObject.connect(self._entityNumBtn, qtc.SIGNAL('clicked()'), self._mesh, qtc.SLOT('getSecletion()'))*/
}

AnalysisWidget::~AnalysisWidget()
{

}

