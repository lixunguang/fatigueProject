
#ifndef HIGHLIGHTINTERACTORSTYLE_H
#define HIGHLIGHTINTERACTORSTYLE_H

#include <QDebug>

#include <deque>  

#include "vtkActor.h"
#include "vtkSmartPointer.h"
#include "vtkUnstructuredGrid.h"
#include "vtkPolyDataMapper.h"
#include "vtkInteractorStyleRubberBandPick.h"



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