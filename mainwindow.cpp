#include <QApplication>
#include <QFileDialog>
#include <QCheckBox>
#include <QMessageBox>
#include <QDesktopServices>
#include <QSet>

#include "stdio.h"

#include "mainwindow.h"
#include "about.h"

int numargs;

MainWindow::MainWindow(QWidget *parent) :RibbonWindow(parent)
{
	createAction();
	createMenuFile();

	meshViewer = new MeshViewer(this);
	setCentralWidget(meshViewer);

	createRibbon();

	connect(m_showFatigueDialog, SIGNAL(triggered()), this, SLOT(showFatigueDialog()));

	ribbonBar()->setFrameThemeEnabled();

	labelViewer = new LabelViewer(this);
	propViewer = new PropertyViewer(this);
	opViewer = new OperationViewer(this);

	labelViewer->setWindowTitle("Label Browser");
	labelViewer->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
	labelViewer->setMinimumWidth(80);

	propViewer->setWindowTitle("Properties");
	propViewer->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
	propViewer->setMinimumWidth(80);
	propViewer->setMaximumHeight(150);

	opViewer->setWindowTitle("operation");
	opViewer->setAllowedAreas(Qt::RightDockWidgetArea);
	opViewer->setMinimumWidth(200);

	this->addDockWidget(Qt::LeftDockWidgetArea, labelViewer);
	this->addDockWidget(Qt::LeftDockWidgetArea, propViewer);
	this->addDockWidget(Qt::RightDockWidgetArea, opViewer);

	this->setOptions(OfficeStyle::Windows7Scenic);

	this->setMinimumSize(1380, 768);

}

void MainWindow::createAction()
{
	QIcon iconOpen;
	iconOpen.addPixmap(QPixmap(":/res/mainwin_mainwin_largeOpenFile.png"));
	iconOpen.addPixmap(QPixmap(":/res/mainwin_smallOpen.png"));
	m_actionOpenFile = new QAction(iconOpen, tr("&Open..."), this);
	m_actionOpenFile->setShortcut(QKeySequence::Open);
	m_actionOpenFile->setToolTip(tr("Open"));
	m_actionOpenFile->setStatusTip(tr("Open an existing document"));
	connect(m_actionOpenFile, SIGNAL(triggered()), this, SLOT(openProject()));

	QIcon iconImport;
	iconImport.addPixmap(QPixmap(":/res/mainwin_mainwin_largeOpenFile.png"));
	iconImport.addPixmap(QPixmap(":/res/mainwin_smallOpen.png"));
	m_actionImportFile = new QAction(iconImport, tr("&Import..."), this);
	m_actionImportFile->setToolTip(tr("Open"));
	m_actionImportFile->setStatusTip(tr("Open an existing document"));
	connect(m_actionImportFile, SIGNAL(triggered()), this, SLOT(importFile()));

	QIcon iconSave;
	iconSave.addPixmap(QPixmap(":/res/mainwin_mainwin_largeOpenFile.png"));
	iconSave.addPixmap(QPixmap(":/res/mainwin_smallOpen.png"));
	m_actionSave = new QAction(iconSave, tr("&Save"), this);
	m_actionSave->setToolTip(tr("Open"));
	m_actionSave->setStatusTip(tr("Open an existing document"));
	connect(m_actionSave, SIGNAL(triggered()), this, SLOT(save()));

	QIcon iconSaveAs;
	iconSaveAs.addPixmap(QPixmap(":/res/mainwin_mainwin_largeOpenFile.png"));
	iconSaveAs.addPixmap(QPixmap(":/res/mainwin_smallOpen.png"));
	m_actionSaveAs = new QAction(iconSaveAs, tr("&Save As..."), this);
	m_actionSaveAs->setToolTip(tr("Open"));
	m_actionSaveAs->setStatusTip(tr("Open an existing document"));
	connect(m_actionSaveAs, SIGNAL(triggered()), this, SLOT(writeProject()));

	QIcon iconClose;
	iconClose.addPixmap(QPixmap(":/res/mainwin_largeOpenFile.png"));
	iconClose.addPixmap(QPixmap(":/res/mainwin_smallOpen.png"));
	m_actionCloseFile = new QAction(iconClose, tr("&Close"), this);
	m_actionCloseFile->setShortcut(QKeySequence::Save);
	m_actionCloseFile->setToolTip(tr("Save"));
	m_actionCloseFile->setStatusTip(tr("Save the active document"));
	connect(m_actionCloseFile, SIGNAL(triggered()), this, SLOT(closeProject()));

}

void MainWindow::openProject()
{
	propViewer->setData(QString("openProject"));

	QString fileName = QFileDialog::getOpenFileName(this, tr("Open Project File"),
		QDir::currentPath(),
		tr("fatigue project Files (*.fproj)"));

	if (fileName.isEmpty())
		return;

	//1 parase xml ,get model file path

	parseProjectFile(fileName);

	//2 get the label that has been added


	//3 获取求解文件的设置，

}

