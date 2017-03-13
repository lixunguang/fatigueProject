
#include "vtkRenderWindowInteractor.h"
#include "vtkRenderWindow.h"
#include "vtkProperty.h"
#include "vtkCellArray.h"
#include "vtkRendererCollection.h"
#include "vtkVertexGlyphFilter.h"
#include "vtkCellPicker.h"
#include "vtkPointPicker.h"
#include "vtkVertex.h"
#include "vtkPlanes.h"
#include "vtkHardwareSelector.h"
#include "vtkMath.h"
#include "vtkProp3DCollection.h"
#include "vtkSelection.h"
#include "vtkAreaPicker.h"
#include "vtkObjectFactory.h"
#include "vtkHardwareSelector.h"
#include "vtkNew.h"
#include "vtkExtractGeometry.h"
#include "vtkTriangleFilter.h"
#include "vtkExtractCells.h"
#include "vtkProp3DCollection.h"
#include "vtkGenericCell.h"

#include "HighlightInteractorStyle.h"

vtkStandardNewMacro(HighlightInteractorStyle);

HighlightInteractorStyle::HighlightInteractorStyle() : vtkInteractorStyleRubberBandPick()
{
	this->SelectedMapper = vtkSmartPointer<vtkPolyDataMapper>::New();
	this->SelectedActor = vtkSmartPointer<vtkActor>::New();
	
	selectType = SelectType_None;
	circleR = 12;

}

HighlightInteractorStyle::~HighlightInteractorStyle()
{

}

void HighlightInteractorStyle::SetMeshViewer(MeshViewer *mv)
{
	meshViewer = mv;
}

void HighlightInteractorStyle::SetData(vtkUnstructuredGrid* data)
{
	this->ugrid = data;
}

vtkUnstructuredGrid* HighlightInteractorStyle::GetData()
{
	return this->ugrid;
}

void HighlightInteractorStyle::OnChar()
{
	qDebug() << "HighlightInteractorStyle::OnChar";
	vtkInteractorStyleRubberBandPick::OnChar();
}

void HighlightInteractorStyle::setSelect(SelectType type)
{
	selectType = type;
	if (type ==  SelectType_Cell || type == SelectType_Point)
	{
		this->CurrentMode = VTKISRBP_SELECT;
	}
	else
	{
		this->CurrentMode = VTKISRBP_ORIENT;
	}

}

QSet<int>& HighlightInteractorStyle::getCurrentSelectNodes()
{
	return currentSelectNodes;
}

QSet<int>& HighlightInteractorStyle::getCurrentSelectElems()
{
	return currentSelectElems;
}

#include "meshviewer.h"
void HighlightInteractorStyle::OnLeftButtonUp()
{

	// Forward events
	vtkInteractorStyleRubberBandPick::OnLeftButtonUp();
	//	selectTest();	
	//	selectSinglePoint();//单个点
	//	selectSingleCell();//单个单元

	qDebug() << "HighlightInteractorStyle::OnLeftButtonUp";
	if (selectType == SelectType_Point)
	{
		createPointsMap();
		meshViewer->resetActor();
		selectAreaPoints();
		currentSelectElems.clear();
	}
	else if (selectType == SelectType_Cell)
	{
		createPointsMap();
		meshViewer->resetActor();
		selectAreaCells();
		currentSelectNodes.clear();
	}

	return;

	InvokeEvent(12346);
	if (d.size() > 4)
	{
		d.pop_front();
		d.pop_front();
	}
	int* pos = this->GetInteractor()->GetEventPosition();
	d.push_back(pos[0]);
	d.push_back(pos[1]);

}

