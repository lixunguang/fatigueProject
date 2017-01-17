
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QDebug>
#include <QDomDocument>

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
	void importFile();
	void save();
	void saveAs();


	void closeProject();

	virtual void options(QAction* action);

	void showFatigueDialog();

	void runPython();

	void help();
	void about();

	void onLabelBrowserStateChanged(int state);
	void onPropertyCheckStateChanged(int state);
	void onFatigueCheckStateChanged(int state);

	public slots:
	// project manager
	void openProject();
	void parseProjectFile(QString &filePath);
	void readProjectConfig(QDomElement& e);
	void readInputConfig(QDomElement& e);
	void readOutputConfig(QDomElement& e);
	void travelElement(QDomElement element, QMap<QString, QString> &map);
	void writeProject();
	void domElemFromProjectConfig(QDomElement& e, QDomDocument&doc);
	void domElemFromInputConfig(QDomElement& e, QDomDocument&doc);
	void domElemFromOutputConfig(QDomElement& e, QDomDocument&doc);

protected:
	void paintEvent(QPaintEvent * event);

protected:
	QAction* m_actionOpenFile;
	QAction* m_actionImportFile;
	QAction* m_actionSave;
	QAction* m_actionSaveAs;
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
		
	//存放xml格式的工程文件数据
	//key name：sectionName_name_type_val，名字中含有xml的数据结构，这也是把树形的xml转为map的关键
	//      val:
	QMap<QString, QString> projectConfigMapData;
	QMap<QString, QString> inputConfigMapData;
	QMap<QString, QString> outputConfigMapData;
};

#endif