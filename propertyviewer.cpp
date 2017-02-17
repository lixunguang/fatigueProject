

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

//嵌入webkit？
void PropertyViewer::setData(QString& strData)
{//比如接受一个字符串

	nodeEdit->append(strData);
	qDebug() << strData;
}

void PropertyViewer::setData(QMap<QString, QString> &mapData)
{//比如接受一个字典
	qDebug() << mapData;
}

void PropertyViewer::reset()
{
	nodeEdit->clear();
}