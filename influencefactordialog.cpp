
#include <QLabel>
#include <QLineEdit>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QDialogButtonBox>
#include <QPushButton>
#include <QDebug>

#include "influencefactordialog.h"

InfluenceFactorDialog::InfluenceFactorDialog(QWidget *parent)
:QDialog(parent)
{
	QVBoxLayout *layout = new QVBoxLayout();

		QLabel *NotchLabel = new QLabel("Notch Factor(kf,kt)");
		NotchEdit = new QLineEdit("1.0");
		QHBoxLayout *Notchlayout = new QHBoxLayout();
		 Notchlayout->addWidget(NotchLabel);
		 Notchlayout->addWidget(NotchEdit);

		 QLabel *SurfaceLabel = new QLabel("Surface Factor(ms)");
		 SurfaceEdit = new QLineEdit("1.0");
		 QHBoxLayout *Surfacelayout = new QHBoxLayout();
		 Surfacelayout->addWidget(SurfaceLabel);
		 Surfacelayout->addWidget(SurfaceEdit);

		 QLabel *SizeLabel = new QLabel("Size Factor(md)");
		 SizeEdit = new QLineEdit("1.0");
		 QHBoxLayout *Sizelayout = new QHBoxLayout();
		 Sizelayout->addWidget(SizeLabel);
		 Sizelayout->addWidget(SizeEdit);

		 QLabel *LoadLabel = new QLabel("Load Factor(mt)");
		 LoadEdit = new QLineEdit("1.0");
		 QHBoxLayout *Loadlayout = new QHBoxLayout();
		 Loadlayout->addWidget(LoadLabel);
		 Loadlayout->addWidget(LoadEdit);

		 QLabel *OtherLabel = new QLabel("Other Factor(mo)");
		 OtherhEdit = new QLineEdit("1.0");
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
		connect(buttonBox, SIGNAL(accepted()), this, SLOT(accept()));
		connect(buttonBox, SIGNAL(rejected()), this, SLOT(reject()));
}

InfluenceFactorDialog::~InfluenceFactorDialog()
{

}

void InfluenceFactorDialog::updateUi(QString uiText)
{
	QStringList li = uiText.split(',');
	if (li.size()==5)
	{
		NotchEdit->setText(li[0]);
		SurfaceEdit->setText(li[1]);
		SizeEdit->setText(li[2]);
		LoadEdit->setText(li[3]);
		OtherhEdit->setText(li[4]);
	}

}