void HighlightInteractorStyle::selectCircle(int* pos, int *prepos)
{//test
	vtkSmartPointer<vtkHardwareSelector> selector = vtkSmartPointer<vtkHardwareSelector>::New();
	selector->SetRenderer(this->GetInteractor()->GetRenderWindow()->GetRenderers()->GetFirstRenderer());

	float p1[3] = { 0 };
	float p2[3] = { 0 };
	p1[0] = (float) pos[0]; p1[1] = (float) pos[1];
	p2[0] = (float) prepos[0]; p2[1] = (float) prepos[1];
	double dist = sqrt(vtkMath::Distance2BetweenPoints(p1, p2));

	std::cout << "  preposx: " << d.at(0) << "  preposy: " << d.at(1) << "  posx : " << d.at(2) << "  posy: " << d.at(3) << "  " << std::endl;

	int* temp = this->Interactor->GetRenderWindow()->GetSize();
	unsigned int windowSize[4];
	// 	windowSize[0] = (pos[0] - dist)>1 ? (pos[0] - dist):1;// temp[2];
	// 	windowSize[1] = pos[1] - dist>1 ? (pos[1] - dist) : 1;//temp[3];
	// 	windowSize[2] = pos[0] + dist>1 ? (pos[0] + dist) : 1;//temp[0];
	// 	windowSize[3] = pos[1] + dist>1 ? (pos[1] + dist) : 1;//temp[1];

	windowSize[0] = pos[0] - 120;
	windowSize[1] = pos[1];
	windowSize[2] = pos[0];
	windowSize[3] = pos[1] + 120;

	for (int i = 0; i < 4; i++)
	{
		if (windowSize[i] < 0)
		{
			windowSize[i] = 1;
		}
		std::cout << "ws: " << windowSize[i] << endl;
	}
	//selector->SetArea(windowSize);
	selector->SetArea(pos[0] - dist, pos[1] - dist, pos[0] + dist, pos[1] + dist);

	selector->SetFieldAssociation(vtkDataObject::FIELD_ASSOCIATION_POINTS);
	//	selector->SetFieldAssociation(vtkDataObject::FIELD_ASSOCIATION_CELLS);

	vtkSelection* sel = selector->Select();
	sel->Print(std::cout);

}

void HighlightInteractorStyle::createPointsMap()
{
	if (this->CurrentMode == VTKISRBP_SELECT)
	{
		currentSelectNodes.clear();
		nodeIdMap.clear();

		vtkPlanes* frustum = static_cast<vtkAreaPicker*>(this->GetInteractor()->GetPicker())->GetFrustum();
		vtkSmartPointer<vtkExtractGeometry> extractGeometry = vtkSmartPointer<vtkExtractGeometry>::New();
		extractGeometry->SetImplicitFunction(frustum);
		extractGeometry->SetInputData(this->ugrid);
		extractGeometry->Update();

		//此处可能需要反向推断point id,他并没有提供所选的point id，更别说提供在inp里面的id了
		//还真是得反向推断,目前是没找到好办法
		vtkUnstructuredGrid *pPoints = extractGeometry->GetOutput();
		vtkIdType num = pPoints->GetNumberOfPoints();

		vtkIdType ptID;
		for (int i = 0; i < num; i++)
		{
			double *tmp = pPoints->GetPoint(i);
//			std::cout << tmp[0] << " " << tmp[1] << " " << tmp[2] << std::endl;
			ptID = this->ugrid->FindPoint(pPoints->GetPoint(i));
			if (ptID > 0)
			{
				currentSelectNodes.insert(ptID);
				nodeIdMap[i] = ptID;
			}
		}
	}
}

void HighlightInteractorStyle::selectAreaPoints()
{
	if (this->CurrentMode == VTKISRBP_SELECT)
	{
		vtkPlanes* frustum = static_cast<vtkAreaPicker*>(this->GetInteractor()->GetPicker())->GetFrustum();

		vtkSmartPointer<vtkExtractGeometry> extractGeometry = vtkSmartPointer<vtkExtractGeometry>::New();
		extractGeometry->SetImplicitFunction(frustum);
		extractGeometry->SetInputData(this->ugrid);
		extractGeometry->Update();

		vtkSmartPointer<vtkVertexGlyphFilter> glyphFilter = vtkSmartPointer<vtkVertexGlyphFilter>::New();
		glyphFilter->SetInputConnection(extractGeometry->GetOutputPort());
		glyphFilter->Update();

		vtkPolyData* selected = glyphFilter->GetOutput();

		vtkSmartPointer<vtkPolyDataMapper> mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
		vtkSmartPointer<vtkActor> actor = vtkSmartPointer<vtkActor>::New();
		
		mapper->SetInputData(selected);
		mapper->ScalarVisibilityOff();

		actor->GetProperty()->SetColor(1.0, 0.0, 0.0);
		actor->GetProperty()->SetPointSize(3);
		actor->SetMapper(mapper);

		this->CurrentRenderer->AddActor(actor);
 		this->GetInteractor()->GetRenderWindow()->Render();
 		this->HighlightProp(NULL);
	}
}

