#include "mesh.h"

#include <vtksys/stl/string>
#include <vtksys/SystemTools.hxx>
#include <vtksys/RegularExpression.hxx>
#include <vtkProperty.h>
#include <vtkPolyDataMapper.h>
#include <vtkAppendPolyData.h>
#include <vtkPointData.h>
#include <vtkCellData.h>
#include <vtkCell.h>
#include <vtkPolyData.h>
#include <vtkFloatArray.h>

#include <vtkPoints.h>
#include <vtkVertex.h>
#include <vtkLine.h>
#include <vtkTriangle.h>
#include <vtkQuadraticTriangle.h>
#include <vtkQuad.h>
#include <vtkQuadraticQuad.h>
#include <vtkHexagonalPrism.h>
#include <vtkHexahedron.h>
#include <vtkQuadraticHexahedron.h>
#include <vtkPentagonalPrism.h>
#include <vtkPolyhedron.h>
#include <vtkPyramid.h>
#include <vtkTetra.h>
#include <vtkQuadraticTetra.h>
#include <vtkVoxel.h>
#include <vtkWedge.h>
#include <vtkQuadraticWedge.h>
#include <vtkPolygon.h>


#include <vtkInformation.h>
#include <vtkInformationVector.h>
#include <vtkObjectFactory.h>
#include <vtkMergePoints.h>
#include <vtkMultiBlockDataSet.h>
#include <vtkRendererCollection.h>
#include <vtkRenderWindow.h>
#include <vtkPropAssembly.h>
#include <vtkAssembly.h>
#include <vtkActorCollection.h>
#include <vtkTransform.h>
#include <vtkTransformPolyDataFilter.h>

#include <QDebug>
Mesh::Mesh()
{
	ugrid = vtkSmartPointer<vtkUnstructuredGrid>::New();
	qDebug() << ugrid->GetReferenceCount();

	vtkSmartPointer<vtkUnstructuredGrid> x = ugrid;
	qDebug() << ugrid->GetReferenceCount();
}

void Mesh::setFileName(char *fileName)
{

	datafun = vdm_DataFunBegin();
	ReadVKISupportFile(fileName,datafun);

	vdm_DataFunGetLibrary(datafun,&lib);

	lman = vdm_LManBegin();
	vdm_LManSetObject(lman,VDM_DATAFUN,datafun);

	model = vis_ModelBegin();
	vdm_LManLoadModel (lman,model);

	vis_ModelGetObject (model,VIS_CONNECT,(Vobject**)&connect);

	gridfun = vis_GridFunBegin ();
	vis_ConnectGridFun (connect,gridfun);

	// Get number of points
	vis_ConnectNumber(connect, SYS_NODE, &n_nodes);

	// Get number of elements
	vis_ConnectNumber(connect, SYS_ELEM, &n_elements);

}