void MainWindow::parseProjectFile(QString &fileName)
{//和下面的domelemFrom...对应
	propViewer->setData(QString("parseProjectFile"));

	QDomDocument doc("fatigueProject");

	QFile file(fileName);
	if (!file.open(QFile::ReadOnly | QFile::Text))
	{
		propViewer->setData(tr("Cannot read file %1:\n%2.").arg(fileName).arg(file.errorString()));
		return;
	}

	if (!doc.setContent(&file))
	{
		propViewer->setData(QString("read %1 error").arg(fileName));
		file.close();
		return;
	}
	file.close();

	QDomElement docElem = doc.documentElement();

	QDomNode n = docElem.firstChild();
	while (!n.isNull())
	{
		QDomElement e = n.toElement(); 
		if (!e.isNull())
		{
			if (e.tagName() == "ProjectConfig")
			{
				readProjectConfig(e);
			}

			if (e.tagName() == "InputConfig")
			{
				readInputConfig(e);
			}

			if (e.tagName() == "OutputConfig")
			{
				readOutputConfig(e);
			}

		}
		n = n.nextSibling();
	}

}

void MainWindow::travelElement(QDomElement element, QMap<QString, QString> &map)
{//tree--->map

	if (!element.isNull())
	{
		QDomNodeList childNodes = element.childNodes();
		QDomNode node;
		QDomElement elem;
		QString keyStr;
		QString attrNamestr;
		QString attrValstr;

		int count = 0;
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
				keyStr = QString("%1@%2%3").arg(element.tagName()).arg(attrNamestr).arg(count++);
				
				map[keyStr] = attrValstr + elem.text().trimmed();//trimmed: remove space

			}
			travelElement(elem, map);
		}
	}
}

void MainWindow::readProjectConfig(QDomElement& e)
{
	travelElement(e, projectConfigMapData);
}

void MainWindow::readInputConfig(QDomElement& e)
{
	travelElement(e, inputConfigMapData);
}

void MainWindow::readOutputConfig(QDomElement& e)
{
	travelElement(e, outputConfigMapData);
}


void MainWindow::domElemFromProjectConfig(QDomElement& projectConfigElem, QDomDocument& doc)
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

			elem.appendChild(elem);
		}
	}

}

