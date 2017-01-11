
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QDebug>

#include "Python.h"

#include "ribbonwindow.h"
#include "meshviewer.h"
#include "propertyviewer.h"
#include "labelviewer.h"
#include "operationviewer.h"

class MainWindow :public RibbonWindow
{
    Q_OBJECT
public:
	MainWindow(QWidget *parent = 0) ;


	~MainWindow()
	{
	}

private:
	void createAction();
	void createMenuFile();
	void createQuickAccessBar();
	void createRibbon();

	void createGroupTools(Qtitan::RibbonPage* page);
	void createGroupView(Qtitan::RibbonPage* page);
	void createGroupAbout(Qtitan::RibbonPage* aboutPage);

protected Q_SLOTS:
	void openFile();
	void closeFile();

	void openProject();
	void closeProject();

	virtual void options(QAction* action);

	void showFatigueDialog();

	void runPython();

	void help();
	void about();

	void onLabelBrowserStateChanged(int state);
	void onPropertyCheckStateChanged(int state);
	void onFatigueCheckStateChanged(int state);
protected:
	void paintEvent(QPaintEvent * event);

protected:
	QAction* m_actionOpenFile;
	QAction* m_actionCloseFile;

	QAction* m_showFatigueDialog;

	
	QAction* m_actionCopy;
	QAction* m_actionPaste1;
	QAction* m_actionPaste2;
	QAction* m_actionFormatPointerAction;
	QAction* m_actionSelectAll;
	QAction* m_actionFullScreen;
	QAction* m_actionPrintLayout;

private:
	MeshViewer *meshViewer;
	LabelViewer *labelViewer;
	PropertyViewer *propViewer;
	OperationViewer *opViewer;
		
};

#endif