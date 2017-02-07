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
 
	//���xml��ʽ�Ĺ����ļ�����,�ڲ�����
	//key name��sectionName_name_type_val�������к���xml�����ݽṹ����Ҳ�ǰ����ε�xmlתΪmap�Ĺؼ�
	//      val:
	QMap<QString, QString> projectConfigMapData;//����������Ϣ
	QMap<QString, QString> modelConfigMapData;//ģ��������Ϣ
	QMap<QString, QString> outputConfigMapData;//���������Ϣ

	QDomDocument doc;
	QDomElement root;
	QString projectFileName;
	QFile projectFile;
};

#endif