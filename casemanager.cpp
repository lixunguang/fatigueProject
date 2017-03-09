#include "casemanager.h"


CaseManager::CaseManager(QObject *parent)
	:QObject(parent)
{

	cases << "case1" << "case2" << "case3" << "case4";
}

CaseManager::~CaseManager()
{

}

QStringList CaseManager::getCaseName()
{
	return cases;
}
