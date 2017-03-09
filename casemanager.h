#ifndef CASEMANAGER_H_
#define CASEMANAGER_H_

#include <QObject>
#include <QStringList>
class  CaseManager : public QObject
{
	Q_OBJECT
public:
	CaseManager(QObject *parent = 0);
	~CaseManager();

	QStringList getCaseName();
private:
	QStringList cases;

};

#endif