void Mesh::readMesh()
{
	vtkPoints *pts = vtkPoints::New();
	pts->Allocate(n_nodes);
	pts->SetNumberOfPoints(n_nodes);

	ugrid->SetPoints(pts);
	ugrid->Allocate(n_elements);

	int featype;
	int nid, cid;
	double coord[3];
	for (int i = 1; i <= n_nodes; i++)
	{
		vis_ConnectCoordsdv(connect, 1, &i, (double(*)[3])coord);
		vis_ConnectNodeAssoc(connect, VIS_USERID, 1, &i, &nid);
		vis_ConnectNodeAssoc(connect, VIS_CSYSID, 1, &i, &cid);
		// Insert points to UnstrcutredGrid with index based 0
		pts->InsertPoint(i - 1, coord);

		nid_map.insert(std::pair<int, int>(nid, i - 1));

	}

	int max_elem_node;
	vis_ConnectMaxElemNode(connect, &max_elem_node);

	int eid, pid, mid, partid;
	int nix, *ix, *ux;
	int shape, maxi, maxj, maxk;

	ix = new int[max_elem_node];
	ux = new int[max_elem_node];
	char externala[5] = { 0 }, externalb[5] = { 0 };
	int j = 0;
	for (int i = 1; i <= n_elements; i++)
	{
		vis_ConnectTopology(connect, i, &shape, &maxi, &maxj, &maxk);
		vis_ConnectElemNode(connect, i, &nix, ix);
		vis_ConnectElemAssoc(connect, VIS_USERID, 1, &i, &eid);
		vis_ConnectElemAssoc(connect, VIS_PARTID, 1, &i, &partid);
		vis_ConnectElemAssoc(connect, VIS_PROPID, 1, &i, &pid);
		vis_ConnectElemAssoc(connect, VIS_MATLID, 1, &i, &mid);
		vis_ConnectElemAssoc(connect, VIS_CSYSID, 1, &i, &cid);
		vis_ConnectElemAssoc(connect, VIS_FEATYPE, 1, &i, &featype);
		vis_ConnectElemAssoc(connect, VIS_EXTNAMEA, 1, &i, (Vint*)externala);
		vis_ConnectElemAssoc(connect, VIS_EXTNAMEB, 1, &i, (Vint*)externalb);

		bool valid_ele = true;


		if (shape == SYS_SHAPEPOINT) // Vertex
		{
			// How to handle Point here? Not sure correct or not. Fixed me
			vtkSmartPointer<vtkVertex> vertex = vtkSmartPointer<vtkVertex>::New();
			if (strcmp(externala, "MPC") == 0 && strcmp(externalb, "184") == 0)
			{
				for (int j = 0; j < nix - 1; j++)	// nix == 2
				{
					vertex->GetPointIds()->SetId(j, ix[j] - 1);
				}
			}
			else
			{
				for (int j = 0; j < nix; j++)	// nix == 2
				{
					vertex->GetPointIds()->SetId(j, ix[j] - 1);
				}
			}

			ugrid->InsertNextCell(vertex->GetCellType(), vertex->GetPointIds());
		}
		else if (shape == SYS_SHAPELINE)		// Line
		{
			vtkSmartPointer<vtkLine> line = vtkSmartPointer<vtkLine>::New();
			if (strcmp(externala, "BEAM") == 0 && strcmp(externalb, "188") == 0 && nix == 3)
			{
				for (int j = 0; j < nix - 1; j++)	// nix == 2
				{
					line->GetPointIds()->SetId(j, ix[j] - 1);
				}
			}
			else
			{
				for (int j = 0; j < nix; j++)	// nix == 2j,
				{
					line->GetPointIds()->SetId(j, ix[j] - 1);
				}
			}

			// Add to UnstructuredGrid
			ugrid->InsertNextCell(line->GetCellType(), line->GetPointIds());
		}
		else if (shape == SYS_SHAPETRI)		// Triangle
		{
			if (nix == 3)	// ctria3
			{
				vtkSmartPointer<vtkTriangle> triangle = vtkSmartPointer<vtkTriangle>::New();
				for (int j = 0; j < nix; j++)
				{
					triangle->GetPointIds()->SetId(j, ix[j] - 1);
				}
				ugrid->InsertNextCell(triangle->GetCellType(), triangle->GetPointIds());
			}
			else if (nix == 6)	// Ctria6
			{
				vtkSmartPointer<vtkQuadraticTriangle> quadtriangle = vtkSmartPointer<vtkQuadraticTriangle>::New();
				for (int j = 0; j < nix; j++)
				{
					quadtriangle->GetPointIds()->SetId(j, ix[j] - 1);
				}
				ugrid->InsertNextCell(quadtriangle->GetCellType(), quadtriangle->GetPointIds());
			}
			else
			{
				//vtkErrorMacro(<< "Warn: Triangle has wrong number of points: " << nix);
			}
		}
		else if (shape == SYS_SHAPEQUAD)	// Quadrilateral
		{
			if (nix == 4)	// QUAD4
			{
				vtkSmartPointer<vtkQuad> quad = vtkSmartPointer<vtkQuad>::New();
				for (int j = 0; j < nix; j++)
				{
					quad->GetPointIds()->SetId(j, ix[j] - 1);
				}
				ugrid->InsertNextCell(quad->GetCellType(), quad->GetPointIds());
			}
			else if (nix == 8)	// QUAD8
			{
				vtkSmartPointer<vtkQuadraticQuad> quad_quad = vtkSmartPointer<vtkQuadraticQuad>::New();
				for (int j = 0; j < nix; j++)
				{
					quad_quad->GetPointIds()->SetId(j, ix[j] - 1);
				}
				ugrid->InsertNextCell(quad_quad->GetCellType(), quad_quad->GetPointIds());
			}
			else
			{
				//vtkErrorMacro(<< "Warn: Quadrilateral has wrong number of points: " << nix);
			}
		}
		else if (shape == SYS_SHAPETET)	// Tetrahedron
		{
			if (nix == 4)	// TETRA4
			{
				vtkSmartPointer<vtkTetra> tetra = vtkSmartPointer<vtkTetra>::New();
				for (int j = 0; j < nix; j++)
				{
					tetra->GetPointIds()->SetId(j, ix[j] - 1);
				}
				ugrid->InsertNextCell(tetra->GetCellType(), tetra->GetPointIds());
			}
			else if (nix == 10)	// TETRA10
			{
				vtkSmartPointer<vtkQuadraticTetra> quad_tetra = vtkSmartPointer<vtkQuadraticTetra>::New();
				for (int j = 0; j < nix; j++)
				{
					quad_tetra->GetPointIds()->SetId(j, ix[j] - 1);
				}
				ugrid->InsertNextCell(quad_tetra->GetCellType(), quad_tetra->GetPointIds());
			}
			else
			{
				//vtkErrorMacro(<< "Warn: Tetrahedron has wrong number of points: " << nix);
			}
		}
		else if (shape == SYS_SHAPEPYR)	// Pyramid
		{
			if (nix == 5)
			{
				vtkSmartPointer<vtkPyramid> pyramid = vtkSmartPointer<vtkPyramid>::New();
				for (int j = 0; j < nix; j++)
				{
					pyramid->GetPointIds()->SetId(j, ix[j] - 1);
				}
				ugrid->InsertNextCell(pyramid->GetCellType(), pyramid->GetPointIds());
			}
			else
			{
				//vtkErrorMacro(<< "Warn: Pyramid has wrong number of points: " << nix);
			}

		}
		else if (shape == SYS_SHAPEWED)	// Pentahedron 
		{
			if (nix == 6)	// PENTA6
			{
				vtkSmartPointer<vtkWedge> wedge = vtkSmartPointer<vtkWedge>::New();
				for (int j = 0; j < nix; j++)
				{
					wedge->GetPointIds()->SetId(j, ix[j] - 1);
				}
				ugrid->InsertNextCell(wedge->GetCellType(), wedge->GetPointIds());
			}
			else if (nix == 15)	// PENTA15
			{
				vtkSmartPointer<vtkQuadraticWedge> quad_wedge = vtkSmartPointer<vtkQuadraticWedge>::New();
				for (int j = 0; j < nix; j++)
				{
					quad_wedge->GetPointIds()->SetId(j, ix[j] - 1);
				}
				ugrid->InsertNextCell(quad_wedge->GetCellType(), quad_wedge->GetPointIds());
			}
			else
			{
				//vtkErrorMacro(<< "Warn: Pentahedron has wrong number of points: " << nix);
			}
		}
		else if (shape == SYS_SHAPEHEX)	// Hexahedron
		{
			if (nix == 8)	// HEXA8
			{
				vtkSmartPointer<vtkHexahedron> hexa = vtkSmartPointer<vtkHexahedron>::New();
				for (int j = 0; j < nix; j++)
				{
					hexa->GetPointIds()->SetId(j, ix[j] - 1);
				}
				ugrid->InsertNextCell(hexa->GetCellType(), hexa->GetPointIds());
			}
			else if (nix == 20)	// HEXA20
			{
				vtkSmartPointer<vtkQuadraticHexahedron> quad_hexa = vtkSmartPointer<vtkQuadraticHexahedron>::New();
				for (int j = 0; j < nix; j++)
				{
					quad_hexa->GetPointIds()->SetId(j, ix[j] - 1);
				}
				ugrid->InsertNextCell(quad_hexa->GetCellType(), quad_hexa->GetPointIds());
			}
			else
			{
				//vtkErrorMacro(<< "Warn: Hexahedron has wrong number of points: " << nix);
			}
		}
		else if (shape == SYS_SHAPEPOLYGON) // Polygon
		{
			vtkSmartPointer<vtkPolygon> polygon = vtkSmartPointer<vtkPolygon>::New();
			polygon->GetPointIds()->SetNumberOfIds(nix);
			for (int j = 0; j < nix; j++)
			{
				polygon->GetPointIds()->SetId(j, ix[j] - 1);
			}

			ugrid->InsertNextCell(polygon->GetCellType(), polygon->GetPointIds());
		}
		else if (shape == SYS_SHAPEPOLYHED)	// Polyhedron : Supported from VTK 5.8.0
		{
			vtkSmartPointer<vtkPolyhedron> polyhedron = vtkSmartPointer<vtkPolyhedron>::New();
			polyhedron->GetPointIds()->SetNumberOfIds(nix);
			for (int j = 0; j < nix; j++)
			{
				polyhedron->GetPointIds()->SetId(j, ix[j] - 1);
			}

			ugrid->InsertNextCell(polyhedron->GetCellType(), polyhedron->GetPointIds());
		}
		else
		{
			valid_ele = false;
		}

		// Store element id to index mapping
		if (valid_ele)
		{
			eid_map.insert(std::pair<int, int>(eid, j++));
		}

	}

	delete[] ix;
	delete[] ux;

}

