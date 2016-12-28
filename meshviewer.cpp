#include "meshviewer.h"

#include <QDebug>
#include <QLabel>
#include <QToolBar>

#include <vtkGenericOpenGLRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkTextMapper.h>
#include <vtkTextActor.h>
#include <vtkTextProperty.h>
#include <vtkCoordinate.h>
#include <vtkPolyDataMapper.h>
#include <vtkFollower.h>
#include <vtkDynamicLoader.h>
#include <vtkMultiBlockDataSetAlgorithm.h>
#include <vtkCell.h>
#include <vtkIdList.h>

/*
MeshView类用用来可视化mesh

*/

MeshViewer::MeshViewer(QWidget *parent)
	:QVTKWidget(parent)
{
	createToolBar();
	
	renderer = vtkSmartPointer<vtkRenderer>::New();
	renderer->SetGradientBackground(true);
	GetRenderWindow()->AddRenderer(renderer);

	mapper = vtkSmartPointer<vtkDataSetMapper>::New();

	mainActor = vtkSmartPointer<vtkActor>::New();
	mainActor->SetMapper(mapper);


	renderer->AddActor(mainActor);

	addOrientationMarkerWidget();
	showOrientationMarkerWidget(true);

	mesh = new Mesh();
	qDebug() << mesh->ugrid->GetReferenceCount();
	connect(mesh, SIGNAL(finishDataLoaded()), this, SLOT(renderWindow()));

	vtkRenderWindowInteractor* renderWindowInteractor = this->GetRenderWindow()->GetInteractor();

	vtkSmartPointer<vtkAreaPicker> areaPicker = vtkSmartPointer<vtkAreaPicker>::New();
	renderWindowInteractor->SetPicker(areaPicker);

	style = vtkSmartPointer<HighlightInteractorStyle>::New();
	style->SetData(mesh->ugrid);

	renderWindowInteractor->SetInteractorStyle(style);

	mapper->SetInputData(mesh->ugrid);

	tooltip = vtkSmartPointer<vtkTooltipItem>::New();

}

MeshViewer::~MeshViewer()
{	
	qDebug() << mesh->ugrid->GetReferenceCount();
	delete mesh;

}

void MeshViewer::createToolBar()
{

	action_reset = new QAction(QIcon(":/res/viewer_Iso.png"), "viewReset", this);
	action_viewLeft = new QAction(QIcon(":/res/viewer_left36t.png"), "viewLeft", this);
	action_viewRight = new QAction(QIcon(":/res/viewer_right36t.png"), "viewRight", this);
	action_viewTop = new QAction(QIcon(":/res/viewer_top36t.png"), "viewTop", this);
	action_viewBottom = new QAction(QIcon(":/res/viewer_bottom36t.png"), "viewBottom", this);
	action_viewFront = new QAction(QIcon(":/res/viewer_front36t.png"), "viewFront", this);
	action_viewBack = new QAction(QIcon(":/res/viewer_back36t.png"), "viewBack", this);
	action_viewMore = new QAction(QIcon(":/res/viewer_book_28.png"), "viewMore", this);
	QAction * action_viewMaxMin = new QAction(QIcon(""), "viewMaxMin", this);
	action_viewMaxMin->setCheckable(true);
	action_viewMore->setCheckable(true);
	reprsentationComboBox = new QComboBox(this);
	QStringList items;
	items << "Point" << "Surface" << "Surface with Edge" << "Wireframe";
	reprsentationComboBox->addItems(items);
	reprsentationComboBox->setCurrentIndex(1);

	connect(action_reset, SIGNAL(triggered()), this, SLOT(viewReset()));
	connect(action_viewLeft, SIGNAL(triggered()), this, SLOT(viewLeft()));
	connect(action_viewRight, SIGNAL(triggered()), this, SLOT(viewRight()));
	connect(action_viewTop, SIGNAL(triggered()), this, SLOT(viewTop()));
	connect(action_viewBottom, SIGNAL(triggered()), this, SLOT(viewBottom()));
	connect(action_viewFront, SIGNAL(triggered()), this, SLOT(viewFront()));
	connect(action_viewBack, SIGNAL(triggered()), this, SLOT(viewBack()));
// 	connect(action_viewMore, SIGNAL(triggered(bool)), this, SLOT(viewMoreInf(bool)));
// 	connect(action_viewMaxMin, SIGNAL(triggered(bool)), this, SLOT(viewMaxMinSign(bool)));
// 	connect(action_viewMaxMin, SIGNAL(triggered(bool)), this, SLOT(setViewMaxMin(bool)));
	connect(reprsentationComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(reprsentationComboBoxIndexChanged(int)));


	QToolBar *toolBar = new QToolBar(this);

	toolBar->addAction(action_reset);
	toolBar->addAction(action_viewLeft);
	toolBar->addAction(action_viewRight);
	toolBar->addAction(action_viewTop);
	toolBar->addAction(action_viewBottom);
	toolBar->addAction(action_viewFront);
	toolBar->addAction(action_viewBack);
	toolBar->addWidget(reprsentationComboBox);
	toolBar->addSeparator();

}

void MeshViewer::viewReset()
{
	renderer->GetActiveCamera()->SetPosition(0, 0, 1);
	renderer->GetActiveCamera()->SetViewUp(0, 1, 0);
	renderer->GetActiveCamera()->SetFocalPoint(0, 0, 0);
	renderer->ResetCamera();
	mainActor->GetProperty()->SetLineWidth(10);
	renderer->GetRenderWindow()->Render();
}

