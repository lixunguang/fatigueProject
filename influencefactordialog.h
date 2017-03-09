#ifndef InfluenceFactorDialog_H_
#define InfluenceFactorDialog_H_

#include <QDialog>
#include <QLineEdit>


class  InfluenceFactorDialog : public QDialog
{
	Q_OBJECT
public:
	InfluenceFactorDialog(QWidget *parent = 0);
	~InfluenceFactorDialog();

public:
	void updateUi(QString uiText);


public:
	QLineEdit *NotchEdit;
	QLineEdit *SurfaceEdit;
	QLineEdit *SizeEdit;
	QLineEdit *LoadEdit;
	QLineEdit *OtherhEdit;
};

#endif