void Mesh::loadData(char *fileName)
{
	setFileName(fileName);
	readMesh();

	emit finishDataLoaded();
}

Mesh::~Mesh()
{
	qDebug() << ugrid->GetReferenceCount();
	vis_GridFunEnd(gridfun);
	vis_ModelEnd(model);
	vdm_LManEnd(lman);
	CloseVKISupportFile(datafun);
	qDebug() << ugrid->GetReferenceCount();
}


void Mesh::ReadVKISupportFile(Vchar *fileName, vdm_DataFun *datafun)
{
	vdm_NASFil  *nasfil;
	vdm_NASLib  *naslib;
	vdm_NatLib  *natlib;
	vdm_SDRCLib *sdrclib;
	vdm_ANSFil  *ansfil;
	vdm_ANSLib  *anslib;
	vdm_ABAFil  *abafil;
	vdm_ABALib  *abalib;
	vdm_D3DFil  *d3dfil;
	vdm_D3DLib  *d3dlib;
	vdm_H3DLib  *h3dlib;
	vdm_HMAFil  *hmafil;
	vdm_STLFil  *stlfil;
	vdm_OBJFil  *objfil;
	vdm_FDILib  *fdilib;
	vdm_PLOT3DLib  *plot3dlib;
	vdm_FLUENTLib  *fluentlib;
	vdm_EnSightLib *ensightlib;
	vdm_STARCCMLib *starccmlib;
	vdm_TecplotLib *tecplotlib;
	vdm_MarcLib   *marclib;
	vdm_CGNSVLib  *cgnsvlib;
	vdm_PatLib  *patlib;
	vdm_RASLib  *raslib;
	vdm_FEMAPLib *femaplib;
	vdm_AUTODYNLib *autodynlib;
	vdm_PAMLib *pamlib;
	vdm_POLYFLOWLib *polyflowlib;
	vdm_OpenFOAMLib *openfoamlib;
	vdm_COMSOLLib *comsollib;
	vdm_PAMFil  *pamfil;
	vdm_CFXLib  *cfxlib;
	vdm_GMVLib  *gmvlib;
	vdm_PERMASLib  *permaslib;

	//set file type
	GetFileType(fileName);

	if (filetype == SYS_SDRC_UNIVERSAL) {
		sdrclib = vdm_SDRCLibBegin();
		vdm_SDRCLibDataFun(sdrclib, datafun);
	}
	else if (filetype == SYS_NASTRAN_BULKDATA) {
		nasfil = vdm_NASFilBegin();
		vdm_NASFilDataFun(nasfil, datafun);
	}
	else if (filetype == SYS_NASTRAN_OUTPUT2 ||
		filetype == SYS_NASTRAN_XDB) {
		naslib = vdm_NASLibBegin();
		vdm_NASLibDataFun(naslib, datafun);
	}
	else if (filetype == SYS_NATIVE) {
		natlib = vdm_NatLibBegin();
		vdm_NatLibDataFun(natlib, datafun);
	}
	else if (filetype == SYS_ANSYS_INPUT) {
		ansfil = vdm_ANSFilBegin();
		vdm_ANSFilDataFun(ansfil, datafun);
	}
	else if (filetype == SYS_ANSYS_RESULT) {
		anslib = vdm_ANSLibBegin();
		vdm_ANSLibDataFun(anslib, datafun);
	}
	else if (filetype == SYS_ABAQUS_INPUT) {
		abafil = vdm_ABAFilBegin();
		vdm_ABAFilDataFun(abafil, datafun);
	}
	else if (filetype == SYS_ABAQUS_FIL ||
		filetype == SYS_ABAQUS_ODB) {
		abalib = vdm_ABALibBegin();
		vdm_ABALibDataFun(abalib, datafun);
	}
	else if (filetype == SYS_LSTC_INPUT) {
		d3dfil = vdm_D3DFilBegin();
		vdm_D3DFilDataFun(d3dfil, datafun);
	}
	else if (filetype == SYS_LSTC_STATE ||
		filetype == SYS_LSTC_STATEFEMZIP ||
		filetype == SYS_LSTC_HISTORY) {
		d3dlib = vdm_D3DLibBegin();
		vdm_D3DLibDataFun(d3dlib, datafun);
	}
	else if (filetype == SYS_HYPERMESH_ASCII) {
		hmafil = vdm_HMAFilBegin();
		vdm_HMAFilDataFun(hmafil, datafun);
	}
	else if (filetype == SYS_STL ||
		filetype == SYS_STLBIN) {
		stlfil = vdm_STLFilBegin();
		vdm_STLFilDataFun(stlfil, datafun);
	}
	else if (filetype == SYS_OBJ) {
		objfil = vdm_OBJFilBegin();
		vdm_OBJFilDataFun(objfil, datafun);
	}
	else if (filetype == SYS_FDI_NEUTRAL) {
		fdilib = vdm_FDILibBegin();
		vdm_FDILibDataFun(fdilib, datafun);
	}
	else if (filetype == SYS_PLOT3D_GRID) {
		plot3dlib = vdm_PLOT3DLibBegin();
		vdm_PLOT3DLibDataFun(plot3dlib, datafun);
	}
	else if (filetype == SYS_PLOT3D_SOLUTION) {
		plot3dlib = vdm_PLOT3DLibBegin();
		vdm_PLOT3DLibDataFun(plot3dlib, datafun);
	}
	else if (filetype == SYS_FLUENT_MESH) {
		fluentlib = vdm_FLUENTLibBegin();
		vdm_FLUENTLibDataFun(fluentlib, datafun);
	}
	else if (filetype == SYS_ENSIGHT) {
		ensightlib = vdm_EnSightLibBegin();
		vdm_EnSightLibDataFun(ensightlib, datafun);
	}
	else if (filetype == SYS_STARCCM) {
		starccmlib = vdm_STARCCMLibBegin();
		vdm_STARCCMLibDataFun(starccmlib, datafun);
	}
	else if (filetype == SYS_TECPLOT) {
		tecplotlib = vdm_TecplotLibBegin();
		vdm_TecplotLibDataFun(tecplotlib, datafun);
	}
	else if (filetype == SYS_MARC_POST) {
		marclib = vdm_MarcLibBegin();
		vdm_MarcLibDataFun(marclib, datafun);
	}
	else if (filetype == SYS_CGNS) {
		cgnsvlib = vdm_CGNSVLibBegin();
		vdm_CGNSVLibDataFun(cgnsvlib, datafun);
	}
	else if (filetype == SYS_PATRAN_NEUTRAL) {
		patlib = vdm_PatLibBegin();
		vdm_PatLibDataFun(patlib, datafun);
	}
	else if (filetype == SYS_MECHANICA_STUDY) {
		raslib = vdm_RASLibBegin();
		vdm_RASLibDataFun(raslib, datafun);
	}
	else if (filetype == SYS_FEMAP_NEUTRAL) {
		femaplib = vdm_FEMAPLibBegin();
		vdm_FEMAPLibDataFun(femaplib, datafun);
	}
	else if (filetype == SYS_PAM_DAISY || filetype == SYS_PAM_ERF) {
		pamlib = vdm_PAMLibBegin();
		vdm_PAMLibDataFun(pamlib, datafun);
	}
	else if (filetype == SYS_AUTODYN_RES) {
		autodynlib = vdm_AUTODYNLibBegin();
		vdm_AUTODYNLibDataFun(autodynlib, datafun);
	}
	else if (filetype == SYS_POLYFLOW) {
		polyflowlib = vdm_POLYFLOWLibBegin();
		vdm_POLYFLOWLibDataFun(polyflowlib, datafun);
	}
	else if (filetype == SYS_OPENFOAM) {
		openfoamlib = vdm_OpenFOAMLibBegin();
		vdm_OpenFOAMLibDataFun(openfoamlib, datafun);
	}
	else if (filetype == SYS_COMSOL_SECTION) {
		comsollib = vdm_COMSOLLibBegin();
		vdm_COMSOLLibDataFun(comsollib, datafun);
	}
	else if (filetype == SYS_H3D) {
		h3dlib = vdm_H3DLibBegin();
		vdm_H3DLibDataFun(h3dlib, datafun);
	}
	else if (filetype == SYS_PAM_INPUT) {
		pamfil = vdm_PAMFilBegin();
		vdm_PAMFilDataFun(pamfil, datafun);
	}
	else if (filetype == SYS_CFX_RESULT) {
		cfxlib = vdm_CFXLibBegin();
		vdm_CFXLibDataFun(cfxlib, datafun);
	}
	else if (filetype == SYS_GMV) {
		gmvlib = vdm_GMVLibBegin();
		vdm_GMVLibDataFun(gmvlib, datafun);
	}
	else if (filetype == SYS_PERMAS_POST) {
		permaslib = vdm_PERMASLibBegin();
		vdm_PERMASLibDataFun(permaslib, datafun);
	}

	//提高精度
	vdm_DataFunSetConvention(datafun, VDM_CONVENTION_DOUBLE);

	vdm_DataFunOpen(datafun, 0, fileName, filetype);
}

