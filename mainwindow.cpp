#include <QApplication>
#include <QFileDialog>
#include <QCheckBox>
#include <QMessageBox>
#include <QDesktopServices>


#include "stdio.h"

#include "mainwindow.h"
#include "about.h"

#pragma region MainWindow func
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

	connect(labelViewer, SIGNAL(showNodeLabel(QTreeWidgetItem *)), meshViewer, SLOT(showNodeLabel(QTreeWidgetItem *)));
	connect(labelViewer, SIGNAL(hideNodeLabel(QTreeWidgetItem *)), meshViewer, SLOT(hideNodeLabel(QTreeWidgetItem *)));
	connect(labelViewer, SIGNAL(showElemLabel(QTreeWidgetItem *)), meshViewer, SLOT(showElemLabel(QTreeWidgetItem *)));
	connect(labelViewer, SIGNAL(hideElemLabel(QTreeWidgetItem *)), meshViewer, SLOT(hideElemLabel(QTreeWidgetItem *)));

	connect(labelViewer, SIGNAL(resetActor()), meshViewer, SLOT(resetActor()));

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
	connect(m_actionSaveAs, SIGNAL(triggered()), this, SLOT(saveAs()));

	QIcon iconClose;
	iconClose.addPixmap(QPixmap(":/res/mainwin_largeOpenFile.png"));
	iconClose.addPixmap(QPixmap(":/res/mainwin_smallOpen.png"));
	m_actionCloseFile = new QAction(iconClose, tr("&Close"), this);
	m_actionCloseFile->setShortcut(QKeySequence::Save);
	m_actionCloseFile->setToolTip(tr("Save"));
	m_actionCloseFile->setStatusTip(tr("Save the active document"));
	connect(m_actionCloseFile, SIGNAL(triggered()), this, SLOT(closeProject()));

}

void MainWindow::writeToMap(QMap<QString, QString> &mapData, QString objName, QString val, int type, int count)
{

}

void MainWindow::updateUi(QMap<QString, QString> &mapData)
{
	QStringList projectFileName;
	QMap<QString, QString> NodeLabel;
	QMap<QString, QString> ElementLabel;

	for each (QString key in mapData.keys())
	{
		QStringList keyList = key.split("@");
		QStringList valList = mapData[key].split("@");

		if (keyList[0] == "File")
		{
			projectFileName << valList[0];
		}
		else if (keyList[0] == "NodeLabel")
		{
			NodeLabel[valList[0]] = valList[1];
		}
		else if (keyList[0] == "ElementLabel")
		{
			ElementLabel[valList[0]] = valList[1];
		}
	}
	
	//read mesh
	if (!projectFileName.empty())
	{
		readMesh(projectFileName[0]);//todo:����ж���ļ���
	}

	//update labelViewer
	for each (QString labelName in NodeLabel.keys())
	{
		labelViewer->addNodeLabel(labelName, NodeLabel[labelName]);
	}
	
	for each (QString labelName in ElementLabel.keys())
	{
		labelViewer->addElemLabel(labelName, ElementLabel[labelName]);
	}

}

void MainWindow::updateMapData(QMap<QString, QString> &mapData)
{

}

void MainWindow::openProject()
{
	propViewer->setData(QString("openProject"));

	QString fileName = QFileDialog::getOpenFileName(this, tr("Open Project File"),
		QDir::currentPath(),
		tr("fatigue project Files (*.fproj)"));

	if (fileName.isEmpty())
		return;

	projectManager.parse(fileName);

	this->updateUi(projectManager.getModelData());

	FatigueWidget *w = (FatigueWidget *)(opViewer->widget());
	w->updateUi(projectManager.getUiData());
}

void MainWindow::save()
{
	FatigueWidget *w = (FatigueWidget *)(opViewer->widget());
	w->updateMapData(projectManager.getUiData());
 
	if (projectManager.getProjectFileName().isEmpty())
	{
		saveAs();
	}
	else
	{
		projectManager.save();
	}
		
}

void MainWindow::saveAs()
{
	FatigueWidget *w = (FatigueWidget *)(opViewer->widget());
	w->updateMapData(projectManager.getUiData());

	QString fileName = QFileDialog::getSaveFileName(this, tr("Save Project File"),
		QDir::currentPath(),
		tr("fatigue project Files (*.fproj)"));

	if (fileName.isEmpty())
	{
		qDebug() << "file name is empty";
		return;
	}
		

	projectManager.setProjectFileName(fileName);
	projectManager.save();
}

void MainWindow::closeProject()
{
	//todo:Ӧ��ִ��һЩ���������Ա����´δ�
}

void MainWindow::importFile()
{
	QString fileName = QFileDialog::getOpenFileName(this, tr("Open File..."),
		QString(), tr("Ansys result (*.fil *.odb);;All Files (*)"));

	if (fileName.isEmpty())
	{
		qDebug() << "file name is empty";
		return;
	}

	readMesh(fileName);

}

void MainWindow::readMesh(QString fileName)
{
	// read mesh
	meshViewer->loadMeshData(fileName.toLatin1().data());

	projectManager.reset();
	QStringList sl;
	sl << fileName;
	projectManager.setModelFileName(sl);

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

	//����
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
			//��������
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

void MainWindow::runPython()
{

	numargs = 1111;

	PyRun_SimpleString("execfile('E:/workspace/fatigueProject/lib/fatigue.py')");

}

#pragma endregion MainWindow func

#pragma region label func
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
#pragma endregion label func