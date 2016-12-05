
#include "HighlightInteractorStyle.h"


vtkStandardNewMacro(HighlightInteractorStyle);


void HighlightInteractorStyle::OnLeftButtonUp()
{
	// Forward events
	vtkInteractorStyleRubberBandPick::OnLeftButtonUp();

	selectPoint();
	selectCell();
}

void HighlightInteractorStyle::selectTest()
{
	if (this->CurrentMode == VTKISRBP_SELECT)
	{
	//	vtkPlanes* frustum = static_cast<vtkAreaPicker*>(this->GetInteractor()->GetPicker())->GetFrustum();

		vtkSmartPointer<vtkExtractUnstructuredGrid> extractPolyUnstructuredGrid = vtkSmartPointer<vtkExtractUnstructuredGrid>::New();
#if VTK_MAJOR_VERSION <= 5
		extractPolyDataGeometry->SetInput(this->PolyData);
#else
		extractPolyUnstructuredGrid->SetInputData(this->ugrid);
#endif
		extractPolyUnstructuredGrid->PointClippingOn();
		extractPolyUnstructuredGrid->SetPointMinimum(20);
		extractPolyUnstructuredGrid->SetPointMaximum(250);

		//extractPolyUnstructuredGrid->CellClippingOn();
		//extractPolyUnstructuredGrid->SetCellMinimum(20);
		//extractPolyUnstructuredGrid->SetCellMaximum(50);

		extractPolyUnstructuredGrid->Update();

		vtkSmartPointer<vtkPolyData> pointset = vtkSmartPointer<vtkPolyData>::New();
		pointset->SetPoints(ugrid->GetPoints());
		pointset->Print(std::cout);

		vtkSmartPointer<vtkVertex>  v = vtkSmartPointer<vtkVertex>::New();
		v->GetPointIds()->SetId(0, 0);
		vtkSmartPointer<vtkCellArray> va = vtkSmartPointer<vtkCellArray>::New();


		va->InsertNextCell(v);
		pointset->SetVerts(va);

#if VTK_MAJOR_VERSION <= 5
		this->SelectedMapper->SetInputConnection(
			extractPolyDataGeometry->GetOutputPort());
#else
		//this->SelectedMapper->SetInputData(extractPolyUnstructuredGrid->GetOutput());
#endif

		this->SelectedMapper->SetInputData(pointset);
		this->SelectedMapper->ScalarVisibilityOff();

		// 			vtkSmartPointer<vtkUnstructuredGrid> grid = extractPolyUnstructuredGrid->GetOutput();
		// 			vtkSmartPointer<vtkCellArray> cells = grid->GetCells();
		// 			qDebug() << cells->GetNumberOfCells();
		// 
		// 			cells->Print(std::cout);
		// 
		vtkSmartPointer<vtkPoints> pts = ugrid->GetPoints();
		qDebug() << pts->GetNumberOfPoints();
		pts->Print(std::cout);

		this->SelectedActor->GetProperty()->SetColor(1.0, 0.0, 0.0); //(R,G,B)
		this->SelectedActor->GetProperty()->SetPointSize(5);

		// 			vtkSmartPointer< vtkActorCollection> acs = this->GetInteractor()->GetRenderWindow()->GetRenderers()->GetFirstRenderer()->GetActors();
		// 			qDebug()<< acs->GetNumberOfItems();
		// 
		this->GetInteractor()->GetRenderWindow()->GetRenderers()->GetFirstRenderer()->AddActor(SelectedActor);
		// 
		// 			acs = this->GetInteractor()->GetRenderWindow()->GetRenderers()->GetFirstRenderer()->GetActors();
		// 			qDebug() << acs->GetNumberOfItems();

		this->GetInteractor()->GetRenderWindow()->Render();
		this->HighlightProp(NULL);
	}
}

void HighlightInteractorStyle::selectCell()
{
	int* pos = this->GetInteractor()->GetEventPosition();
	vtkSmartPointer<vtkCellPicker> picker = vtkSmartPointer<vtkCellPicker>::New();
	picker->SetTolerance(0.005);


	picker->Pick(pos[0], pos[1], 0, this->GetInteractor()->GetRenderWindow()->GetRenderers()->GetFirstRenderer());

	std::cout << "Cell id is: " << picker->GetCellId() << std::endl;


}

void HighlightInteractorStyle::selectPoint()
{
	int* pos = this->GetInteractor()->GetEventPosition();
	vtkSmartPointer<vtkPointPicker> picker = vtkSmartPointer<vtkPointPicker>::New();
	picker->SetTolerance(0.005);


	picker->Pick(pos[0], pos[1], 0, this->GetInteractor()->GetRenderWindow()->GetRenderers()->GetFirstRenderer());

	std::cout << "point id is: " << picker->GetPointId() << std::endl;
}