void Mesh::CloseVKISupportFile(vdm_DataFun *datafun)
{

	vdm_DataFunClose(datafun);
	Vobject *obj;
	vdm_DataFunGetObj(datafun, (Vobject**)&obj);

	if (filetype == SYS_SDRC_UNIVERSAL) {
		vdm_SDRCLibEnd((vdm_SDRCLib*)obj);
	}
	else if (filetype == SYS_NASTRAN_BULKDATA) {
		vdm_NASFilEnd((vdm_NASFil*)obj);
	}
	else if (filetype == SYS_NASTRAN_OUTPUT2 ||
		filetype == SYS_NASTRAN_XDB) {
		vdm_NASLibEnd((vdm_NASLib*)obj);
	}
	else if (filetype == SYS_NATIVE) {
		vdm_NatLibEnd((vdm_NatLib*)obj);
	}
	else if (filetype == SYS_ANSYS_INPUT) {
		vdm_ANSFilEnd((vdm_ANSFil*)obj);
	}
	else if (filetype == SYS_ANSYS_RESULT) {
		vdm_ANSLibEnd((vdm_ANSLib*)obj);
	}
	else if (filetype == SYS_ABAQUS_INPUT) {
		vdm_ABAFilEnd((vdm_ABAFil*)obj);
	}
	else if (filetype == SYS_ABAQUS_FIL ||
		filetype == SYS_ABAQUS_ODB) {
		vdm_ABALibEnd((vdm_ABALib*)obj);
	}
	else if (filetype == SYS_LSTC_INPUT) {
		vdm_D3DFilEnd((vdm_D3DFil*)obj);
	}
	else if (filetype == SYS_LSTC_STATE ||
		filetype == SYS_LSTC_STATEFEMZIP ||
		filetype == SYS_LSTC_HISTORY) {
		vdm_D3DLibEnd((vdm_D3DLib*)obj);
	}
	else if (filetype == SYS_HYPERMESH_ASCII) {
		vdm_HMAFilEnd((vdm_HMAFil*)obj);
	}
	else if (filetype == SYS_STL ||
		filetype == SYS_STLBIN) {
		vdm_STLFilEnd((vdm_STLFil*)obj);
	}
	else if (filetype == SYS_OBJ) {
		vdm_OBJFilEnd((vdm_OBJFil*)obj);
	}
	else if (filetype == SYS_FDI_NEUTRAL) {
		vdm_FDILibEnd((vdm_FDILib*)obj);
	}
	else if (filetype == SYS_PLOT3D_GRID) {
		vdm_PLOT3DLibEnd((vdm_PLOT3DLib*)obj);
	}
	else if (filetype == SYS_PLOT3D_SOLUTION) {
		vdm_PLOT3DLibEnd((vdm_PLOT3DLib*)obj);
	}
	else if (filetype == SYS_FLUENT_MESH) {
		vdm_FLUENTLibEnd((vdm_FLUENTLib*)obj);
	}
	else if (filetype == SYS_ENSIGHT) {
		vdm_EnSightLibEnd((vdm_EnSightLib*)obj);
	}
	else if (filetype == SYS_STARCCM) {
		vdm_STARCCMLibEnd((vdm_STARCCMLib*)obj);
	}
	else if (filetype == SYS_TECPLOT) {
		vdm_TecplotLibEnd((vdm_TecplotLib*)obj);
	}
	else if (filetype == SYS_MARC_POST) {
		vdm_MarcLibEnd((vdm_MarcLib*)obj);
	}
	else if (filetype == SYS_CGNS) {
		vdm_CGNSVLibEnd((vdm_CGNSVLib*)obj);
	}
	else if (filetype == SYS_PATRAN_NEUTRAL) {
		vdm_PatLibEnd((vdm_PatLib*)obj);
	}
	else if (filetype == SYS_MECHANICA_STUDY) {
		vdm_RASLibEnd((vdm_RASLib*)obj);
	}
	else if (filetype == SYS_FEMAP_NEUTRAL) {
		vdm_FEMAPLibEnd((vdm_FEMAPLib*)obj);
	}
	else if (filetype == SYS_PAM_DAISY || filetype == SYS_PAM_ERF) {
		vdm_PAMLibEnd((vdm_PAMLib*)obj);
	}
	else if (filetype == SYS_AUTODYN_RES) {
		vdm_AUTODYNLibEnd((vdm_AUTODYNLib*)obj);
	}
	else if (filetype == SYS_POLYFLOW) {
		vdm_POLYFLOWLibEnd((vdm_POLYFLOWLib*)obj);
	}
	else if (filetype == SYS_OPENFOAM) {
		vdm_OpenFOAMLibEnd((vdm_OpenFOAMLib*)obj);
	}
	else if (filetype == SYS_COMSOL_SECTION) {
		vdm_COMSOLLibEnd((vdm_COMSOLLib*)obj);
	}
	else if (filetype == SYS_H3D) {
		vdm_H3DLibEnd((vdm_H3DLib*)obj);
	}
	else if (filetype == SYS_PAM_INPUT) {
		vdm_PAMFilEnd((vdm_PAMFil*)obj);
	}
	else if (filetype == SYS_CFX_RESULT) {
		vdm_CFXLibEnd((vdm_CFXLib*)obj);
	}
	else if (filetype == SYS_GMV) {
		vdm_GMVLibEnd((vdm_GMVLib*)obj);
	}
	else if (filetype == SYS_PERMAS_POST) {
		vdm_PERMASLibEnd((vdm_PERMASLib*)obj);
	}


}


