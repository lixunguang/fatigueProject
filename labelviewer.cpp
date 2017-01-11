
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QPushButton>
#include <QInputDialog>
#include <QMessageBox>
#include <QDebug>

#include "labelviewer.h"

LabelViewer::LabelViewer(QWidget *parent)
:QDockWidget(parent)
{
	QVBoxLayout *vLayout = new QVBoxLayout();
	QWidget *labelBrowser = new QWidget();
	labelBrowser->setLayout(vLayout);

	QTreeWidget *tree = new QTreeWidget();
	vLayout->addWidget(tree);

	QPushButton *addBtn = new QPushButton();
	addBtn->setIcon(QIcon(QPixmap(":/res/label/add.png")));	
	connect(addBtn, SIGNAL(clicked()), this, SLOT(onAddBtn()));

	QPushButton *removeBtn = new QPushButton(); 
	removeBtn->setIcon(QIcon(QPixmap(":/res/label/remove.png")));
	connect(removeBtn, SIGNAL(clicked()), this, SLOT(onRemoveBtn()));

	QHBoxLayout *hLayout = new QHBoxLayout();
	hLayout->addWidget(addBtn);
	hLayout->addWidget(removeBtn);

	vLayout->addLayout(hLayout);

	this->setWidget(labelBrowser);

	this->setFeatures(QDockWidget::NoDockWidgetFeatures);
}

LabelViewer::~LabelViewer()
{

}

void LabelViewer::onAddBtn()
{
	bool ok;
	QString text = QInputDialog::getText(this, tr("input Label name"),	
		tr("Label name:"), QLineEdit::Normal,
		0, &ok);

	if (ok && !text.isEmpty())
		;//textLabel->setText(text);
}

void LabelViewer::onRemoveBtn()
{
	QString waringInfo = "remove label";
	QMessageBox::warning(this, "remove label", "remove ###", QMessageBox::Ok | QMessageBox::Cancel);
}