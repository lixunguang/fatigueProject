#ifndef analysiswidget_H_
#define analysiswidget_H_

#include "QWidget.h"
#include "qfiledialog.h"
#include "QLineEdit.h"
#include "QTextEdit.h"
#include "QLabel.h"
#include "QComboBox.h"
#include "QGroupBox.h"
#include <QVector>

#include "influencefactordialog.h"

class  AnalysisWidget : public QWidget
{
	Q_OBJECT
public:
	AnalysisWidget(QWidget *parent = 0);
	~AnalysisWidget();

public slots:
	void entityTypeChanged(int index);
	void algorithmChanged(int index);
	void showStressUi();
	void showBwiUi();
	void showStrainUi();

	void showInfuenceFactor();
	void showFileDialog();
	void writeToFile(QString fileName);

	void updateSecletion();

private:
	InfluenceFactorDialog* influenceFactorDialog;
	QFileDialog *fileDialog;

	QLabel * algorithmLabel;
	QGroupBox *grpAnalysis;
	QLabel *pathLabel;
	QPushButton *pathBtn;
	QLabel *meanStressLabel;
	QComboBox *meanStressCombo;
	QLabel *inFluenceFactorsLabel;
	QPushButton *inFluenceFactorsBtn;
	QLabel *weldLabel;
	QComboBox *weldCombo;
	QLabel *sdNumLabel;
	QComboBox *sdNumCombo;
	QGroupBox* grpRainFlow;
	QLabel* peakValleyLabel;
	QComboBox *peakValleyCombo;
	QLabel* countLabel;
	QComboBox* countCombo;
	QLabel* entityTypeLabel;
	QComboBox* entityTypeCombo;
	QLabel* entityNumLabel;
	QTextEdit* entityNumEdit;
	QPushButton*entityNumBtn;

	QLineEdit *inFluenceFactorsEdit;
	QLineEdit *pathEdit;
	QComboBox *algorithmCombo;

};

#endif