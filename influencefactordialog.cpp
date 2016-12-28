#include "influencefactordialog.h"

#include <QLabel>
#include <QTextEdit>
#include <QLineEdit>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QDialog>
#include <QDialogButtonBox>
#include <QPushButton>
#include <QDebug>


InfluenceFactorDialog::InfluenceFactorDialog(QWidget *parent)
:QDialog(parent)
{
	QVBoxLayout *layout = new QVBoxLayout();

		QLabel *NotchLabel = new QLabel("Notch Factor(kf,kt)");
		QLineEdit *NotchEdit = new QLineEdit("1.0");
		QHBoxLayout *Notchlayout = new QHBoxLayout();
		 Notchlayout->addWidget(NotchLabel);
		 Notchlayout->addWidget(NotchEdit);

		 QLabel *SurfaceLabel = new QLabel("Surface Factor(ms)");
		 QLineEdit *SurfaceEdit = new QLineEdit("1.0");
		 QHBoxLayout *Surfacelayout = new QHBoxLayout();
		 Surfacelayout->addWidget(SurfaceLabel);
		 Surfacelayout->addWidget(SurfaceEdit);

		 QLabel *SizeLabel = new QLabel("Size Factor(md)");
		 QLineEdit *SizeEdit = new QLineEdit("1.0");
		 QHBoxLayout *Sizelayout = new QHBoxLayout();
		 Sizelayout->addWidget(SizeLabel);
		 Sizelayout->addWidget(SizeEdit);

		 QLabel *LoadLabel = new QLabel("Load Factor(mt)");
		 QLineEdit *LoadEdit = new QLineEdit("1.0");
		 QHBoxLayout *Loadlayout = new QHBoxLayout();
		 Loadlayout->addWidget(LoadLabel);
		 Loadlayout->addWidget(LoadEdit);

		 QLabel *OtherLabel = new QLabel("Other Factor(mo)");
		 QLineEdit *OtherhEdit = new QLineEdit("1.0");
		 QHBoxLayout *Otherlayout = new QHBoxLayout();
		 Otherlayout->addWidget(OtherLabel);
		 Otherlayout->addWidget(OtherhEdit);

		 layout->addLayout(Notchlayout);
		 layout->addLayout(Surfacelayout);
		 layout->addLayout(Sizelayout);
		 layout->addLayout(Loadlayout);
		 layout->addLayout(Otherlayout);

		 QDialogButtonBox *buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
		 layout->addWidget(buttonBox);
		this->setLayout( layout);

//#signals
	//	qtc.QObject.connect(self._buttonBox, qtc.SIGNAL("accepted()"), self, qtc.SLOT("accept()"))
	//	qtc.QObject.connect(self._buttonBox, qtc.SIGNAL("rejected()"), self, qtc.SLOT("reject()"))
}

InfluenceFactorDialog::~InfluenceFactorDialog()
{

}