void MainWindow::domElemFromInputConfig(QDomElement& inputConfigElem, QDomDocument& doc)
{
	//构建section 部分 ex. InputConfig File NodeLabel ElementLabel,etc
	QSet<QString> keys;
	for each (QString itemKey in inputConfigMapData.keys())
	{
		QStringList list = itemKey.split("@");
		keys.insert(list[0]);
	}

	for each (QString key in keys)
	{
		QDomElement elem = doc.createElement(key);
		inputConfigElem.appendChild(elem);
	}
 
	QDomElement fElem = inputConfigElem.elementsByTagName("File").at(0).toElement();
	QDomElement nElem = inputConfigElem.elementsByTagName("NodeLabel").at(0).toElement();
	QDomElement eElem = inputConfigElem.elementsByTagName("ElementLabel").at(0).toElement();

 	for each (QString itemKey in inputConfigMapData.keys())
	{
		QDomElement elem = doc.createElement("Item");

		QStringList valList = inputConfigMapData[itemKey].split("@");
		QDomText t = doc.createTextNode(valList[valList.size()-1]);
		elem.appendChild(t);
	 

		QStringList keyList = itemKey.split("@");
		
		qDebug() << valList;
	
		for (int i = 1; i < keyList.size()-1;i++)
		{
			elem.setAttribute(keyList[i], valList[i-1]);

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

void MainWindow::domElemFromOutputConfig(QDomElement& outputConfigElem, QDomDocument& doc)
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

void MainWindow::writeProject()
{
	//1 open file
	QString fileName = QFileDialog::getSaveFileName(this, tr("Save Project File"),
		QDir::currentPath(),
		tr("fatigue project Files (*.fproj)"));

	if (fileName.isEmpty())
		return;

	QFile outFile(fileName);
	if (!outFile.open(QIODevice::WriteOnly | QIODevice::Text))
	{
		propViewer->setData(tr("Failed to open file for writing."));
		return ;
	}

	//2 write to file
	QDomDocument doc("fatigueProject");
	QDomProcessingInstruction instruction;
	instruction = doc.createProcessingInstruction("xml", "version=\"1.0\" encoding=\"UTF-8\"");
	doc.appendChild(instruction);

	QDomElement root = doc.createElement("Project");
	root.setAttribute("Version", "1.0");
	doc.appendChild(root);

	QDomElement projectConfigElem = doc.createElement("projectConfig");
	domElemFromProjectConfig(projectConfigElem,doc);

	QDomElement inputConfigElem = doc.createElement("inputConfig");
	domElemFromInputConfig(inputConfigElem,doc);

	QDomElement outputConfigElem = doc.createElement("OutputConfig");
	domElemFromOutputConfig(outputConfigElem,doc);
	
	root.appendChild(projectConfigElem);
	root.appendChild(inputConfigElem);
	root.appendChild(outputConfigElem);

	QTextStream stream(&outFile);
	stream << doc.toString();

	outFile.close();
}

void MainWindow::importFile()
{
	QString fileName = QFileDialog::getOpenFileName(this, tr("Open File..."),
		QString(), tr("Ansys result (*.fil *.odb);;All Files (*)"));

	if (fileName.isEmpty())
	{
		return;
	}
	// read mesh
	meshViewer->loadMeshData(fileName.toLatin1().data());
}

void MainWindow::save()
{

}

void MainWindow::saveAs()
{

}

void MainWindow::closeProject()
{
	QString fn = QFileDialog::getOpenFileName(this, tr("Open File..."),
		QString(), tr("HTML-Files (*.htm *.html);;All Files (*)"));

}

void MainWindow::showFatigueDialog()
{
	runPython();

}

void MainWindow::options(QAction* action)
{
	RibbonWindow::options(action);
}

void MainWindow::createMenuFile()
{
	QIcon iconLogo;
	iconLogo.addPixmap(QPixmap(":/res/mainwin_file.png"));
	iconLogo.addPixmap(QPixmap(":/res/mainwin_qtitanlogo32x32.png"));
	if (QAction* actionFile = ribbonBar()->addSystemButton(iconLogo, tr("&File")))
	{
		actionFile->setToolTip(tr("Click here to see everything you can do with your document"));
		Qtitan::RibbonSystemPopupBar* popupBar = qobject_cast<Qtitan::RibbonSystemPopupBar*>(actionFile->menu());

		popupBar->addAction(m_actionOpenFile);
		popupBar->addSeparator();
		popupBar->addAction(m_actionImportFile);
		popupBar->addAction(m_actionSave);
		popupBar->addAction(m_actionSaveAs);
		popupBar->addSeparator();
		popupBar->addAction(m_actionCloseFile);
	}
}

void MainWindow::createRibbon()
{
	if (Qtitan::RibbonPage* pageView = ribbonBar()->addPage(tr("&View")))
	{
		createGroupView(pageView);
	}

	if (Qtitan::RibbonPage* pageTools = ribbonBar()->addPage(tr("&Tools")))
	{
		createGroupTools(pageTools);
	}

	if (Qtitan::RibbonPage* about = ribbonBar()->addPage(tr("&About")))
	{
		createGroupAbout(about);
	}
}

void MainWindow::createGroupTools(Qtitan::RibbonPage* page)
{

	//材料
	if (Qtitan::RibbonGroup* materialGroup = page->addGroup(tr("&material Tools")))
	{

		if (materialGroup)
		{
			QAction* action = materialGroup->addAction(QIcon(":/res/tool_largeButton1.png"), tr("Material add"), Qt::ToolButtonTextUnderIcon);
			//connect(action, SIGNAL(triggered()), this, SLOT(pressButton()));

			action = materialGroup->addAction(QIcon(":/res/tool_largebutton2.png"), tr("Material edit"), Qt::ToolButtonTextUnderIcon);

			action = materialGroup->addAction(QIcon(":/res/tool_largebutton2.png"), tr("Material"), Qt::ToolButtonTextUnderIcon);
			//connect(action, SIGNAL(triggered()), this, SLOT(pressButton()));
		}
	}
	if (Qtitan::RibbonGroup* group = page->addGroup(QIcon(":/res/tool_smallpaste.png"), tr("Tools")))
	{
		if (group)
		{
			//其他工具
			group->setOptionButtonVisible();
			QAction* act = group->optionButtonAction();
			act->setText(tr("Pyqt"));
			act->setIcon(QIcon(":/res/mainwin_clipboardToolTip.png"));
			act->setToolTip(tr("generate input file"));
			act->setStatusTip(tr("gen input file"));


			m_showFatigueDialog = group->addAction(QIcon(":/res/tool_smallcut.png"), tr("&Fatigue"), Qt::ToolButtonTextBesideIcon);
			m_showFatigueDialog->setToolTip(tr("generate fatigue dialog"));

			m_actionCopy = group->addAction(QIcon(":/res/tool_smallcopy.png"), tr("&Tool2"), Qt::ToolButtonTextBesideIcon);
			m_actionCopy->setShortcut(QKeySequence::Copy);

			m_actionFormatPointerAction = group->addAction(QIcon(":/res/mainwin_smallformatpainter.png"), tr("Tool3"), Qt::ToolButtonTextBesideIcon);
		}
	}

}

void MainWindow::createGroupView(Qtitan::RibbonPage* page)
{
	Qtitan::RibbonGroup* mainViewButtons = page->addGroup(tr("Main View"));
	if (mainViewButtons)
	{
		mainViewButtons->setControlsCentering(true);

		QCheckBox* labelBrowserCheck = new QCheckBox(tr("LabelBrowser"));
		labelBrowserCheck->setToolTip(tr("Show Label Browser"));
		labelBrowserCheck->setCheckState(Qt::Checked);
		mainViewButtons->addWidget(labelBrowserCheck);
		connect(labelBrowserCheck, SIGNAL(stateChanged(int)), this, SLOT(onLabelBrowserStateChanged(int)));

		QCheckBox* propertyCheck = new QCheckBox(tr("Property"));
		propertyCheck->setToolTip(tr("Show Property"));
		propertyCheck->setCheckState(Qt::Checked);
		mainViewButtons->addWidget(propertyCheck);
		connect(propertyCheck, SIGNAL(stateChanged(int)), this, SLOT(onPropertyCheckStateChanged(int)));

		QCheckBox* fatigueCheck = new QCheckBox(tr("Fatigue Operation"));
		fatigueCheck->setToolTip(tr("Show Fatigue Operation"));
		fatigueCheck->setCheckState(Qt::Checked);
		mainViewButtons->addWidget(fatigueCheck);
		connect(fatigueCheck, SIGNAL(stateChanged(int)), this, SLOT(onFatigueCheckStateChanged(int)));
	}

	Qtitan::RibbonGroup* viewDirectionGroup = page->addGroup(tr("3D View"));
	if (viewDirectionGroup)
	{
		viewDirectionGroup->setControlsCentering(true);

		viewDirectionGroup->addAction(meshViewer->action_reset);
		viewDirectionGroup->addAction(meshViewer->action_viewLeft);
		viewDirectionGroup->addAction(meshViewer->action_viewRight);
		viewDirectionGroup->addAction(meshViewer->action_viewTop);
		viewDirectionGroup->addAction(meshViewer->action_viewBottom);
		viewDirectionGroup->addAction(meshViewer->action_viewFront);

	}

	Qtitan::RibbonGroup* viewReprsentation = page->addGroup(tr("Reprsentation"));
	if (viewReprsentation)
	{
		viewReprsentation->setControlsCentering(true);
		viewReprsentation->addWidget(meshViewer->reprsentationComboBox);
	}
}

void MainWindow::createGroupAbout(Qtitan::RibbonPage* aboutPage)
{
	Qtitan::RibbonGroup* viewButtons = aboutPage->addGroup(tr("View"));
	if (viewButtons)
	{
		QAction  *helpButton = viewButtons->addAction(QIcon(":/about/about_help.png"), tr("Help"), Qt::ToolButtonIconOnly);
		helpButton->setToolTip(tr("Show Help"));
		connect(helpButton, SIGNAL(triggered()), this, SLOT(help()));

		QAction  *aboutButton = viewButtons->addAction(QIcon(":/about/about_about.png"), tr("About"), Qt::ToolButtonIconOnly);
		aboutButton->setToolTip(tr("Show about fatigue"));
		connect(aboutButton, SIGNAL(triggered()), this, SLOT(about()));
	}
}

void MainWindow::help()
{
	QDesktopServices::openUrl(QUrl("http://www.qq.com"));
}

void MainWindow::about()
{
	QMessageBox::about(this, "fatigue", "asfddddddddddddddddddddddddddd\nasdfsdfsdkf");
}

void MainWindow::paintEvent(QPaintEvent * event)
{
	//meshViewer->renderWindow();

}

void MainWindow::runPython()
{

	numargs = 1111;

	PyRun_SimpleString("execfile('E:/workspace/fatigueProject/lib/fatigue.py')");

}

void MainWindow::onLabelBrowserStateChanged(int state)
{
	if (state == Qt::Unchecked)
	{
		this->removeDockWidget(labelViewer);
		this->update();
	}
	else
	{
		this->addDockWidget(Qt::LeftDockWidgetArea, labelViewer);
		labelViewer->setVisible(true);
	}
}

void MainWindow::onPropertyCheckStateChanged(int state)
{
	if (state == Qt::Unchecked)
	{
		this->removeDockWidget(propViewer);
		this->update();

	}
	else
	{
		this->addDockWidget(Qt::LeftDockWidgetArea, propViewer);
		propViewer->setVisible(true);
	}


}

void MainWindow::onFatigueCheckStateChanged(int state)
{
	if (state == Qt::Unchecked)
	{
		opViewer->setVisible(false);
	}
	else
	{
		opViewer->setVisible(true);
	}
}