#ifndef MESH_H_
#define MESH_H_

#include <QObject>

#include "base/base.h"
#include "vis/visdata.h"
#include "vdm/vdm.h"
#include "vis/vis.h"

#include <vtkSmartPointer.h>
#include <vtkPoints.h>
#include <vtkUnstructuredGrid.h>

#include <list>//use qt instead
#include <set>
#include <string>
using namespace std;

#include "typedef.h"

/*
Mesh������Ҫ�����Է����ļ�(�����ļ�,����ļ�)�Ĳ鿴,ת��. ��:
1 ��ȡ��������е�mises��,�����Ӧ����;
2 ��ȡ����ļ����������ļ�������;

*/


class Mesh :public QObject
{
	Q_OBJECT
public:
	Mesh();
	~Mesh();
public:
	void loadData(char *fileName);
	
signals:
	void finishDataLoaded();


public:
	void setFileName(char *fileName);

	void getDisplacementMinMax(vis_State *state);

	void getMisesStressAtElemMinMax(vis_State *state);
	void getMisesStressMinMax(vis_State *state);

	void getMinPrincipalStressMinMax(vis_State *state);
	void getMinPrincipalStressAtElemMinMax(vis_State *state);

	void getMaxPrincipalStressMinMax(vis_State *state);
	void getMaxPrincipalStressAtElemMinMax(vis_State *state);

	long getMaxNode();

public:
	SetType getDatasetType(char *datasetName);
	vdm_Dataset *getDataset(char *attrVal, SetType setType);

protected:
	Vint suftst(Vchar inputfile[], Vchar suffix[]);
	void GetFileType(Vchar* inputfile);
	void ReadVKISupportFile(Vchar *fileName,vdm_DataFun *datafun);
	void CloseVKISupportFile(vdm_DataFun *datafun);

	void readMesh();
public:
	//get dataset from datafun

	void getDatasetNameFromDatasetAttrName(char *datasetAttrVal,list<string> &dataSetName,char* datasetSubName);
	int getDatasetIndexFromName(char *datasetName);
	void getDatasetFromName(char *datasetName,vdm_Dataset** dataset);
	void printDatasetint(char *datesetName);


	//get state from dataset
	void getStateFromDatasetIndex(int index,vis_State *state,int type);
	void getStateFromDatasetName(char *datasetName,vis_State *state,int type);


	//ͨ����ǩ�õ��ڵ㼯��
	void getNodesetFromName(set<int> &nodeids,char *labelName);
	void getElemsetFromName(set<int> &elemids,char *labelName);

public:
	vdm_DataFun *datafun;
	vdm_LMan *lman;
	vis_Model *model;
	vdm_Library *lib;

	vis_Connect *connect;
	vis_GridFun *gridfun;
	vdm_Dataset *dataset;
	vis_State *state;
	std::string dpFileName;

	Vint filetype;//��ȡ���ļ�����

	int n_nodes;
	int n_elements;
	int n_faces;

	std::map<int, int> nid_map;
	std::map<int, int> eid_map;

	vtkSmartPointer<vtkUnstructuredGrid> ugrid;
};

#endif