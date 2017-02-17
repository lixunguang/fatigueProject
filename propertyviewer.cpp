

#include <QVBoxLayout>
#include <QFrame>
#include <QDebug>

#include "propertyviewer.h"

PropertyViewer::PropertyViewer(QWidget *parent)
	:QDockWidget(parent)
{
	this->setFeatures(QDockWidget::NoDockWidgetFeatures);

	QFrame *dockWidget = new QFrame();
	this->setWidget(dockWidget);

	QVBoxLayout *vLayout = new QVBoxLayout();
	dockWidget->setLayout(vLayout);


	nodeEdit = new QTextEdit();
 
	vLayout->addWidget(nodeEdit);
 
}

PropertyViewer::~PropertyViewer()
{

}

//Ƕ��webkit��
void PropertyViewer::setData(QString& strData)
{//�������һ���ַ���

	nodeEdit->append(strData);
	qDebug() << strData;
}

void PropertyViewer::setData(QMap<QString, QString> &mapData)
{//�������һ���ֵ�
	qDebug() << mapData;
}

void PropertyViewer::reset()
{
	nodeEdit->clear();
}