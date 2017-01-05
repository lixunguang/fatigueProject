#include <QApplication>
#include <QFileDialog>
#include <QCheckBox>

#include "stdio.h"

#include <Python.h>

#include "mainwindow.h"


int numargs;

MainWindow::MainWindow(QWidget *parent) :RibbonWindow(parent)
{
	createAction();
	createMenuFile();
	createRibbon();

	connect(m_showFatigueDialog, SIGNAL(triggered()), this, SLOT(showFatigueDialog()));

	ribbonBar()->setFrameThemeEnabled();

	meshViewer = new MeshViewer(this);
	setCentralWidget(meshViewer);

	labelViewer = new LabelViewer(this);
	propViewer = new PropertyViewer(this);
	opViewer = new OperationViewer(this);

	this->addDockWidget(Qt::LeftDockWidgetArea, labelViewer);
	this->addDockWidget(Qt::LeftDockWidgetArea, propViewer);
	this->addDockWidget(Qt::RightDockWidgetArea, opViewer);

	labelViewer->setWindowTitle("Label Browser");
	labelViewer->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
	labelViewer->setMinimumWidth(80);

	propViewer->setWindowTitle("Properties");
	propViewer->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
	propViewer->setMinimumWidth(80);

	opViewer->setWindowTitle("operation");
	opViewer->setAllowedAreas(Qt::RightDockWidgetArea);
	opViewer->setMinimumWidth(250);

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
	connect(m_actionOpenFile, SIGNAL(triggered()), this, SLOT(openFile()));

	QIcon iconClose;
	iconClose.addPixmap(QPixmap(":/res/mainwin_largeOpenFile.png"));
	iconClose.addPixmap(QPixmap(":/res/mainwin_smallOpen.png"));
	m_actionCloseFile = new QAction(iconClose, tr("&Close"), this);
	m_actionCloseFile->setShortcut(QKeySequence::Save);
	m_actionCloseFile->setToolTip(tr("Save"));
	m_actionCloseFile->setStatusTip(tr("Save the active document"));
	connect(m_actionCloseFile, SIGNAL(triggered()), this, SLOT(closeFile()));

}

void MainWindow::openFile()
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

void MainWindow::openProject()
{//todo:
	//1 parase xml ,get model file path

	//2 get the label that has been added

}

void MainWindow::closeProject()
{

}

void MainWindow::closeFile()
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
		popupBar->addAction(m_actionCloseFile);

	}
}

void MainWindow::createRibbon()
{
	if (Qtitan::RibbonPage* pageTools = ribbonBar()->addPage(tr("&Tools")))
	{
		createGroupTools(pageTools);
		// 		createGroupFont(pageHome);
		// 		createGroupParagraph(pageHome);
		// 		createGroupEditing(pageHome);
	}

	if (Qtitan::RibbonPage* pageView = ribbonBar()->addPage(tr("&View")))
	{
		createGroupView(pageView);
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
	Qtitan::RibbonGroup* viewButtons = page->addGroup(tr("View"));
	if (viewButtons)
	{
		// 		viewButtons->setOptionButtonVisible();
		// 		QAction* act = viewButtons->optionButtonAction();
		// 		act->setToolTip(tr("Checkboxes && Radio Buttons"));
		// 		connect(act, SIGNAL(triggered()), this, SLOT(pressButton()));

		viewButtons->setControlsCentering(true);
		QCheckBox* labelBrowserCheck = new QCheckBox(tr("LabelBrowser"));
		labelBrowserCheck->setToolTip(tr("Show Label Browser"));
		labelBrowserCheck->setCheckState(Qt::Checked);
		viewButtons->addWidget(labelBrowserCheck);

		QCheckBox* propertyCheck = new QCheckBox(tr("Property"));
		propertyCheck->setToolTip(tr("Show Property"));
		propertyCheck->setCheckState(Qt::Checked);
		viewButtons->addWidget(propertyCheck);

		QCheckBox* fatigueCheck = new QCheckBox(tr("Fatigue Operation"));
		fatigueCheck->setToolTip(tr("Show Fatigue Operation"));
		fatigueCheck->setCheckState(Qt::Checked);
		viewButtons->addWidget(fatigueCheck);


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
	qDebug() << "MainWindow::help()";
}

void MainWindow::about()
{

	qDebug() << "MainWindow::about()";
}

void MainWindow::paintEvent(QPaintEvent * event)
{
	meshViewer->renderWindow();

}

void MainWindow::runPython()
{

	numargs = 1111;

	PyRun_SimpleString("execfile('E:/workspace/fatigueProject/lib/fatigue.py')");

}