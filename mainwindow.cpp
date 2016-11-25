#include <QApplication>
#include <QFileDialog>

#include "stdio.h"

#include <Python.h>

#include "mainwindow.h"


int numargs;

MainWindow::MainWindow(QWidget *parent ) :RibbonWindow(parent)
{
	createAction();
	createMenuFile();
	createRibbon();

	connect(m_showFatigueDialog, SIGNAL(triggered()), this, SLOT(showFatigueDialog()));

	ribbonBar()->setFrameThemeEnabled();

	meshViewer = new MeshViewer(this);
	setCentralWidget(meshViewer);

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
	connect(m_actionOpenFile, SIGNAL(triggered()), this, SLOT(fileOpen()));

	QIcon iconClose;
	iconClose.addPixmap(QPixmap(":/res/mainwin_largeOpenFile.png"));
	iconClose.addPixmap(QPixmap(":/res/mainwin_smallOpen.png"));
	m_actionCloseFile = new QAction(iconClose, tr("&Close"), this);
	m_actionCloseFile->setShortcut(QKeySequence::Save);
	m_actionCloseFile->setToolTip(tr("Save"));
	m_actionCloseFile->setStatusTip(tr("Save the active document"));
	connect(m_actionCloseFile, SIGNAL(triggered()), this, SLOT(closeFile()));

}

void MainWindow::fileOpen()
{
	QString fn = QFileDialog::getOpenFileName(this, tr("Open File..."),
		QString(), tr("HTML-Files (*.htm *.html);;All Files (*)"));

	// read mesh

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
	if (Qtitan::RibbonPage* pageHome = ribbonBar()->addPage(tr("&Tools")))
	{
		createGroup(pageHome);
// 		createGroupFont(pageHome);
// 		createGroupParagraph(pageHome);
// 		createGroupEditing(pageHome);
	}

	if (Qtitan::RibbonPage* pageLayout = ribbonBar()->addPage(tr("&View")))
	{

	}


}

void MainWindow::createGroup(Qtitan::RibbonPage* page)
{
	if (Qtitan::RibbonGroup* group = page->addGroup(QIcon(":/res/mainwin_smallpaste.png"), tr("Clipboard")))
	{
		group->setOptionButtonVisible();
		QAction* act = group->optionButtonAction();
		act->setText(tr("Pyqt"));
		act->setIcon(QIcon(":/res/mainwin_clipboardToolTip.png"));
		act->setToolTip(tr("generate input file"));
		act->setStatusTip(tr("gen input file"));
		//connect(act, SIGNAL(triggered()), this, SLOT(optionClipboard()));



		m_showFatigueDialog = group->addAction(QIcon(":/res/mainwin_smallcut.png"), tr("&Fatigue"), Qt::ToolButtonTextBesideIcon);
		m_showFatigueDialog->setToolTip(tr("generate fatigue dialog"));

		m_actionCopy = group->addAction(QIcon(":/res/mainwin_smallcopy.png"),tr("&Copy"), Qt::ToolButtonTextBesideIcon);
		m_actionCopy->setShortcut(QKeySequence::Copy);

		m_actionFormatPointerAction = group->addAction(QIcon(":/res/mainwin_smallformatpainter.png"),tr("F&ormat Pointer"), Qt::ToolButtonTextBesideIcon);
	}

}

void MainWindow::runPython()
{

	numargs = 1111;

	PyRun_SimpleString("execfile('E:/workspace/fatigueProject/lib/fatigue.py')");

}