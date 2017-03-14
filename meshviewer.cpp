
#include <QDebug>
#include <QLabel>
#include <QToolBar>
#include <QTime>

#include "vtkVertex.h"
#include "vtkCellArray.h"
#include "vtkGenericOpenGLRenderWindow.h"
#include "vtkMinimalStandardRandomSequence.h"
#include "vtkRendererCollection.h"

#include "treeitem.h"
#include "meshviewer.h"

/*
MeshView类用用来可视化mesh

*/

MeshViewer::MeshViewer(QWidget *parent)
:QVTKWidget(parent)
{
	mainActor = NULL;
	mesh = NULL;

	createToolBar();

	addOrientationMarkerWidget();
	selectType = SelectType_None;
}

MeshViewer::~MeshViewer()
{
	if (mesh)
	{
		delete mesh;
		mesh = NULL;
	}
}

void MeshViewer::loadMeshData(QString fileName)
{
	mesh = new Mesh();
	connect(mesh, SIGNAL(finishDataLoaded()), this, SLOT(renderWindowEx()));

	mesh->loadData(fileName.toLatin1().data());

	vtkDataSetMapper* mapper = vtkDataSetMapper::New();
	mapper->SetInputData(mesh->ugrid);//之前是1个，一下变成3个

	vtkActor* mainActor_ = vtkActor::New();

	mainActor = mainActor_;
	mainActor_->SetMapper(mapper);
	//mainActor->VisibilityOff();
	
	vtkRenderer *renderer = vtkRenderer::New();
	renderer->SetGradientBackground(true);
	renderer->AddActor(mainActor_);

	vtkRenderWindow * renderWindow = this->GetRenderWindow();//从这里开始建立opengl和render的关系,注意renderwindow可以有多个render
	renderWindow->AddRenderer(renderer);

	vtkRenderWindowInteractor *renderWindowInteractor = renderWindow->GetInteractor();

	OMwidget->SetInteractor(renderWindowInteractor);
	OMwidget->SetEnabled(1);
	OMwidget->InteractiveOff();

/*
 	vtkLineWidget *line = vtkLineWidget::New();
 	line->SetPoint1(-4258.4668, -44591.0078, 1926.40979);
 	line->SetPoint2(0, 0, 0);//0, -44360.1211, 4050.34229
 	line->SetInteractor(renderWindowInteractor);
	
 	line->SetEnabled(1);
 	line->Delete();

 	vtkProperty *prop = line->GetLineProperty();
 	prop->SetColor(1.0, 1.0, 0.0);
// 
// 	vtkMyCallback *callback = vtkMyCallback::New();
// 	line->AddObserver(vtkCommand::AnyEvent, callback);
// 	callback->Delete();
 
	double d1[3] = { -4258.4668, -44591.0078, 1926.40979 };
	double d2[3] = { 0, -44360.1211, 4050.34229 };
 

 
	vtkSmartPointer<vtkAngleRepresentation3D> rep = vtkSmartPointer<vtkAngleRepresentation3D>::New();
	rep->ArcVisibilityOff();

	//angleWidget->SetEnabled(1);
 
// 	vtkMyCallback *callback2 = vtkMyCallback::New();
// 	distanceWidget->AddObserver(vtkCommand::AnyEvent, callback2);
// 	callback2->Delete();
*/

	vtkAreaPicker *areaPicker = vtkAreaPicker::New();
	renderWindowInteractor->SetPicker(areaPicker);
	areaPicker->Delete();

	HighlightInteractorStyle* style = HighlightInteractorStyle::New();
	style->SetData(mesh->ugrid);
	style->SetMeshViewer(this);

	//style->SetInteractor(renderWindowInteractor);//以下这两句等效？
	renderWindowInteractor->SetInteractorStyle(style);

	style->Delete();
	mapper->Delete();

	viewReset();

}

void MeshViewer::reset()
{
	vtkRenderWindow * renderWindow = this->GetRenderWindow();
	vtkRenderer * renderer = renderWindow->GetRenderers()->GetFirstRenderer();

	this->resetActor();
	renderWindow->Render();

	if (renderer)
	{
		vtkActorCollection* actorCollection = renderer->GetActors();
		actorCollection->InitTraversal();

		for (vtkIdType i = 0; i < actorCollection->GetNumberOfItems(); i++)
		{
			vtkActor* nextActor = actorCollection->GetNextActor();
			renderer->RemoveActor(nextActor);
			renderWindow->Render();
			nextActor->Delete();
		}
		renderWindow->RemoveRenderer(renderer);
	}

	mainActor = NULL;

	if (mesh)
	{
		delete mesh;
		mesh = NULL;
	}

}

