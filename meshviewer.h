#ifndef MeshViewer_H_
#define MeshViewer_H_

#include <QWidget>
#include <QTimer>
#include <QString>
#include <QStringList>
#include <QMap>
#include <QIcon>
#include <QAction>
#include <QVector>
#include <QComboBox>

#include <vtkDataSetMapper.h>
#include <QVTKWidget.h>
#include <vtkActor.h>
#include <vtkActor2D.h>
#include <vtkScalarBarActor.h>
#include <vtkLookupTable.h>
#include <vtkSmartPointer.h>
#include <vtkDataSet.h>
#include <vtkRenderWindow.h>
#include <vtkRenderer.h>
#include <vtkDataArray.h>
#include <vtkUnstructuredGrid.h>
#include <vtkCellData.h>
#include <vtkPointData.h>
#include <vtkProperty.h>
#include <vtkCamera.h>
#include <vtkOrientationMarkerWidget.h>
#include <vtkMultiBlockDataSet.h>
#include <vtkAxesActor.h>
#include <vtkMath.h>
#include <vtkArrowSource.h>
#include <vtkPolyData.h>
#include <vtkTransform.h>
#include <vtkDoubleArray.h>
#include <vtkWarpVector.h>
#include <vtkExtractGrid.h>
#include <vtkWarpScalar.h>
#include <vtkLogLookupTable.h>
#include <vtkPolyDataNormals.h>


struct struct_tensor 
{
	struct_tensor(double _xx = 0.0,
		double _xy = 0.0,
		double _xz = 0.0,
		double _yy = 0.0,
		double _yz = 0.0,
		double _zz = 0.0)
		:xx(_xx),
		xy(_xy),
		xz(_xz),
		yy(_yy),
		yz(_yz),
		zz(_zz)
	{

	}
	double xx;
	double xy;
	double xz;
	double yy;
	double yz;
	double zz;
};

class  MeshViewer : public QVTKWidget
{
	Q_OBJECT
public:
	MeshViewer(QWidget *parent = 0);
	~MeshViewer();

public:
	void createToolBar();

public slots:
	void setBackground(const QColor color);
	
private slots:
	void viewTop();
	void viewBottom();
	void viewLeft();
	void viewRight();
	void viewFront();
	void viewBack();
	void viewReset();

	void reprsentationComboBoxIndexChanged(int index);

private:
	void display(QString theName);

	void renderWindow();
	void showOrientationMarkerWidget(bool isShow);
	void addOrientationMarkerWidget();


private:
 	vtkOrientationMarkerWidget * OMwidget;
 	vtkSmartPointer<vtkRenderer>		renderer;
 	vtkSmartPointer<vtkActor>			mainActor;
 	vtkSmartPointer<vtkDataSetMapper>	mapper;
 	vtkSmartPointer<vtkLookupTable>		lut;
	vtkSmartPointer<vtkScalarBarActor>	scalarBar;
 	vtkSmartPointer<vtkUnstructuredGrid> geometryData;
 	vtkSmartPointer<vtkActor> warpActor;

 	QAction *action_reset;
 	QAction *action_viewTop;
 	QAction *action_viewBottom;
 	QAction *action_viewLeft;
 	QAction *action_viewRight;
 	QAction *action_viewFront;
 	QAction *action_viewBack;
 	QAction *action_viewMore;
 	QComboBox *reprsentationComboBox; 


};
#endif