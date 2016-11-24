
#ifndef WIDGET3D_H
#define WIDGET3D_H

#include <QDebug>
#include <QTimer>
#include "QVTKWidget.h"
#include "vtkUnstructuredGrid.h"
#include "vtkCellData.h"
#include "vtkDoubleArray.h"
#include "vtkDataSetMapper.h"

#include <vtkSmartPointer.h>
#include <vtkSphereSource.h>
#include <vtkPolyDataMapper.h>
#include <vtkActor.h>
#include <vtkRenderWindow.h>
#include <vtkRenderer.h>
#include "vtkProperty.h"
#include <vtkTriangle.h>  
#include <vtkDataArray.h>  
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

//#include "HighlightInteractorStyle.h"

#include "Python.h"






class Widget3D :public QVTKWidget
{
    Q_OBJECT
public:
	Widget3D(QWidget *parent = 0) :QVTKWidget(parent)
	{
	}

	~Widget3D(){}

 

public slots :
	void update();


};

#endif