SETTYPE Mesh::getDatasetType(char *datasetName)
{
	SETTYPE type;
	std::string str = datasetName;
	if (str.find("SET.ELEM.T") != -1 )
	{
		type = SETTYPE_ELEM;
	}
	else if(str.find("SET.NODE.T") != -1)
	{
		type = SETTYPE_NODE;
	}
	else if(str.find("SET.ELEM.FACE.T") != -1)
	{
		type = SETTYPE_FACE;
	}
	return type;
}

vdm_Dataset *Mesh::getDataset(char *attrVal,SETTYPE setType)
{

	vdm_Dataset *res = NULL;
	Vint datasetNum,attrNum;
	vdm_DataFunNumDatasets(datafun,&datasetNum);
	int i = 0,j = 0;
	Vchar datasetName[64],attrName[64];
	Vlong lrec;
	Vint nrow,ncol,type;

	while (i < datasetNum)
	{
		vdm_DataFunInqDataset(datafun,i,datasetName,&lrec,&nrow,&ncol,&type);
		vdm_DataFunNumAttributes(datafun,i,&attrNum);
		vdm_LibraryGetDataset(lib,i,&dataset);
		vdm_Attribute *attr;
		Vint len;
		Vint type;
		for (j= 0 ;j < attrNum; j++)
		{
			vdm_DatasetGetAttribute(dataset,j,&attr);
			vdm_AttributeInq(attr,attrName,&len,&type);
			if (strcmp(attrName,"Name") == 0)
			{
				char attrVlue[128]={0};
				vdm_AttributeValueString(attr,attrVlue);
				if (strcmp(attrVal,attrVlue)== 0 )
				{
					SETTYPE sType = SETTYPE_NONE;
					sType = getDatasetType(datasetName);
					if (sType == setType)
					{
						res = dataset;
						return res;
					}
				}	
			}
		}
		i++;
	}

	return res;
}


