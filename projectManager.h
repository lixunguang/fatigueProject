#ifndef PROJECTMANAGER_H_
#define PROJECTMANAGER_H_

#include <QObject>
#include <QString>
#include <QDomDocument>
#include <QFile>
#include <QSet>
#include <QMap>
#include <QTextStream>
#include <QDebug>

class  ProjectManager :public QObject
{
	Q_OBJECT
public:
	ProjectManager();
	~ProjectManager();

public slots:
	// project manager
    void setProjectFileName(QString fileName);
	void setModelFileName(QStringList &fileNames);
	QString getProjectFileName();

	void parse(QString &filePath);
    void save();
	void reset();

	void travelElement(QDomElement element, QMap<QString, QString> &map);
	void projectConfigFromDomElem(QDomElement& e);
	void modelConfigFromDomElem(QDomElement& e);
	void outputConfigFromDomElem(QDomElement& e);

	void xmlElemFromProjectConfig(QDomElement &e);
	void xmlElemFromModelConfig(QDomElement &e);
	void xmlElemFromOutputConfig(QDomElement &e);

	QMap<QString, QString>& getUiData();
	QMap<QString, QString>& getProjectData();
	QMap<QString, QString>& getModelData();
    private:
 
	//存放xml格式的工程文件数据,内部数据
	//key name：sectionName_name_type_val，名字中含有xml的数据结构，这也是把树形的xml转为map的关键
	//      val:
	QMap<QString, QString> projectConfigMapData;//工程配置信息
	QMap<QString, QString> modelConfigMapData;//模型配置信息
	QMap<QString, QString> outputConfigMapData;//求解配置信息

	QDomDocument doc;
	QDomElement root;
	QString projectFileName;
	QFile projectFile;
};

#endif