void MeshViewer::viewLeft()
{
	renderer->GetActiveCamera()->SetPosition(0, 0, -1);
	renderer->GetActiveCamera()->SetViewUp(0, 1, 0);
	renderer->GetActiveCamera()->SetFocalPoint(0, 0, 0);
	renderer->ResetCamera();
	mainActor->GetProperty()->SetLineWidth(10);
	renderer->GetRenderWindow()->Render();
}

void MeshViewer::wheelEvent(QWheelEvent *e)
{
	QVTKWidget::wheelEvent(e);
}

void MeshViewer::viewRight()
{
	renderer->GetActiveCamera()->SetPosition(0, 0, 1);
	renderer->GetActiveCamera()->SetViewUp(0, 1, 0);
	renderer->GetActiveCamera()->SetFocalPoint(0, 0, 0);
	renderer->ResetCamera();
	mainActor->GetProperty()->SetLineWidth(10);
	renderer->GetRenderWindow()->Render();
}

void MeshViewer::viewTop()
{

	renderer->GetActiveCamera()->SetPosition(0, 1, 0);
	renderer->GetActiveCamera()->SetViewUp(0, 0, 1);
	renderer->GetActiveCamera()->SetFocalPoint(0, 0, 0);
	renderer->ResetCamera();
	mainActor->GetProperty()->SetLineWidth(10);
	renderer->GetRenderWindow()->Render();
}

void MeshViewer::viewBottom()
{
	renderer->GetActiveCamera()->SetPosition(0, -1, 0);
	renderer->GetActiveCamera()->SetViewUp(0, 0, 1);
	renderer->GetActiveCamera()->SetFocalPoint(0, 0, 0);
	renderer->ResetCamera();
	mainActor->GetProperty()->SetLineWidth(10);
	renderer->GetRenderWindow()->Render();
}

void MeshViewer::viewFront()
{
	renderer->GetActiveCamera()->SetPosition(1, 0, 0);
	renderer->GetActiveCamera()->SetViewUp(0, 0, 1);
	renderer->GetActiveCamera()->SetFocalPoint(0, 0, 0);
	renderer->ResetCamera();
	mainActor->GetProperty()->SetLineWidth(10);
	renderer->GetRenderWindow()->Render();
}

void MeshViewer::viewBack()
{
	renderer->GetActiveCamera()->SetPosition(-1, 0, 0);
	renderer->GetActiveCamera()->SetViewUp(0, 0, 1);
	renderer->GetActiveCamera()->SetFocalPoint(0, 0, 0);
	renderer->ResetCamera();
	mainActor->GetProperty()->SetLineWidth(10);
	renderer->GetRenderWindow()->Render();
}

void MeshViewer::addOrientationMarkerWidget()
{
	vtkSmartPointer<vtkAxesActor> axes =
		vtkSmartPointer<vtkAxesActor>::New();
	OMwidget = vtkOrientationMarkerWidget::New();
	OMwidget->SetOutlineColor(0.9300, 0.5700, 0.1300);
	OMwidget->SetOrientationMarker(axes);
	OMwidget->SetViewport(0.0, 0.0, 0.2, 0.2);
	OMwidget->SetInteractor((vtkRenderWindowInteractor*)(GetInteractor()));
	OMwidget->SetEnabled(1);
	OMwidget->InteractiveOff();
}

void MeshViewer::showOrientationMarkerWidget(bool isShow)
{
	OMwidget->SetEnabled(isShow);

}

void MeshViewer::setBackground(const QColor color)
{
	if (!color.isValid())
	{
		return;
	}
	int r, g, b;
	color.getRgb(&r, &g, &b);
	renderer->SetBackground(r / 255.0, g / 255.0, b / 255.0);
}

void MeshViewer::renderWindow()
{
	qDebug()<<mesh->ugrid->GetReferenceCount();
//	mapper->SetInputData(mesh->ugrid);
	qDebug() << mesh->ugrid->GetReferenceCount();
	renderer->GetRenderWindow()->Render();
}



void MeshViewer::reprsentationComboBoxIndexChanged(int index)
{
	mainActor->GetProperty()->EdgeVisibilityOff();
	mainActor->GetProperty()->SetLineWidth(10);

	switch(index)
	{
	case 0:
		mainActor->GetProperty()->SetRepresentationToPoints();
		style->SelectedActor->GetProperty()->SetRepresentationToPoints();
		break;
	case 1:
		mainActor->GetProperty()->SetRepresentationToSurface();
		style->SelectedActor->GetProperty()->SetRepresentationToSurface();
		break;
	case 2:

		mainActor->GetProperty()->SetLineWidth(1);
		mainActor->GetProperty()->SetRepresentationToSurface();
		mainActor->GetProperty()->EdgeVisibilityOn();

		style->SelectedActor->GetProperty()->SetRepresentationToSurface();
		break;
	case 3:
		mainActor->GetProperty()->SetLineWidth(1);
		mainActor->GetProperty()->SetRepresentationToWireframe();

		style->SelectedActor->GetProperty()->SetRepresentationToWireframe();
		break;
	default:
		;
	}
}

void MeshViewer::loadMeshData(char* fileName)
{
	mesh->loadData(fileName);

}