void Mesh::getMinPrincipalStressMinMax(vis_State *state)
{
	vis_StateSetObject (state, VIS_GRIDFUN, gridfun);
	vdm_Dataset *dataset;
	getDatasetFromName("NID.N",&dataset);
	Vchar name[25];
	Vlong lrec;
	Vint nrow, ncol, ntyp;
	vdm_DatasetInq(dataset,name,&lrec,&nrow,&ncol,&ntyp)  ; 

	vis_StateSetDerive (state,VIS_TENSOR);

	vis_State *newstate;
	newstate = vis_StateBegin();
	vis_StateDef (newstate,lrec,SYS_NODE,SYS_NONE,VIS_TENSOR);
	vis_StateSetParami(state,STATE_MAPNODE ,STATE_MAPAVE);
	vis_StateMap(newstate,state,NULL);


	Vfloat extent[24];
	vis_StateSetDerive (newstate,VIS_TENSOR_MINPRINC);
	vis_StateExtent(newstate,NULL,extent);

	vis_StateEnd(newstate);
}

void Mesh::getMaxPrincipalStressMinMax(vis_State *state)
{
	vis_StateSetObject (state, VIS_GRIDFUN, gridfun);
	vdm_Dataset *dataset;
	getDatasetFromName("NID.N",&dataset);
	Vchar name[25];
	Vlong lrec;
	Vint nrow, ncol, ntyp;
	vdm_DatasetInq(dataset,name,&lrec,&nrow,&ncol,&ntyp)  ; 

	vis_StateSetDerive (state,VIS_TENSOR);

	vis_State *newstate;
	newstate = vis_StateBegin();
	vis_StateDef (newstate,lrec,SYS_NODE,SYS_NONE,VIS_TENSOR);
	vis_StateSetParami(state,STATE_MAPNODE ,STATE_MAPAVE);
	vis_StateMap(newstate,state,NULL);


	Vfloat extent[24];
	vis_StateSetDerive (newstate,VIS_TENSOR_MAXPRINC);
	vis_StateExtent(newstate,NULL,extent);

	vis_StateEnd(newstate);
}

void Mesh::getMinPrincipalStressAtElemMinMax(vis_State *state)
{
	vis_StateSetObject (state, VIS_GRIDFUN, gridfun);
	Vfloat extent[24];
	vis_StateSetDerive (state,VIS_TENSOR_MINPRINC);
	vis_StateExtent(state,NULL,extent);
}

void Mesh::getMaxPrincipalStressAtElemMinMax(vis_State *state)
{
	vis_StateSetObject (state, VIS_GRIDFUN, gridfun);
	Vfloat extent[24];
	vis_StateSetDerive (state,VIS_TENSOR_MAXPRINC);
	vis_StateExtent(state,NULL,extent);
}

void Mesh::getMisesStressMinMax(vis_State *state)
{//need map

	vis_StateSetObject (state, VIS_GRIDFUN, gridfun);
	vdm_Dataset *dataset;
	getDatasetFromName("NID.N",&dataset);
	Vchar name[25];
	Vlong lrec;
	Vint nrow, ncol, ntyp;
	vdm_DatasetInq(dataset,name,&lrec,&nrow,&ncol,&ntyp)  ; 

	vis_StateSetDerive (state,VIS_TENSOR);

	vis_State *newstate;
	newstate = vis_StateBegin();
	vis_StateDef (newstate,lrec,SYS_NODE,SYS_NONE,VIS_TENSOR);
	vis_StateSetParami(state,STATE_MAPNODE ,STATE_MAPAVE);
	vis_StateMap(newstate,state,NULL);


	Vfloat extent[24];
	vis_StateSetDerive (newstate,VIS_TENSOR_VONMISES);
	vis_StateExtent(newstate,NULL,extent);

	vis_StateEnd(newstate);

}


void Mesh::getMisesStressAtElemMinMax(vis_State *state)
{
	vis_StateSetObject (state, VIS_GRIDFUN, gridfun);
	Vfloat extent[24];
	vis_StateSetDerive (state,VIS_TENSOR_VONMISES);
	vis_StateExtent(state,NULL,extent);
}



void Mesh::getDisplacementMinMax(vis_State *state)
{
	vis_StateSetObject (state, VIS_GRIDFUN, gridfun);
	Vfloat extent[24];
	vis_StateSetDerive (state,VIS_VECTOR_MAG);
	vis_StateExtent(state,NULL,extent);
}

long Mesh::getMaxNode()
{
	vdm_Dataset *ds;
	getDatasetFromName("NID.N",&ds);
	Vlong a;
	Vint aa,b,c,d;
	Vchar name[42];
	vdm_DatasetInq(ds,name,&a,&b,&c,&d);
	return a;
}

//read file to datafun
Vint Mesh::suftst(Vchar inputfile[], Vchar suffix[])
{
	Vint leni, lens;

	leni = (Vint)strlen(inputfile);
	lens = (Vint)strlen(suffix);
	return strcmp(&inputfile[leni-lens],suffix);
}

