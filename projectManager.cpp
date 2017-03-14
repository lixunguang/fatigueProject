#include "qapplication.h"
#include "mainwindow.h"

#include "projectManager.h"




ProjectManager::ProjectManager(QObject *parent)
:doc("fatigueProject"), QObject(parent)
{
	QDomProcessingInstruction instruction;
	instruction = doc.createProcessingInstruction("xml", "version=\"1.0\" encoding=\"UTF-8\"");
	doc.appendChild(instruction);

	uniqueID = 0;
}

ProjectManager::~ProjectManager()
{
	projectFile.close();
}

#pragma region project func
 
void ProjectManager::parse(QString &fileName)
{//和下面的domelemFrom...对应

	QFile file(fileName);
	if (!file.open(QFile::ReadOnly | QFile::Text))
	{
		qDebug()<<(tr("Cannot read file %1:\n%2.").arg(fileName).arg(file.errorString()));
		return;
	}

	if (!doc.setContent(&file))
	{
		qDebug() << (QString("read %1 error").arg(fileName));
		file.close();
		return;
	}
	file.close();

	QDomElement docElem = doc.documentElement();

	QDomNode node = docElem.firstChild();
	while (!node.isNull())
	{
		QDomElement e = node.toElement();
		if (!e.isNull())
		{
			if (e.tagName() == "ProjectConfig")
			{
				projectConfigFromDomElem(e);
			}

			if (e.tagName() == "ModelConfig")
			{
				modelConfigFromDomElem(e);
			}

			if (e.tagName() == "OutputConfig")
			{
				outputConfigFromDomElem(e);
			}
		}
		node = node.nextSibling();
	}

}

void ProjectManager::setProjectFileName(QString fileName)
{
	MainWindow *mw = MainWindow::getMainWindow();
	mw->setWindowTitle(qApp->applicationName() + " | " + fileName);

	projectFileName = fileName;
}

QString ProjectManager::getProjectFileName()
{
	return projectFileName;
}

QStringList ProjectManager::getModelFileName()
{//"File@0",d:\model\Crane_Support.fil
	QStringList modelFileName;

	for each (QString itemKey in modelConfigMapData.keys())
	{
		QStringList valList = modelConfigMapData[itemKey].split("@");

		if (itemKey.startsWith("File"))
		{
			modelFileName << valList[0];
		}
	}

	return modelFileName;
}


void ProjectManager::travelElement(QDomElement element, QMap<QString, QString> &map)
{//tree--->map

	if (!element.isNull())
	{
		QDomNodeList childNodes = element.childNodes();
		QDomNode node;
		QDomElement elem;
		QString keyStr;
		QString attrNamestr;
		QString attrValstr;

		for (int i = 0; i < childNodes.size(); i++)
		{
			elem = childNodes.at(i).toElement();

			if (elem.tagName() == "Item")
			{
				QDomNamedNodeMap attrMap = elem.attributes();
				QDomAttr attr;
				attrNamestr = "";
				keyStr = "";
				attrValstr = "";
				for (int j = 0; j < attrMap.count(); j++)//读取多个attr
				{
					attr = attrMap.item(j).toAttr();
					attrNamestr += attr.name();
					attrNamestr += "@";

					attrValstr += attr.value();
					attrValstr += "@";
				}
				keyStr = QString("%1@%2%3").arg(element.tagName()).arg(attrNamestr).arg(uniqueID++);

				map[keyStr] = attrValstr + elem.text().trimmed();//trimmed: remove space

			}
			travelElement(elem, map);
		}
	}
}


void ProjectManager::addNodeLabelToModelConfig(QString& labelName, QString& val)
{
	QString keyStr = QString("%1@%2@%3").arg("NodeLabel").arg("name").arg(uniqueID++);
	modelConfigMapData[keyStr] = labelName + "@" + val;
}

void ProjectManager::addElemLabelToModelConfig(QString& labelName, QString& val)
{
	QString keyStr = QString("%1@%2@%3").arg("ElementLabel").arg("name").arg(uniqueID++);
	modelConfigMapData[keyStr] = labelName + "@" + val;
}

void ProjectManager::addToOutputConfig()
{//not used yet
	
}

