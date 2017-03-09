
#include <QVBoxLayout>
#include <QPushButton>
#include <QDebug>
#include <QTextEdit>

#include "fatiguewidget.h"
#include "analysiswidget.h"
#include "loadWidget.h"

//注意！！！！！！！！！
//把ui数据写入data
//约定: 把objectname非空的写到mapdata里面
//objectname名字加上段名，分段的目的是xml易读


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

	connect(finishBtn, SIGNAL(clicked()), this, SLOT(writeToFile()));

	this->setLayout(layout);

}

FatigueWidget::~FatigueWidget()
{

}



void FatigueWidget::writeToFile()
{
	qDebug() << "FatigueWidget::writeToFile";
}

void FatigueWidget::updateUi(QMap<QString,QString> &mapData)
{   //from Data to UI
	//according to object name
	
	for each (QString key in mapData.keys())
	{
		QStringList keyList = key.split("@");
		QStringList valList = mapData[key].split("@");

		QString val;
		OBJECTTYPE type = type_NONE;

		for (int j = 1; j < keyList.size(); j++)
		{
			if (keyList[j] == "name")
			{
				val = valList[j-1];
			}

			if (keyList[j] == "type")
			{
				if (valList[j-1].toInt() == 1)
				{
					type = type_edit;
				}
				else if (valList[j-1].toInt() == 2)
				{
					type = type_combo;
				}
				else if (valList[j - 1].toInt() == 3)
				{
					type = type_text;
				}
				else
				{
					qDebug() << valList[j].toInt() << "OBJECTTYPE ERROR" << valList;
				}
			}
		}

		if (type == type_edit )
		{
			QLineEdit *obj = this->findChild<QLineEdit *>(val);
			if (!obj)
			{
				qDebug() << "can't find object: " << val;
			}
			obj->setText(valList[valList.size()-1]);
		}
		if ( type == type_text)
		{
			QTextEdit *obj = this->findChild<QTextEdit *>(val);
			if (!obj)
			{
				qDebug() << "can't find object: " << val;
			}
			obj->setText(valList[valList.size() - 1]);
		}
		else if (type == type_combo)
		{
			QComboBox *obj = this->findChild<QComboBox *>(val);
			if (!obj)
			{
				qDebug() << "can't find object: " << val;
			}
			obj->setCurrentIndex(valList[valList.size()-1].toInt());
		}
	}
	
	
}

void FatigueWidget::writeToMap(QMap<QString, QString> &mapData, QString objName, QString val,int type,int count)
{
	
	QStringList list = objName.split("_");
	if (list.size()!=2)
	{
		return;
	}
	QString name = QString("%1@type@name@%2").arg(list[0]).arg(count);
	QString nameVal = QString("%1@%2@%3").arg(type).arg(list[1]).arg(val);

	mapData[name] = nameVal;
}

void FatigueWidget::updateMapData(QMap<QString, QString> &mapData)
{//From UI to Data
	//把ui数据写入data
	//可以约定，把objectname非空的写到mapdata里面
	//objectname名字加上段名，分段的目的是xml易读


	mapData.clear();

	int count = 0;
	QList<QLineEdit *> lineList = this->findChildren<QLineEdit *>();// Qt::FindDirectChildOnly
	for each (QLineEdit *edit  in lineList)
	{

		QString objName = edit->objectName();
		if (!objName.isEmpty())
		{
			writeToMap(mapData, objName, edit->text(),1,count++);
		}
	}

	QList<QComboBox *> comboList = this->findChildren<QComboBox *>();
	for each (QComboBox *combo  in comboList)
	{
		QString objName = combo->objectName();
		if (!objName.isEmpty())
		{
			writeToMap(mapData, objName, QString(combo->currentIndex()),2,count++);
		}
	}


}