void Mesh::GetFileType(Vchar* inputfile)
{
	if(suftst(inputfile,".unv") == 0 ||
		suftst(inputfile,".bun") == 0) {
			filetype = SYS_SDRC_UNIVERSAL;
	} else if(suftst(inputfile,".dat") == 0  ||
		suftst(inputfile,".bdf") == 0  ||
		suftst(inputfile,".nas") == 0) {
			filetype = SYS_NASTRAN_BULKDATA;
	} else if(suftst(inputfile,".op2") == 0  ||
		suftst(inputfile,".OP2") == 0  ||
		suftst(inputfile,".rs2") == 0) {
			filetype = SYS_NASTRAN_OUTPUT2;
	} else if(suftst(inputfile,".xdb") == 0) {
		filetype = VDM_NASTRAN_XDB;
	} else if(suftst(inputfile,".vdm") == 0) {
		filetype = SYS_NATIVE;
	} else if(suftst(inputfile,".cdb") == 0  ||
		suftst(inputfile,".ans") == 0) {
			filetype = SYS_ANSYS_INPUT;
	} else if(suftst(inputfile,".rst") == 0  ||
		suftst(inputfile,".rth") == 0  ||
		suftst(inputfile,".rmg") == 0  ||
		suftst(inputfile,".rfl") == 0) {
			filetype = SYS_ANSYS_RESULT;
	} else if(suftst(inputfile,".inp") == 0) {
		filetype = SYS_ABAQUS_INPUT;
	} else if(suftst(inputfile,".fil") == 0) {
		filetype = SYS_ABAQUS_FIL;
	} else if(suftst(inputfile,".odb") == 0) {
		filetype = SYS_ABAQUS_ODB;
	} else if(suftst(inputfile,".k")   == 0 ||
		suftst(inputfile,".key") == 0 ||
		suftst(inputfile,".dyn") == 0) {
			filetype = SYS_LSTC_INPUT;
	} else if(suftst(inputfile,"d3plot") == 0 ||
		suftst(inputfile,"D3PLOT") == 0 ||
		suftst(inputfile,"ptf") == 0) {
			filetype = SYS_LSTC_STATE;
	} else if(suftst(inputfile,".fz") == 0) {
		filetype = SYS_LSTC_STATEFEMZIP;
	} else if(suftst(inputfile,"d3thdt") == 0 ||
		suftst(inputfile,"D3THDT") == 0) {
			filetype = SYS_LSTC_HISTORY;
	} else if(suftst(inputfile,".hma") == 0) {
		filetype = SYS_HYPERMESH_ASCII;
	} else if(suftst(inputfile,".stl") == 0) {
		filetype = SYS_STL;
	} else if(suftst(inputfile,".STL") == 0) {
		filetype = SYS_STLBIN;
	} else if(suftst(inputfile,".obj") == 0) {
		filetype = SYS_OBJ;
	} else if(suftst(inputfile,".FDNEUT") == 0 ||
		suftst(inputfile,".fdneut") == 0) {
			filetype = SYS_FDI_NEUTRAL;
	} else if(suftst(inputfile,".x") == 0  ||
		suftst(inputfile,".g") == 0 ||
		suftst(inputfile,".xyz") == 0 ||
		suftst(inputfile,".p3d") == 0 ||
		suftst(inputfile,".bin") == 0 ||
		suftst(inputfile,".grd") == 0) {
			filetype = SYS_PLOT3D_GRID;
	} else if(suftst(inputfile,".q") == 0) {
		filetype = SYS_PLOT3D_SOLUTION;
	} else if(suftst(inputfile,".case") == 0 ||
		suftst(inputfile,".CASE") == 0 ||
		suftst(inputfile,".encas") == 0) {
			filetype = SYS_ENSIGHT;
	} else if(suftst(inputfile,".ccm") == 0 ||
		suftst(inputfile,".ccmg") == 0 ||
		suftst(inputfile,".ccmp") == 0 ||
		suftst(inputfile,".ccmt") == 0 ||
		suftst(inputfile,".CCM") == 0) {
			filetype = SYS_STARCCM;
	} else if(suftst(inputfile,".plt") == 0) {
		filetype = SYS_TECPLOT;
	} else if(suftst(inputfile,".cas") == 0 ||
		suftst(inputfile,".msh") == 0) {
			filetype = SYS_FLUENT_MESH;
	} else if(suftst(inputfile,".t16") == 0 ||
		suftst(inputfile,".t19") == 0) {
			filetype = SYS_MARC_POST;
	} else if(suftst(inputfile,".cgns") == 0) {
		filetype = SYS_CGNS;
	} else if(suftst(inputfile,".out") == 0 ||
		suftst(inputfile,".pat") == 0) {
			filetype = SYS_PATRAN_NEUTRAL;
	} else if(suftst(inputfile,".neu") == 0) {
		filetype = SYS_MECHANICA_STUDY;
	} else if(suftst(inputfile,".NEU") == 0) {
		filetype = SYS_FEMAP_NEUTRAL;
	} else if(suftst(inputfile,".dsy") == 0 || suftst(inputfile,".DSY") ==0) {
		filetype = SYS_PAM_DAISY;
	} else if(suftst(inputfile,".thp") == 0 || suftst(inputfile,".THP") == 0) {
		filetype = SYS_PAM_DAISY;
	} else if(suftst(inputfile,".adres") == 0) {
		filetype = SYS_AUTODYN_RES;
	} else if(suftst(inputfile,".msh") == 0) {
		filetype = SYS_POLYFLOW;
	} else if(suftst(inputfile,".erfh5") == 0) {
		filetype = SYS_PAM_ERF;
	} else if(strstr(inputfile,"controlDict") != NULL) {
		filetype = SYS_OPENFOAM;
	} else if(suftst(inputfile,".txt") == 0) {
		filetype = SYS_COMSOL_SECTION;
	} else if(suftst(inputfile,".h3d") == 0) {
		filetype = SYS_H3D;
	} else if(suftst(inputfile,".pc") == 0) {
		filetype = SYS_PAM_INPUT;
	} else if(suftst(inputfile,".res") == 0) {
		filetype = SYS_CFX_RESULT;
	} else if(strstr(inputfile,"gmv") != NULL) {
		filetype = SYS_GMV;
	} else if(strstr(inputfile,".post") != NULL) {
		filetype = SYS_PERMAS_POST;
	} else {
		filetype = 0;
	}
}