void ProjectManager::projectConfigFromDomElem(QDomElement& e)
{
	travelElement(e, projectConfigMapData);
}

void ProjectManager::modelConfigFromDomElem(QDomElement& e)
{
	travelElement(e, modelConfigMapData);

}

void ProjectManager::outputConfigFromDomElem(QDomElement& e)
{
	travelElement(e, outputConfigMapData);
}

void ProjectManager::xmlElemFromProjectConfig(QDomElement& projectConfigElem)
{

	QSet<QString> keys;
	for each (QString itemKey in projectConfigMapData.keys())
	{
		QStringList list = itemKey.split("@");
		keys.insert(list[0]);
	}

	for each (QString key in keys)
	{
		QDomElement elem = doc.createElement(key);
		projectConfigElem.appendChild(elem);
	}

	QDomElement sElem = projectConfigElem.elementsByTagName("ShowView").at(0).toElement();

	for each (QString itemKey in projectConfigMapData.keys())
	{
		QDomElement elem = doc.createElement("Item");

		QStringList valList = projectConfigMapData[itemKey].split("@");
		QDomText t = doc.createTextNode(valList[valList.size() - 1]);
		elem.appendChild(t);

		QStringList keyList = itemKey.split("@");

		for (int i = 1; i < keyList.size() - 1; i++)
		{
			elem.setAttribute(keyList[i], valList[i - 1]);
		}

		QStringList list = itemKey.split("@");

		if (itemKey.startsWith("ShowView"))
		{
			sElem.appendChild(elem);
		}
	}

}

void ProjectManager::xmlElemFromModelConfig(QDomElement& modelConfigElem)
{
	
	//构建section 部分 ex. ModelConfig File NodeLabel ElementLabel,etc
	QSet<QString> keys;
	for each (QString itemKey in modelConfigMapData.keys())
	{
		QStringList list = itemKey.split("@");
		keys.insert(list[0]);
	}

	for each (QString key in keys)
	{
		QDomElement elem = doc.createElement(key);
		modelConfigElem.appendChild(elem);
	}

	QDomElement fElem = modelConfigElem.elementsByTagName("File").at(0).toElement();
	QDomElement nElem = modelConfigElem.elementsByTagName("NodeLabel").at(0).toElement();
	QDomElement eElem = modelConfigElem.elementsByTagName("ElementLabel").at(0).toElement();

	for each (QString itemKey in modelConfigMapData.keys())
	{
		QDomElement elem = doc.createElement("Item");

		QStringList valList = modelConfigMapData[itemKey].split("@");
		QDomText t = doc.createTextNode(valList[valList.size() - 1]);
		elem.appendChild(t);

		QStringList keyList = itemKey.split("@");

		for (int i = 1; i < keyList.size() - 1; i++)
		{
			elem.setAttribute(keyList[i], valList[i - 1]);
		}

		if (itemKey.startsWith("File"))
		{
			fElem.appendChild(elem);
		}
		else if (itemKey.startsWith("NodeLabel"))
		{
			nElem.appendChild(elem);
		}
		else if (itemKey.startsWith("ElementLabel"))
		{
			eElem.appendChild(elem);
		}
	}

}

