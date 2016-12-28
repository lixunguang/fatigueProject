
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QDebug>
#include <QTimer>

#include "ribbonwindow.h"

#include "vtkUnstructuredGrid.h"
#include "vtkCellData.h"
#include "vtkDoubleArray.h"
#include "vtkDataSetMapper.h"

#include <vtkSmartPointer.h>
#include <vtkSphereSource.h>
#include <vtkPolyDataMapper.h>
#include <vtkActor.h>
#include <vtkRenderWindow.h>
#include <vtkRenderer.h>
#include "vtkProperty.h"
#include <vtkTriangle.h>  
#include <vtkDataArray.h>  
#include <vtkVersion.h>

#include <vtkActor.h>
#include <vtkAreaPicker.h>
#include <vtkDataSetMapper.h>
#include <vtkDataSetSurfaceFilter.h>
#include <vtkExtractPolyDataGeometry.h>
#include <vtkIdFilter.h>
#include <vtkIdTypeArray.h>
#include <vtkInteractorStyleRubberBandPick.h>
#include <vtkObjectFactory.h>
#include <vtkPlanes.h>
#include <vtkPointData.h>
#include <vtkPolyData.h>
#include <vtkPolyDataMapper.h>
#include <vtkProperty.h>
#include <vtkRenderer.h>
#include <vtkRendererCollection.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkSmartPointer.h>
#include <vtkSphereSource.h>
#include <vtkUnstructuredGrid.h>
#include <vtkVersion.h>
#include <vtkVertexGlyphFilter.h>

//#include "HighlightInteractorStyle.h"

#include "Python.h"

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

	void createGroup(Qtitan::RibbonPage* page);

protected Q_SLOTS:
	void openFile();
	void closeFile();

	void openProject();
	void closeProject();

	virtual void options(QAction* action);

	void showFatigueDialog();

	void runPython();

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