
#include <QDebug>
#include <QLabel>
#include <QToolBar>

#include "vtkGenericOpenGLRenderWindow.h"


#include "meshviewer.h"

/*
MeshView类用用来可视化mesh

*/

MeshViewer::MeshViewer(QWidget *parent)
	:QVTKWidget2(parent)
{
	
	createToolBar();
	
	renderer = vtkSmartPointer<vtkRenderer>::New();
	renderer->SetGradientBackground(true);

 	vtkRenderWindow * renw = GetRenderWindow();
// 	renw->Print(std::cout);

	GetRenderWindow()->AddRenderer(renderer);

	mapper = vtkSmartPointer<vtkDataSetMapper>::New();

	mainActor = vtkSmartPointer<vtkActor>::New();
	mainActor->SetMapper(mapper);

	renderer->AddActor(mainActor);

	tooltip = vtkSmartPointer<vtkTooltipItem>::New();

	 vtkRenderWindowInteractor* renderWindowInteractor = GetRenderWindow()->GetInteractor();
	 
	 vtkSmartPointer<vtkAreaPicker> areaPicker = vtkSmartPointer<vtkAreaPicker>::New();
	 renderWindowInteractor->SetPicker(areaPicker);
		
	mesh = new Mesh();
	connect(mesh, SIGNAL(finishDataLoaded()), this, SLOT(renderWindowEx()));
	style = vtkSmartPointer<HighlightInteractorStyle>::New();
	style->SetData(mesh->ugrid);

	renderWindowInteractor->SetInteractorStyle(style);

	addOrientationMarkerWidget();
	showOrientationMarkerWidget(true);

	mapper->SetInputData(mesh->ugrid);

	 
 
}

MeshViewer::~MeshViewer()
{	
	qDebug() << mesh->ugrid->GetReferenceCount();
	delete mesh;

}

void MeshViewer::createToolBar()
{
	action_reset = new QAction(QIcon(":/res/viewer_Iso.png"), "viewReset", 0);
	action_viewLeft = new QAction(QIcon(":/res/viewer_left36t.png"), "viewLeft", 0);
	action_viewRight = new QAction(QIcon(":/res/viewer_right36t.png"), "viewRight", 0);
	action_viewTop = new QAction(QIcon(":/res/viewer_top36t.png"), "viewTop", 0);
	action_viewBottom = new QAction(QIcon(":/res/viewer_bottom36t.png"), "viewBottom", 0);
	action_viewFront = new QAction(QIcon(":/res/viewer_front36t.png"), "viewFront", 0);
	action_viewBack = new QAction(QIcon(":/res/viewer_back36t.png"), "viewBack", 0);
	reprsentationComboBox = new QComboBox(0);

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
	connect(reprsentationComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(reprsentationComboBoxIndexChanged(int)));
}
void MeshViewer::wheelEvent(QWheelEvent *e)
{
	QVTKWidget2::wheelEvent(e);
}

void MeshViewer::viewReset()
{
	renderer->GetActiveCamera()->SetPosition(0, 0, 1);
	renderer->GetActiveCamera()->SetViewUp(0, 1, 0);
	renderer->GetActiveCamera()->SetFocalPoint(0, 0, 0);
	renderer->ResetCamera();
	mainActor->GetProperty()->SetLineWidth(10);

	renderWindowEx();
}

void MeshViewer::viewLeft()
{
	renderer->GetActiveCamera()->SetPosition(0, 0, -1);
	renderer->GetActiveCamera()->SetViewUp(0, 1, 0);
	renderer->GetActiveCamera()->SetFocalPoint(0, 0, 0);
	renderer->ResetCamera();
	mainActor->GetProperty()->SetLineWidth(10);

	renderWindowEx();
}

void MeshViewer::viewRight()
{
	renderer->GetActiveCamera()->SetPosition(0, 0, 1);
	renderer->GetActiveCamera()->SetViewUp(0, 1, 0);
	renderer->GetActiveCamera()->SetFocalPoint(0, 0, 0);
	renderer->ResetCamera();
	mainActor->GetProperty()->SetLineWidth(10);

	renderWindowEx();
}

void MeshViewer::viewTop()
{
	renderer->GetActiveCamera()->SetPosition(0, 1, 0);
	renderer->GetActiveCamera()->SetViewUp(0, 0, 1);
	renderer->GetActiveCamera()->SetFocalPoint(0, 0, 0);
	renderer->ResetCamera();
	mainActor->GetProperty()->SetLineWidth(10);

	renderWindowEx();
}

void MeshViewer::viewBottom()
{
	renderer->GetActiveCamera()->SetPosition(0, -1, 0);
	renderer->GetActiveCamera()->SetViewUp(0, 0, 1);
	renderer->GetActiveCamera()->SetFocalPoint(0, 0, 0);
	renderer->ResetCamera();
	mainActor->GetProperty()->SetLineWidth(10);

	renderWindowEx();
}

void MeshViewer::viewFront()
{
	renderer->GetActiveCamera()->SetPosition(1, 0, 0);
	renderer->GetActiveCamera()->SetViewUp(0, 0, 1);
	renderer->GetActiveCamera()->SetFocalPoint(0, 0, 0);
	renderer->ResetCamera();
	mainActor->GetProperty()->SetLineWidth(10);

	renderWindowEx();
}

void MeshViewer::viewBack()
{
	renderer->GetActiveCamera()->SetPosition(-1, 0, 0);
	renderer->GetActiveCamera()->SetViewUp(0, 0, 1);
	renderer->GetActiveCamera()->SetFocalPoint(0, 0, 0);
	renderer->ResetCamera();
	mainActor->GetProperty()->SetLineWidth(10);

	renderWindowEx();
}

void MeshViewer::addOrientationMarkerWidget()
{
	vtkSmartPointer<vtkAxesActor> axes = vtkSmartPointer<vtkAxesActor>::New();
	OMwidget = vtkOrientationMarkerWidget::New();
	OMwidget->SetOutlineColor(0.9300, 0.5700, 0.1300);
	OMwidget->SetOrientationMarker(axes);
	OMwidget->SetViewport(0.0, 0.0, 0.2, 0.2);
	OMwidget->SetInteractor((vtkRenderWindowInteractor*)(GetInteractor()));
	OMwidget->SetEnabled(1);
	//OMwidget->InteractiveOff();
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

void MeshViewer::renderWindowEx()
{
	//qDebug()<<"renderWindowEx--";

	renderer->GetRenderWindow()->Render();
}
/*
void MeshViewer::paintEvent(QPaintEvent * event)
{
	
	qDebug() << "--paintEvent--";
	QWidget::paintEvent(event);
	renderWindowEx();
}

void MeshViewer::paintGL()
{
	 
	qDebug() << "--paintGL--";
	QVTKWidget2::paintGL();
}
*/
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
	renderWindowEx();
}

void MeshViewer::loadMeshData(char* fileName)
{
	mesh->loadData(fileName);
	viewReset();
}