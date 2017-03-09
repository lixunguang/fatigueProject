
#ifndef HIGHLIGHTINTERACTORSTYLE_H
#define HIGHLIGHTINTERACTORSTYLE_H

#include <QDebug>

#include <deque>  

#include "vtkActor.h"
#include "vtkSmartPointer.h"
#include "vtkUnstructuredGrid.h"
#include "vtkPolyDataMapper.h"
#include "vtkInteractorStyleRubberBandPick.h"


#include "typedef.h"

#define VTKISRBP_ORIENT 0
#define VTKISRBP_SELECT 1

// Define interaction style
class MeshViewer;
class HighlightInteractorStyle : public vtkInteractorStyleRubberBandPick
{
public:
	static HighlightInteractorStyle* New();
	vtkTypeMacro(HighlightInteractorStyle, vtkInteractorStyleRubberBandPick);

	HighlightInteractorStyle();
	~HighlightInteractorStyle();

	virtual void OnLeftButtonUp();
	virtual void OnChar();
	void SetData(vtkUnstructuredGrid* data);
	vtkUnstructuredGrid* GetData();

	void setSelect(Select_Type type);

	QSet<int>& getCurrentSelectNodes();
	QSet<int>& getCurrentSelectElems();

	void SetMeshViewer(MeshViewer*);

private:
	void selectAreaPoints();
	void selectAreaCells();
	void selectSingleCell();
	void selectSinglePoint();
	void selectTest();
	void selectCircle(int* pos, int *prepos);
	void createPointsMap();
public:
	vtkUnstructuredGrid* ugrid;

	vtkSmartPointer<vtkActor> SelectedActor;
	vtkSmartPointer<vtkPolyDataMapper> SelectedMapper;

	float circleR;
	std::deque<int> d;

	QSet<int> currentSelectNodes;
	QSet<int> currentSelectElems;
	QMap<int, int> nodeIdMap;

	Select_Type selectType;

	MeshViewer *meshViewer;
};


#endif