void HighlightInteractorStyle::selectAreaCells()
{
	if (this->CurrentMode == VTKISRBP_SELECT)
	{
		currentSelectElems.clear();

		vtkPlanes* frustum = static_cast<vtkAreaPicker*>(this->GetInteractor()->GetPicker())->GetFrustum();

		vtkSmartPointer<vtkExtractGeometry> extractGeometry = vtkSmartPointer<vtkExtractGeometry>::New();
		extractGeometry->SetImplicitFunction(frustum);
		extractGeometry->SetInputData(this->ugrid);
		extractGeometry->Update();

		//此处可能需要反向推断cell id
		//还真是得反向推断,目前是没找到好办法
		//在其他的引擎，比如occ，proe里面也发现了类似的问题：不能从下往上找
		vtkUnstructuredGrid *pCells = extractGeometry->GetOutput();
		vtkIdType num = pCells->GetNumberOfCells();
		QVector<int> orignal_pts;
		vtkIdList *idList = vtkIdList::New();

		for (int i = 0; i < num; i++)
		{//每一个cell
			orignal_pts.clear();

			for (int j = 0; j < pCells->GetCell(i)->GetNumberOfPoints(); j++)
			{//根据新节点id找到老节点id建立映射
				//std::cout << pCells->GetCell(i)->GetPointId(j) << " --> " << nodeIdMap[pCells->GetCell(i)->GetPointId(j)]<<"   ";
				orignal_pts.push_back(nodeIdMap[pCells->GetCell(i)->GetPointId(j)]);
			}

			//然后根据老节点，找到单元id
			QSet<int> secondset;
			for each (int ptid in orignal_pts)
			{
				QSet<int> firstset;

				ugrid->GetPointCells(ptid, idList);
				for (int i = 0; i < idList->GetNumberOfIds(); ++i)
				{
					firstset.insert(idList->GetId(i));
				}

				if (secondset.empty())
				{
					secondset = secondset.unite(firstset);
				}
				else
				{
					secondset = secondset.intersect(firstset);
				}
			}

			currentSelectElems.unite(secondset);

			secondset.clear();
		}
		idList->Delete();

		if (currentSelectElems.empty())
		{
			return;
		}
		vtkSmartPointer<vtkPolyData> cellset = vtkSmartPointer<vtkPolyData>::New();
		cellset->SetPoints(ugrid->GetPoints());

		vtkSmartPointer<vtkCellArray> va = vtkSmartPointer<vtkCellArray>::New();
		for each (int var in currentSelectElems)
		{
			va->InsertNextCell(ugrid->GetCell(var));
		}
		cellset->SetPolys(va);

		vtkSmartPointer<vtkPolyDataMapper> mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
		vtkSmartPointer<vtkActor> actor = vtkSmartPointer<vtkActor>::New();
	
		mapper->SetInputData(cellset);
		mapper->ScalarVisibilityOff();

		actor->GetProperty()->SetColor(1.0, 0.0, 0.0);
		actor->GetProperty()->SetPointSize(3);
		actor->SetMapper(mapper);

		this->CurrentRenderer->AddActor(actor);
		this->GetInteractor()->GetRenderWindow()->Render();
		this->HighlightProp(NULL);
	}
}

