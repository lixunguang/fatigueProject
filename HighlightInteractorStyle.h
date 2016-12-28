
#ifndef HIGHLIGHTINTERACTORSTYLE_H
#define HIGHLIGHTINTERACTORSTYLE_H

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
#include <vtkExtractUnstructuredGrid.h>
#include <vtkActorCollection.h>
#include <vtkCellArray.h>
#include <vtkPoints.h>
#include <vtkPolyData.h>
#include <vtkVertex.h>
#include <vtkCellPicker.h>
#include <vtkPointPicker.h>
#include <QDebug>

#include <deque>  

#define VTKISRBP_ORIENT 0
#define VTKISRBP_SELECT 1

// Define interaction style
class HighlightInteractorStyle : public vtkInteractorStyleRubberBandPick
{
public:
	static HighlightInteractorStyle* New();
	vtkTypeMacro(HighlightInteractorStyle, vtkInteractorStyleRubberBandPick);

	HighlightInteractorStyle() : vtkInteractorStyleRubberBandPick()
	{
		this->SelectedMapper = vtkSmartPointer<vtkPolyDataMapper>::New();
		this->SelectedActor = vtkSmartPointer<vtkActor>::New();
		this->SelectedActor->SetMapper(SelectedMapper);

		circleR = 12;
	}

	virtual void OnLeftButtonUp();
	
	void SetData(vtkSmartPointer<vtkUnstructuredGrid> data) 
	{ 
		this->ugrid = data;
	}

	vtkUnstructuredGrid* GetData() { return this->ugrid; }

private:
	void selectArea(int* pos, int *prepos);
	void selectCell();
	void selectPoint();
	void selectTest();
	void selectCircle(int* pos, int *prepos);

public:
	vtkSmartPointer<vtkUnstructuredGrid> ugrid;

	vtkSmartPointer<vtkActor> SelectedActor;
	vtkSmartPointer<vtkPolyDataMapper> SelectedMapper;


	float circleR;
	std::deque<int> d;
};


#endif