void MeshViewer::resetActor()
{//mainActor
	//ref reset
	vtkRenderWindow * renderWindow = this->GetRenderWindow();
	vtkRenderer * renderer = renderWindow->GetRenderers()->GetFirstRenderer();

	vtkActorCollection* actorCollection = renderer->GetActors();
	actorCollection->InitTraversal();

	for (vtkIdType i = 0; i < actorCollection->GetNumberOfItems(); i++)
	{
		vtkActor* nextActor = actorCollection->GetNextActor();

		std::string className = nextActor->GetClassName();

		if (nextActor != mainActor)
			renderer->RemoveActor(nextActor);
	}

	renderWindowEx();
}

void MeshViewer::updateUi(QMap<QString, QString> &mapData)
{
	QStringList projectFileName;

	for each (QString key in mapData.keys())
	{
		QStringList keyList = key.split("@");
		QStringList valList = mapData[key].split("@");

		if (keyList[0] == "File")
		{
			projectFileName << valList[0];
		}
	}

	if (!projectFileName.empty())
	{
		loadMeshData(projectFileName[0]);//todo:如果有多个文件？
	}
	else
	{
		qDebug() << "error,no model file in project file.";
	}

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

void MeshViewer::viewReset()
{
	vtkRenderWindow * renderWindow = this->GetRenderWindow();
	vtkRenderer * renderer = renderWindow->GetRenderers()->GetFirstRenderer();

	renderer->GetActiveCamera()->SetPosition(0, 0, 1);
	renderer->GetActiveCamera()->SetViewUp(0, 1, 0);
	renderer->GetActiveCamera()->SetFocalPoint(0, 0, 0);
	renderer->ResetCamera();
	mainActor->GetProperty()->SetLineWidth(10);

	renderWindowEx();
}

void MeshViewer::viewLeft()
{
	vtkRenderWindow * renderWindow = this->GetRenderWindow();
	vtkRenderer * renderer = renderWindow->GetRenderers()->GetFirstRenderer();
	renderer->GetActiveCamera()->SetPosition(0, 0, -1);
	renderer->GetActiveCamera()->SetViewUp(0, 1, 0);
	renderer->GetActiveCamera()->SetFocalPoint(0, 0, 0);
	renderer->ResetCamera();
	mainActor->GetProperty()->SetLineWidth(10);

	renderWindowEx();
}

void MeshViewer::viewRight()
{
	vtkRenderWindow * renderWindow = this->GetRenderWindow();
	vtkRenderer * renderer = renderWindow->GetRenderers()->GetFirstRenderer();
	renderer->GetActiveCamera()->SetPosition(0, 0, 1);
	renderer->GetActiveCamera()->SetViewUp(0, 1, 0);
	renderer->GetActiveCamera()->SetFocalPoint(0, 0, 0);
	renderer->ResetCamera();
	mainActor->GetProperty()->SetLineWidth(10);

	renderWindowEx();
}

void MeshViewer::viewTop()
{
	vtkRenderWindow * renderWindow = this->GetRenderWindow();
	vtkRenderer * renderer = renderWindow->GetRenderers()->GetFirstRenderer();
	renderer->GetActiveCamera()->SetPosition(0, 1, 0);
	renderer->GetActiveCamera()->SetViewUp(0, 0, 1);
	renderer->GetActiveCamera()->SetFocalPoint(0, 0, 0);
	renderer->ResetCamera();
	mainActor->GetProperty()->SetLineWidth(10);

	renderWindowEx();
}

void MeshViewer::viewBottom()
{
	vtkRenderWindow * renderWindow = this->GetRenderWindow();
	vtkRenderer * renderer = renderWindow->GetRenderers()->GetFirstRenderer();
	renderer->GetActiveCamera()->SetPosition(0, -1, 0);
	renderer->GetActiveCamera()->SetViewUp(0, 0, 1);
	renderer->GetActiveCamera()->SetFocalPoint(0, 0, 0);
	renderer->ResetCamera();
	mainActor->GetProperty()->SetLineWidth(10);

	renderWindowEx();
}

void MeshViewer::viewFront()
{
	vtkRenderWindow * renderWindow = this->GetRenderWindow();
	vtkRenderer * renderer = renderWindow->GetRenderers()->GetFirstRenderer();
	renderer->GetActiveCamera()->SetPosition(1, 0, 0);
	renderer->GetActiveCamera()->SetViewUp(0, 0, 1);
	renderer->GetActiveCamera()->SetFocalPoint(0, 0, 0);
	renderer->ResetCamera();
	mainActor->GetProperty()->SetLineWidth(10);

	renderWindowEx();
}

void MeshViewer::viewBack()
{
	vtkRenderWindow * renderWindow = this->GetRenderWindow();
	vtkRenderer * renderer = renderWindow->GetRenderers()->GetFirstRenderer();
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

}

void MeshViewer::setBackground(const QColor color)
{
	vtkRenderWindow * renderWindow = this->GetRenderWindow();
	vtkRenderer * renderer = renderWindow->GetRenderers()->GetFirstRenderer();
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
	vtkRenderWindow * renderWindow = this->GetRenderWindow();
	vtkRenderer * renderer = renderWindow->GetRenderers()->GetFirstRenderer();

	if (renderer)
	{
		renderer->GetRenderWindow()->Render();
	}

}

void MeshViewer::reprsentationComboBoxIndexChanged(int index)
{
	mainActor->GetProperty()->EdgeVisibilityOff();
	mainActor->GetProperty()->SetLineWidth(10);

	vtkRenderWindow * renderWindow = this->GetRenderWindow();
	vtkRenderWindowInteractor *renderWindowInteractor = renderWindow->GetInteractor();
	HighlightInteractorStyle* style = (HighlightInteractorStyle*) renderWindowInteractor->GetInteractorStyle();
	switch (index)
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

void MeshViewer::getActorColor(double* color)
{
	vtkSmartPointer<vtkMinimalStandardRandomSequence> rs = vtkSmartPointer<vtkMinimalStandardRandomSequence>::New();
	QTime t = QTime::currentTime();
	qDebug() << t.secsTo(QTime(0, 0, 0));
	rs->SetSeed(t.secsTo(QTime(0, 0, 0)));

	rs->Next(); color[0] = rs->GetValue();
	rs->Next(); color[1] = rs->GetValue();
	rs->Next(); color[2] = rs->GetValue();
}

void MeshViewer::showNodeLabel(QTreeWidgetItem *item)
{
	setSelectType(SelectType_Point);

	TreeItem *treeItem = (TreeItem*) item;
	QString str = treeItem->getAttrData()[item->text(1)];
	QStringList strList = str.split(" ");
	QSet<int> set;
	for each (QString var in strList)
	{
		set.insert(var.toInt());
	}
	currentNodes += set;

	vtkSmartPointer<vtkActor> actor = vtkSmartPointer<vtkActor>::New();
	double color[3];
	this->getActorColor(&color[0]);

	actor->GetProperty()->SetColor(color[0], color[1], color[2]);
	actor->GetProperty()->SetPointSize(8);

	vtkSmartPointer<vtkVertex> vertex = vtkSmartPointer<vtkVertex>::New();
	vtkSmartPointer<vtkCellArray> verArray = vtkSmartPointer<vtkCellArray>::New();

	int cnt = 0;
	for each (int id in currentNodes)
	{
		vertex->GetPointIds()->InsertId(cnt, id);
		verArray->InsertNextCell(vertex);
		cnt++;
	}

	vtkSmartPointer<vtkPolyData> pointset = vtkSmartPointer<vtkPolyData>::New();
	pointset->SetPoints(mesh->ugrid->GetPoints());
	pointset->SetVerts(verArray);

	vtkSmartPointer<vtkPolyDataMapper> pointMapper = vtkSmartPointer<vtkPolyDataMapper>::New();
	pointMapper->SetInputData(pointset);

	actor->SetMapper(pointMapper);


	vtkRenderWindow * renderWindow = this->GetRenderWindow();
	vtkRenderer * renderer = renderWindow->GetRenderers()->GetFirstRenderer();
	renderer->AddActor(actor);
	nodeActorMap[item->text(1)] = actor;

	renderer->Render();
}

void MeshViewer::showElemLabel(QTreeWidgetItem *item)
{
	setSelectType(SelectType_Point);

	TreeItem *treeItem = (TreeItem*) item;
	QString str = treeItem->getAttrData()[item->text(1)];
	QStringList strList = str.split(" ");
	QSet<int> set;
	for each (QString var in strList)
	{
		set.insert(var.toInt());
	}

	currentElems += set;

	vtkSmartPointer<vtkActor> actor = vtkSmartPointer<vtkActor>::New();
	double color[3];
	this->getActorColor(&color[0]);

	actor->GetProperty()->SetColor(color[0], color[1], color[2]);
	actor->GetProperty()->SetPointSize(8);

	vtkSmartPointer<vtkCellArray> cellArray = vtkSmartPointer<vtkCellArray>::New();

	int cnt = 0;
	for each (int id in currentElems)
	{
		cellArray->InsertNextCell(mesh->ugrid->GetCell(vtkIdType(id)));
		cnt++;
	}

	vtkSmartPointer<vtkPolyData> cellset = vtkSmartPointer<vtkPolyData>::New();
	cellset->SetPoints(mesh->ugrid->GetPoints());
	cellset->SetPolys(cellArray);

	vtkSmartPointer<vtkPolyDataMapper> cellMapper = vtkSmartPointer<vtkPolyDataMapper>::New();
	cellMapper->SetInputData(cellset);

	actor->SetMapper(cellMapper);

	vtkRenderWindow * renderWindow = this->GetRenderWindow();
	vtkRenderer * renderer = renderWindow->GetRenderers()->GetFirstRenderer();
	renderer->AddActor(actor);
	elemActorMap[item->text(1)] = actor;

	renderer->Render();
}

void MeshViewer::hideNodeLabel(QTreeWidgetItem *item)
{
	TreeItem *treeItem = (TreeItem*) item;
	QString str = treeItem->getAttrData()[item->text(1)];
	QStringList strList = str.split(" ");
	QSet<int> set;
	for each (QString var in strList)
	{
		set.insert(var.toInt());
	}
	currentNodes -= set;
 
	vtkRenderWindow * renderWindow = this->GetRenderWindow();
	vtkRenderer * renderer = renderWindow->GetRenderers()->GetFirstRenderer();
	renderer->RemoveActor(nodeActorMap[item->text(1)]);

	renderWindowEx();
}

void MeshViewer::hideElemLabel(QTreeWidgetItem *item)
{
	TreeItem *treeItem = (TreeItem*) item;
	QString str = treeItem->getAttrData()[item->text(1)];
	QStringList strList = str.split(" ");
	QSet<int> set;
	for each (QString var in strList)
	{
		set.insert(var.toInt());
	}
	currentElems -= set;

	vtkRenderWindow * renderWindow = this->GetRenderWindow();
	vtkRenderer * renderer = renderWindow->GetRenderers()->GetFirstRenderer();
	renderer->RemoveActor(elemActorMap[item->text(1)]);

	renderWindowEx();
}

QSet<int> MeshViewer::getSelectNodes()
{
	QSet<int> res = currentNodes;

	vtkRenderWindowInteractor *renderWindowInteractor = this->GetInteractor();
	if (this->mesh != NULL)
	{
		HighlightInteractorStyle* style = (HighlightInteractorStyle*) renderWindowInteractor->GetInteractorStyle();
 
		res += style->getCurrentSelectNodes();
	}

	return res;
}

QSet<int> MeshViewer::getSelectElems()
{
	QSet<int> res = currentElems;

	vtkRenderWindowInteractor *renderWindowInteractor = this->GetInteractor();
	if (this->mesh != NULL)
	{
		HighlightInteractorStyle* style = (HighlightInteractorStyle*) renderWindowInteractor->GetInteractorStyle();
		res += style->getCurrentSelectElems();
	}

	return res;
}

void MeshViewer::getSecletion(SelectType type, QList<int>& s)
{//for convenience
 
	if (type == SelectType_Point)
	{
		s.append(getSelectNodes().toList());
	}
	else if (type == SelectType_Cell)
	{
		s.append(getSelectElems().toList());
	}
}

void MeshViewer::selectTypeChanged(SelectType type)
{
	vtkRenderWindow * renderWindow = this->GetRenderWindow();
	vtkRenderWindowInteractor *renderWindowInteractor = renderWindow->GetInteractor();

	setSelectType(type);
	currentElems.clear();
	currentNodes.clear();

	HighlightInteractorStyle* style = (HighlightInteractorStyle*) renderWindowInteractor->GetInteractorStyle();
	style->getCurrentSelectNodes().clear();
 	style->getCurrentSelectElems().clear();
	style->setSelect(type);

	resetActor();

}

void MeshViewer::setSelectType(SelectType type)
{
	selectType = type;
}

SelectType MeshViewer::getSelectType()
{
	return selectType;
}