void HighlightInteractorStyle::selectTest()
{//for test
	if (this->CurrentMode == VTKISRBP_SELECT)
	{
		vtkPlanes* frustum = static_cast<vtkAreaPicker*>(this->GetInteractor()->GetPicker())->GetFrustum();

		vtkSmartPointer<vtkExtractGeometry> extractGeometry = vtkSmartPointer<vtkExtractGeometry>::New();
		extractGeometry->SetImplicitFunction(frustum);
		extractGeometry->SetInputData(this->ugrid);
		extractGeometry->Update();

		vtkSmartPointer<vtkVertexGlyphFilter> glyphFilter = vtkSmartPointer<vtkVertexGlyphFilter>::New();
		glyphFilter->SetInputConnection(extractGeometry->GetOutputPort());
		glyphFilter->Update();

		vtkUnstructuredGrid *pcells = extractGeometry->GetOutput();
		vtkIdType num = pcells->GetNumberOfPoints();

		vtkIdType ptID;
		for (int i = 0; i < num; i++)
		{
			ptID = this->ugrid->FindPoint(pcells->GetPoint(i));
			if (ptID > 0)
			{
				std::cout << "ptid: " << (int) ptID << std::endl;
			}
		}

		vtkPolyData* selected = glyphFilter->GetOutput();


		//此处可能需要反向推断point id
		//还真是得反向推断,目前没找到好办法

		this->SelectedMapper->SetInputData(selected);//selected polycell
		this->SelectedMapper->ScalarVisibilityOff();

		this->SelectedActor->GetProperty()->SetColor(1.0, 0.0, 0.0);
		this->SelectedActor->GetProperty()->SetPointSize(3);

		this->CurrentRenderer->AddActor(SelectedActor);
		this->GetInteractor()->GetRenderWindow()->Render();
		this->HighlightProp(NULL);
	}
}

void HighlightInteractorStyle::selectSingleCell()
{
	int* pos = this->GetInteractor()->GetEventPosition();
	vtkSmartPointer<vtkCellPicker> picker = vtkSmartPointer<vtkCellPicker>::New();
	picker->SetTolerance(0.005);

	picker->Pick(pos[0], pos[1], 0, this->GetInteractor()->GetRenderWindow()->GetRenderers()->GetFirstRenderer());
	//std::cout << "Cell id is: " << picker->GetCellId() << std::endl;

	if (picker->GetCellId() == -1)
	{
		return;
	}

	vtkSmartPointer<vtkPolyData> cellset = vtkSmartPointer<vtkPolyData>::New();
	cellset->SetPoints(ugrid->GetPoints());

	vtkSmartPointer<vtkCellArray> va = vtkSmartPointer<vtkCellArray>::New();
	va->InsertNextCell(ugrid->GetCell(picker->GetCellId()));

	cellset->SetPolys(va);

	vtkSmartPointer<vtkPolyDataMapper> mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
	vtkSmartPointer<vtkActor> actor = vtkSmartPointer<vtkActor>::New();

	mapper->SetInputData(cellset);
	mapper->ScalarVisibilityOff();

	actor->GetProperty()->SetColor(1.0, 0.0, 0.0);
	actor->GetProperty()->SetPointSize(3);
	actor->SetMapper(mapper);

	this->CurrentRenderer->AddActor(actor);
	this->GetInteractor()->GetRenderWindow()->Render();
	this->HighlightProp(NULL);
}

void HighlightInteractorStyle::selectSinglePoint()
{
	int* pos = this->GetInteractor()->GetEventPosition();
	vtkSmartPointer<vtkPointPicker> picker = vtkSmartPointer<vtkPointPicker>::New();
	picker->SetTolerance(0.005);

	picker->Pick(pos[0], pos[1], 0, this->GetInteractor()->GetRenderWindow()->GetRenderers()->GetFirstRenderer());

//	std::cout << "point id is: " << picker->GetPointId() << std::endl;
	if (picker->GetPointId() == -1)
	{
		return;
	}
	vtkSmartPointer<vtkPolyData> pointset = vtkSmartPointer<vtkPolyData>::New();
	pointset->SetPoints(ugrid->GetPoints());

	vtkSmartPointer<vtkVertex>  v = vtkSmartPointer<vtkVertex>::New();
	v->GetPointIds()->SetId(0, picker->GetPointId());

	vtkSmartPointer<vtkCellArray> va = vtkSmartPointer<vtkCellArray>::New();
	va->InsertNextCell(v);

	pointset->SetVerts(va);


	vtkSmartPointer<vtkPolyDataMapper> mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
	vtkSmartPointer<vtkActor> actor = vtkSmartPointer<vtkActor>::New();

	mapper->SetInputData(pointset);
	mapper->ScalarVisibilityOff();

	actor->GetProperty()->SetColor(1.0, 0.0, 0.0);
	actor->GetProperty()->SetPointSize(3);
	actor->SetMapper(mapper);

	this->CurrentRenderer->AddActor(actor);
	this->GetInteractor()->GetRenderWindow()->Render();
	this->HighlightProp(NULL);
}
