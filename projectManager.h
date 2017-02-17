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
	ProjectManager(QObject *parent);
	~ProjectManager();

public slots:
	// project manager
	void parse(QString &filePath);

    void setProjectFileName(QString fileName);
	void setModelFileName(QStringList &fileNames);
	QString getProjectFileName();
	QStringList getModelFileName();

    void save();
	void reset();

	void addNodeLabelToModelConfig(QString& key,QString& val);
	void addElemLabelToModelConfig(QString& key, QString& val);
	void addToOutputConfig();

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
 	//���xml��ʽ�Ĺ����ļ�����,�ڲ�����
	//key name��sectionName_name_type_val�������к���xml�����ݽṹ����Ҳ�ǰ����ε�xmlתΪmap�Ĺؼ�
	//      val:
	QMap<QString, QString> projectConfigMapData;//����������Ϣ
	QMap<QString, QString> modelConfigMapData;//ģ��������Ϣ
	QMap<QString, QString> outputConfigMapData;//���������Ϣ

	QDomDocument doc;
	
	QString projectFileName;
	QFile projectFile;


	int uniqueID;//�ڲ�����ʹ�õ�Ψһid
};

#endif