#include "vtkInteractorStyleRubberBandPick.h"
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

#include "HighlightInteractorStyle.h"

vtkStandardNewMacro(HighlightInteractorStyle);


void HighlightInteractorStyle::OnLeftButtonUp()
{
	// Forward events
	vtkInteractorStyleRubberBandPick::OnLeftButtonUp();
	selectTest();	return;
	selectPoint();
	selectCell();

	InvokeEvent(12346);
	if (d.size() > 4)
	{
		d.pop_front();
		d.pop_front();
	}
	int* pos = this->GetInteractor()->GetEventPosition();
	d.push_back(pos[0]);
	d.push_back(pos[1]);

	
	//selectPoint();
	if (d.size() >= 4)
	{
		int prepos[2];
		prepos[0] = d.at(0);
		prepos[1] = d.at(1);
		
		int p[2];
		p[0] = pos[0];
		p[1] = pos[1];
		selectArea(p, prepos);
	}
	
	//selectCircle();
}

void HighlightInteractorStyle::selectCircle(int* pos,int *prepos)
{
	vtkSmartPointer<vtkHardwareSelector> selector = vtkSmartPointer<vtkHardwareSelector>::New();
	selector->SetRenderer(this->GetInteractor()->GetRenderWindow()->GetRenderers()->GetFirstRenderer());

	float p1[3] = { 0 };
	float p2[3] = { 0 };
	p1[0] = (float)pos[0]; p1[1] = (float)pos[1];
	p2[0] = (float)prepos[0]; p2[1] = (float)prepos[1];
	double dist = sqrt(vtkMath::Distance2BetweenPoints(p1,p2));

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

	for (int i = 0; i < 4;i++)
	{
		if (windowSize[i]<0)
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
#include "vtkHardwareSelector.h"
#include "vtkNew.h"
void HighlightInteractorStyle::selectArea(int* pos, int *prepos)
{

	float p1[3] = { 0 };
	float p2[3] = { 0 };
	p1[0] = (float)pos[0]; p1[1] = (float)pos[1];
	p2[0] = (float)prepos[0]; p2[1] = (float)prepos[1];
	double dist = sqrt(vtkMath::Distance2BetweenPoints(p1, p2));

	vtkSmartPointer<vtkAreaPicker> picker = vtkSmartPointer<vtkAreaPicker>::New();
	this->GetInteractor()->SetPicker(picker);
	std::cout << "current pos: " << pos[0] <<"  "<< pos[1] << std::endl;
	std::cout << "AreaPick: " << pos[0] - dist << "  " << pos[1] - dist << "  " << pos[0] + dist << "  " << pos[1] + dist << std::endl;
//	picker->AreaPick(pos[0] - dist, pos[1] - dist, pos[0] + dist, pos[1] + dist, this->GetInteractor()->GetRenderWindow()->GetRenderers()->GetFirstRenderer());

	picker->AreaPick(0,0,400,400, this->GetInteractor()->GetRenderWindow()->GetRenderers()->GetFirstRenderer());

	vtkProp3DCollection *props = picker->GetProp3Ds();

	std::cout << "GetNumberOfItems " << props->GetNumberOfItems() << std::endl;
	for (vtkIdType i = 0; i < props->GetNumberOfItems(); i++)
	{
		vtkProp3D* prop = props->GetNextProp3D();
	 

		//std::cout << "Picked prop: " << prop << std::endl;
	}

 
//	props->Print(std::cout);
}
#include "vtkExtractGeometry.h"
void HighlightInteractorStyle::selectTest()
{
	if (this->CurrentMode == VTKISRBP_SELECT)
	{
		vtkPlanes* frustum = static_cast<vtkAreaPicker*>(this->GetInteractor()->GetPicker())->GetFrustum();

		vtkSmartPointer<vtkExtractGeometry> extractGeometry = vtkSmartPointer<vtkExtractGeometry>::New();
		extractGeometry->SetImplicitFunction(frustum);
		extractGeometry->SetInputData(this->ugrid);
		extractGeometry->Update();

		vtkSmartPointer<vtkVertexGlyphFilter> glyphFilter =	vtkSmartPointer<vtkVertexGlyphFilter>::New();
		glyphFilter->SetInputConnection(extractGeometry->GetOutputPort());
		glyphFilter->Update();

		vtkPolyData* selected = glyphFilter->GetOutput();
		std::cout << "Selected " << selected->GetNumberOfPoints() << " points." << std::endl;
		std::cout << "Selected " << selected->GetNumberOfCells() << " cells." << std::endl;

		this->SelectedMapper->SetInputData(selected);
		this->SelectedMapper->ScalarVisibilityOff();
	
		this->SelectedActor->GetProperty()->SetColor(1.0, 0.0, 0.0); //(R,G,B)
		this->SelectedActor->GetProperty()->SetPointSize(3);

		this->CurrentRenderer->AddActor(SelectedActor);
		this->GetInteractor()->GetRenderWindow()->Render();
		this->HighlightProp(NULL);

		return;


	}
}

void HighlightInteractorStyle::selectCell()
{
	int* pos = this->GetInteractor()->GetEventPosition();
	vtkSmartPointer<vtkCellPicker> picker = vtkSmartPointer<vtkCellPicker>::New();
	picker->SetTolerance(0.005);


	picker->Pick(pos[0], pos[1], 0, this->GetInteractor()->GetRenderWindow()->GetRenderers()->GetFirstRenderer());

	std::cout << "Cell id is: " << picker->GetCellId() << std::endl;

	if (picker->GetCellId() == -1)
	{
		return;
	}


	vtkSmartPointer<vtkPolyData> cellset = vtkSmartPointer<vtkPolyData>::New();
	cellset->SetPoints(ugrid->GetPoints());


	vtkSmartPointer<vtkCellArray> va = vtkSmartPointer<vtkCellArray>::New();
	va->InsertNextCell(ugrid->GetCell(picker->GetCellId()));

	cellset->SetPolys(va);

	this->SelectedMapper->SetInputData(cellset);
	this->SelectedMapper->ScalarVisibilityOff();

	this->SelectedActor->GetProperty()->SetColor(1.0, 0.0, 0.0); //(R,G,B)
	this->SelectedActor->GetProperty()->SetPointSize(5);


	this->GetInteractor()->GetRenderWindow()->GetRenderers()->GetFirstRenderer()->AddActor(SelectedActor);
	this->GetInteractor()->GetRenderWindow()->Render();
	this->HighlightProp(NULL);

}

void HighlightInteractorStyle::selectPoint()
{
	int* pos = this->GetInteractor()->GetEventPosition();
	vtkSmartPointer<vtkPointPicker> picker = vtkSmartPointer<vtkPointPicker>::New();
	picker->SetTolerance(0.005);


	picker->Pick(pos[0], pos[1], 0, this->GetInteractor()->GetRenderWindow()->GetRenderers()->GetFirstRenderer());

	std::cout << "point id is: " << picker->GetPointId() << std::endl;
	if (picker->GetPointId()==-1)
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

	this->SelectedMapper->SetInputData(pointset);
	this->SelectedMapper->ScalarVisibilityOff();

	this->SelectedActor->GetProperty()->SetColor(1.0, 0.0, 0.0); //(R,G,B)
	this->SelectedActor->GetProperty()->SetPointSize(5);


	this->GetInteractor()->GetRenderWindow()->GetRenderers()->GetFirstRenderer()->AddActor(SelectedActor);
	this->GetInteractor()->GetRenderWindow()->Render();
	this->HighlightProp(NULL);
}