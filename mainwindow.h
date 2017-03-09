
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QDebug>

#include "Python.h"

#include "ribbonwindow.h"
#include "meshviewer.h"
#include "propertyviewer.h"
#include "labelviewer.h"
#include "operationviewer.h"
#include "projectManager.h"
#include "fatiguewidget.h"
#include "qwidget.h"
#include "QApplication.h"


class MainWindow :public RibbonWindow
{
    Q_OBJECT
public:
	MainWindow(QWidget *parent = 0) ;
	~MainWindow(){}

private:
	void createAction();
	void createMenuFile();
	void createQuickAccessBar();
	void createRibbon();

	void createGroupTools(Qtitan::RibbonPage* page);
	void createGroupView(Qtitan::RibbonPage* page);
	void createGroupAbout(Qtitan::RibbonPage* aboutPage);
public:
	static MainWindow* getMainWindow()
	{
		foreach(QWidget *widget, QApplication::allWidgets())
		if (widget->objectName().compare("fatigueMainwindow") == 0)
		{
			return (MainWindow*) widget;
		}
		return NULL;
	}
public:
	MeshViewer *getMeshViewer()
	{
		return meshViewer;
	}
	LabelViewer *getLabelViewer()
	{
		return labelViewer;
	}
	PropertyViewer *getPropViewer()
	{
		return propViewer;
	}
	OperationViewer *getOpViewer()
	{
		return opViewer;
	}

	ProjectManager *getProjectManager()
	{
		return &projectManager;
	}
protected Q_SLOTS:
	void newProject();
	void openProject();
	QString importFile();
	void save();
	void saveAs();
	void closeProject();
	void reset();//重置应用程序


	virtual void options(QAction* action);

	void showFatigueDialog();

	void runPython();
	
	void onLabelBrowserStateChanged(int state);
	void onPropertyCheckStateChanged(int state);
	void onFatigueCheckStateChanged(int state);

	void writeToMap(QMap<QString, QString> &mapData, QString objName, QString val, int type, int count);
	void updateUi(QMap<QString, QString> &mapData);
	void updateMapData(QMap<QString, QString> &mapData);

	void help();
	void about();
protected:
	QAction* m_actionNewProject;
	QAction* m_actionOpenProject;
	QAction* m_actionImportFile;
	QAction* m_actionSave;
	QAction* m_actionSaveAs;
	QAction* m_actionCloseFile;
	QAction *m_actionReset;
	QAction* m_showFatigueDialog;

	
	QAction* m_actionCopy;
	QAction* m_actionPaste1;
	QAction* m_actionPaste2;
	QAction* m_actionFormatPointerAction;
	QAction* m_actionSelectAll;
	QAction* m_actionFullScreen;
	QAction* m_actionPrintLayout;

public:
	MeshViewer *meshViewer;
	LabelViewer *labelViewer;
	PropertyViewer *propViewer;
	OperationViewer *opViewer;
	
	ProjectManager projectManager;
 
};

#endif