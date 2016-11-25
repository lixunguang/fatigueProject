#include "MeshViewer.h"

#include <QDebug>
#include <QLabel>

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

// #include "EMTSign.h"
// #include "EMTSignActor.h"
// #include "EMTFileReader.h"
//#include "vtkMeshReader.h"


MeshViewer::MeshViewer(QWidget *parent)
	:QVTKWidget(parent)
{
	createToolBar();
	
	renderer = vtkSmartPointer<vtkRenderer>::New();
	GetRenderWindow()->AddRenderer(renderer);
	mapper = vtkSmartPointer<vtkDataSetMapper>::New();
	renderer->SetGradientBackground(true);

	addOrientationMarkerWidget();
	showOrientationMarkerWidget(true);

}

MeshViewer::~MeshViewer()
{

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
	connect(action_viewMore, SIGNAL(triggered(bool)), this, SLOT(viewMoreInf(bool)));
	connect(action_viewMaxMin, SIGNAL(triggered(bool)), this, SLOT(viewMaxMinSign(bool)));
	connect(action_viewMaxMin, SIGNAL(triggered(bool)), this, SLOT(setViewMaxMin(bool)));
	connect(reprsentationComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(reprsentationComboBoxIndexChanged(int)));

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
	renderWindow();
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
	renderer->GetRenderWindow()->Render();
}


void MeshViewer::display(QString theName)
{
	Q_UNUSED(theName)
	mapper->ScalarVisibilityOn();
	//mapper->SetScalarModeToUsePointData();
	mapper->SetScalarModeToUseCellData();
	mapper->SetColorModeToMapScalars();
	mapper->SetUseLookupTableScalarRange(1);
	mapper->SetInputData(geometryData);
	mapper->SetInterpolateScalarsBeforeMapping(1);


	lut = vtkSmartPointer<vtkLookupTable>::New();
	lut->SetHueRange(0.667,0);
	lut->SetValueRange(1,1);
	lut->SetNumberOfColors(16);
	lut->SetSaturationRange (1, 1);
	lut->Build();

	scalarBar = vtkSmartPointer<vtkScalarBarActor>::New();
	scalarBar->SetLookupTable(mapper->GetLookupTable());
	scalarBar->SetWidth(0.1);
	scalarBar->SetHeight(0.5);
	scalarBar->SetPosition(0.05,0.4);
	scalarBar->SetLabelFormat("%.4e");
	scalarBar->SetNumberOfLabels(6);
	scalarBar->SetTitle("Scalar");

	mapper->SetLookupTable(lut);
	scalarBar->SetLookupTable(lut);
	mapper->Update();
	mainActor = vtkSmartPointer<vtkActor>::New();
	mainActor->SetMapper(mapper);
	mainActor->GetProperty()->SetLineWidth(10);
	renderer->AddActor(mainActor);
	renderer->AddActor2D(scalarBar);
	renderer->ResetCamera();
	renderer->GetRenderWindow()->Render();


}


void MeshViewer::reprsentationComboBoxIndexChanged(int index)
{
	mainActor->GetProperty()->EdgeVisibilityOff();
	mainActor->GetProperty()->SetLineWidth(10);
	if (warpActor)
	{
		warpActor->GetProperty()->EdgeVisibilityOff();
		warpActor->GetProperty()->SetLineWidth(10);
	}
	
	switch(index)
	{
	case 0:
		mainActor->GetProperty()->SetRepresentationToPoints();
		
		if (warpActor)
		{
			warpActor->GetProperty()->SetRepresentationToPoints();
		}
		break;
	case 1:
		mainActor->GetProperty()->SetRepresentationToSurface();
		
		if (warpActor)
		{
			warpActor->GetProperty()->SetRepresentationToSurface();
		}
		break;
	case 2:
		if(mainActor->GetMapper()->GetInput()->GetCellType(1) == VTK_LINE)
		{
			break;
		}
		mainActor->GetProperty()->SetLineWidth(1);
		mainActor->GetProperty()->SetRepresentationToSurface();
		mainActor->GetProperty()->EdgeVisibilityOn();
		
		if (warpActor)
		{
			warpActor->GetProperty()->SetLineWidth(1);
			warpActor->GetProperty()->SetRepresentationToSurface();
			warpActor->GetProperty()->EdgeVisibilityOn();
		}
		break;
	case 3:
		mainActor->GetProperty()->SetLineWidth(1);
		mainActor->GetProperty()->SetRepresentationToWireframe();
		
		if (warpActor)
		{
			warpActor->GetProperty()->SetRepresentationToWireframe();
			warpActor->GetProperty()->SetLineWidth(1);
		}
		break;
	default:
		;
	}
}