void ProjectManager::xmlElemFromOutputConfig(QDomElement& outputConfigElem)
{
	
	QSet<QString> keys;
	for each (QString itemKey in outputConfigMapData.keys())
	{
		QStringList list = itemKey.split("@");
		keys.insert(list[0]);
	}

	for each (QString key in keys)
	{
		QDomElement elem = doc.createElement(key);
		outputConfigElem.appendChild(elem);
	}

	QDomElement fElem = outputConfigElem.elementsByTagName("File").at(0).toElement();
	QDomElement algorithmElem = outputConfigElem.elementsByTagName("AlgorithmSection").at(0).toElement();
	QDomElement analisisElem = outputConfigElem.elementsByTagName("AnalisisDefSection").at(0).toElement();
	QDomElement rainFlowElem = outputConfigElem.elementsByTagName("RainFlowSection").at(0).toElement();
	QDomElement entityElem = outputConfigElem.elementsByTagName("EntitySection").at(0).toElement();
	QDomElement caseElem = outputConfigElem.elementsByTagName("CaseSection").at(0).toElement();
	QDomElement loadElem = outputConfigElem.elementsByTagName("LoadSection").at(0).toElement();

	//itemKey: 
	for each (QString itemKey in outputConfigMapData.keys())
	{
		QDomElement elem = doc.createElement("Item");

		QStringList valList = outputConfigMapData[itemKey].split("@");
		QDomText t = doc.createTextNode(valList[valList.size() - 1]);
		elem.appendChild(t);

		QStringList keyList = itemKey.split("@");

		for (int i = 1; i < keyList.size() - 1; i++)
		{
			elem.setAttribute(keyList[i], valList[i - 1]);
		}

		/////////////////////////////////////////////

		if (itemKey.startsWith("File"))
		{
			fElem.appendChild(elem);
		}
		else if (itemKey.startsWith("AlgorithmSection"))
		{
			algorithmElem.appendChild(elem);
		}
		else if (itemKey.startsWith("AnalisisDefSection"))
		{
			analisisElem.appendChild(elem);
		}
		else if (itemKey.startsWith("RainFlowSection"))
		{
			rainFlowElem.appendChild(elem);
		}
		else if (itemKey.startsWith("EntitySection"))
		{
			entityElem.appendChild(elem);
		}
		else if (itemKey.startsWith("CaseSection"))
		{
			caseElem.appendChild(elem);
		}
		else if (itemKey.startsWith("LoadSection"))
		{
			loadElem.appendChild(elem);
		}
	}
}

void ProjectManager::setModelFileName(QStringList &fileNames)
{
	int count = 0;
	for each (QString fn in fileNames)
	{
		QString name = QString("File@%1").arg(count++);
		QString val = QString("%1").arg(fn);

		modelConfigMapData[name] = val;
	}
	
}

void ProjectManager::reset()
{
	projectFile.close();
	projectFileName = "";
	projectConfigMapData.clear();
	modelConfigMapData.clear();
	outputConfigMapData.clear();

}

void ProjectManager::save()
{//save all project config
	projectFile.close();
	projectFile.setFileName(projectFileName);
	if (!projectFile.open(QIODevice::Truncate|QIODevice::ReadWrite | QIODevice::Text))
	{
		qDebug() << (tr("Failed to open file for writing."));
		return;
	}
	QDomElement projectElem = doc.elementsByTagName("Project").at(0).toElement();
	if (projectElem.isNull())
	{
		projectElem = doc.createElement("Project");
		projectElem.setAttribute("Version", "1.0");
		doc.appendChild(projectElem);
	}
	else
	{
		QDomNode oldProjectConfigElem = projectElem.elementsByTagName("ProjectConfig").at(0);
		projectElem.removeChild(oldProjectConfigElem);

		QDomNode oldModelConfigElem = projectElem.elementsByTagName("ModelConfig").at(0);
		projectElem.removeChild(oldModelConfigElem);

		QDomNode oldOutputConfigElem = projectElem.elementsByTagName("OutputConfig").at(0);
		projectElem.removeChild(oldOutputConfigElem);
	}

	QDomElement projectConfigElem = doc.createElement("ProjectConfig");
	xmlElemFromProjectConfig(projectConfigElem);
	QDomNode node = projectElem.appendChild(projectConfigElem);

	QDomElement modelConfigElem = doc.createElement("ModelConfig");
	xmlElemFromModelConfig(modelConfigElem);
	node = projectElem.appendChild(modelConfigElem);

	QDomElement outputConfigElem = doc.createElement("OutputConfig");
	xmlElemFromOutputConfig(outputConfigElem);
	node = projectElem.appendChild(outputConfigElem);

	QTextStream stream(&projectFile);
	stream << doc.toString();
	projectFile.close();
}

void ProjectManager::saveAsSolveFile(QString &filePath)
{//保存为求解使用的文件
	qDebug() << "ProjectManager::saveAsSolveFile";
}

QMap<QString,QString>& ProjectManager::getSolverData()
{
	return outputConfigMapData;
}

QMap<QString, QString>& ProjectManager::getProjectData()
{
	return projectConfigMapData;
}

QMap<QString, QString>& ProjectManager::getModelData()
{
	return modelConfigMapData;
}

#pragma endregion project func