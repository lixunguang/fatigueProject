#ifndef MESHVIEWER_H_
#define MESHVIEWER_H_

#include <QWidget>
#include <QAction>
#include <QComboBox>

#include "vtkDataSetMapper.h"
#include "QVTKWidget.h"
//#include "QVTKWidget2.h"
#include "vtkActor.h"
#include "vtkSmartPointer.h"
#include "vtkDataSet.h"
#include "vtkRenderWindow.h"
#include "vtkRenderer.h"
#include "vtkDataArray.h"
#include "vtkUnstructuredGrid.h"
#include "vtkCellData.h"
#include "vtkPointData.h"
#include "vtkProperty.h"
#include "vtkCamera.h"
#include "vtkOrientationMarkerWidget.h"
#include "vtkAxesActor.h"
#include "vtkPolyData.h"
#include "vtkTooltipItem.h"
#include "vtkAreaPicker.h"
#include "vtkSmartPointer.h"
#include "vtkRenderWindowInteractor.h"
#include "HighlightInteractorStyle.h"


#include <QTreeWidgetItem>

#include "mesh.h"

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
{//i do not know why there have a bug when us qvtkwidget2:can not select use key "r"
	Q_OBJECT
public:
	MeshViewer(QWidget *parent = 0);
	~MeshViewer();

public:
	void updateUi(QMap<QString, QString> &mapData);
	void loadMeshData(QString fileName);
	void reset();

public slots:
	void setBackground(const QColor color);
	void renderWindowEx();

	void showNodeLabel(QTreeWidgetItem *item);
	void hideNodeLabel(QTreeWidgetItem *item);

	void showElemLabel(QTreeWidgetItem *item);
	void hideElemLabel(QTreeWidgetItem *item);

	QSet<int> getSelectNodes();
	QSet<int> getSelectElems();
	void getSecletion(SelectType type, QList<int>& s);

	void selectTypeChanged(SelectType type);
	void setSelectType(SelectType type);
	SelectType getSelectType();

	void resetActor();
	void getActorColor(double* color);
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
	void addOrientationMarkerWidget();
	void createToolBar();

// signals:
// 	void finishSelectNodes(QSet<int> nodes);
// 	void finishSelectElems(QSet<int> elems);

public:
 	QAction *action_reset;
 	QAction *action_viewTop;
 	QAction *action_viewBottom;
 	QAction *action_viewLeft;
 	QAction *action_viewRight;
 	QAction *action_viewFront;
 	QAction *action_viewBack;
 	QAction *action_viewMore;
 	QComboBox *reprsentationComboBox; 

	vtkOrientationMarkerWidget * OMwidget;
	vtkActor* mainActor;


	Mesh *mesh;

	map<QString, vtkActor*> nodeActorMap;
	map<QString, vtkActor*> elemActorMap;

	SelectType selectType;
	QSet<int> currentNodes;
	QSet<int> currentElems;
};
#endif