//get dataset from datafun
//返回dataset index
int Mesh::getDatasetIndexFromName(char *datasetName)
{
	int searchNum = 0;
	Vint searchItem[5];
	vdm_LibrarySearchDataset(lib,datasetName,1,searchItem,&searchNum);
	return searchItem[0];
}

void Mesh::getDatasetFromName(char *datasetName,vdm_Dataset** dataset)
{
	int index = getDatasetIndexFromName(datasetName);
	vdm_LibraryGetDataset(lib,index,dataset);
}

void Mesh::printDatasetint(char *datesetName)
{

	vdm_Dataset* dataset;
	Vlong lrec;
	Vint nrow, ncol, ntyp;
	Vchar dsname[DATASET_MAXNAME];

	getDatasetFromName(datesetName, &dataset);
	int index = getDatasetIndexFromName(datesetName);

	vdm_DatasetInq(dataset, dsname, &lrec, &nrow, &ncol, &ntyp);

	Vint* buff = (Vint*)malloc(lrec * sizeof(int));

	vdm_DataFunReadDataset(datafun, index, buff);

	int i = 0;
	int j = 0;
	while (i < lrec)
	{
		j = 0;
		for (; j < nrow; j++)
		{
			printf("%d ", *(buff + i + j));
		}
		i += nrow;
		printf("\n");

	}
	free(buff);

}


//get state from dataset
//type =1 type=2 根据parentnode和childnode来区分
//todo:type??
void Mesh::getStateFromDatasetIndex(int index,vis_State *state,int type)
{
	vis_RProp *rprop;
	rprop = vis_RPropBegin();
	if (type == 1)
	{
		vis_RPropDef (rprop,SYS_ELEM,SYS_NODE);
	}
	else if (type == 2)
	{
		vis_RPropDef (rprop,SYS_NODE,SYS_NONE);
	}
	else if(type == 3)
	{
		vis_RPropDef (rprop,SYS_ELEM,SYS_NONE);
	}


	vis_RPropSetDatasetIndex (rprop,index);

	vdm_LManLoadState(lman,state,rprop);


	vis_RPropEnd(rprop);
}

void Mesh::getStateFromDatasetName(char *datasetName,vis_State *state,int type)
{

	Vint index = getDatasetIndexFromName(datasetName);
	getStateFromDatasetIndex(index,state,type);
}

//datasetSubName用来提供额外的dataset名字信息,保证找到正确的dataset
void Mesh::getDatasetNameFromDatasetAttrName(char *datasetAttrVal,list<string> &dataSetName,char* datasetSubName)
{
	Vint tmp;
	vdm_LibraryGetNumDatasets(lib,&tmp);


	Vint datasetNum,attrNum;
	vdm_DataFunNumDatasets(datafun,&datasetNum);
	int i = 0,j = 0;
	Vchar dsn[64],attrName[64];
	Vlong lrec;
	Vint nrow,ncol,dtype;
	vdm_Dataset *ds;
	while (i < datasetNum)
	{

		vdm_DataFunInqDataset(datafun,i,dsn,&lrec,&nrow,&ncol,&dtype);
		vdm_DataFunNumAttributes(datafun,i,&attrNum);
		vdm_LibraryGetDataset(lib,i,&ds);
		vdm_Attribute *attr;
		Vint len;
		Vint type;
		for (j= 0 ;j < attrNum; j++)
		{

			vdm_DatasetGetAttribute(ds,j,&attr);
			Vfloat val;
			vdm_AttributeInq(attr,attrName,&len,&type);
			if (strcmp(attrName,"Name") == 0)
			{
				char attrVlue[128]={0};
				vdm_AttributeValueString(attr,attrVlue);
				if( strcmp(datasetAttrVal,attrVlue) == 0 )
				{
					int k = strlen(datasetSubName);
					if (strcmp(datasetSubName,"") != 0 && strncmp(dsn,datasetSubName,k) == 0)
					{
						dataSetName.push_back(dsn);
						//strcpy(dataSetName,dsn);
					//	return;
					}
					
					
				}
			}

		}

		i++;
	}
}


void Mesh::getNodesetFromName(set<Vint> &nodeids,char *labelName)
{
	vis_IdTran *idtran;
	Vint iset;
	Vint i = 0,lrec;
	Vint id;
	Vchar name[256];
	vsy_HashTable *nsethash;
	vis_ModelGetHashTable (model, VIS_IDTRAN_NODE, &nsethash);
	if (nsethash != NULL)
	{
		vsy_HashTableInitIter (nsethash);
		while (vsy_HashTableNextIter(nsethash, &iset, (Vobject**)&idtran), idtran) 
		{
			vis_IdTranGetName (idtran,name);
			if(strcmp(name,labelName) == 0)
			{
				vis_IdTranCount(idtran,IDTRAN_MAXINDEX,&lrec);
				for (i = 1;i <= lrec;i++)
				{
					vis_IdTranGetId(idtran,i,&id);
					nodeids.insert(id);
				}
			}
		}
	}
}

void Mesh::getElemsetFromName(set<Vint> &elemids,char *labelName)
{
	vis_IdTran *idtran;
	Vint iset;
	Vint i = 0,lrec;
	Vint id;
	Vchar name[256];
	vsy_HashTable *nsethash;
	vis_ModelGetHashTable (model, VIS_IDTRAN_ELEM, &nsethash);
	if (nsethash != NULL)
	{
		vsy_HashTableInitIter (nsethash);
		while (vsy_HashTableNextIter(nsethash, &iset, (Vobject**)&idtran), idtran) 
		{
			vis_IdTranGetName (idtran,name);
			if(strcmp(name,labelName) == 0)
			{
				vis_IdTranCount(idtran,IDTRAN_MAXINDEX,&lrec);
				for (i = 1;i <= lrec;i++)
				{
					vis_IdTranGetId(idtran,i,&id);
					elemids.insert(id);
				}
			}
		}
	}
}

