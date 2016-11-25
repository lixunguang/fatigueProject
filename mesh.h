
#ifndef MESH_H_
#define MESH_H_

#include "base/base.h"
#include "vis/visdata.h"
#include "vdm/vdm.h"
#include "vis/vis.h"

#include <list>
#include <set>
#include <string>
using namespace std;


/*
datapicker类用主要用来对仿真文件(输入文件,结果文件)的查看,转换. 如:
1 获取结果数据中的mises力,最大主应力等;
2 获取结果文件或者输入文件的数据;

*/
enum SETTYPE
{
	SETTYPE_NONE,
	SETTYPE_NODE,
	SETTYPE_ELEM,
	SETTYPE_FACE,
};

class Mesh
{
public:
	Mesh(char *fileName);
	~Mesh();

public:
//datapicker interface

	void getDisplacementMinMax(vis_State *state);

	void getMisesStressAtElemMinMax(vis_State *state);
	void getMisesStressMinMax(vis_State *state);

	void getMinPrincipalStressMinMax(vis_State *state);
	void getMinPrincipalStressAtElemMinMax(vis_State *state);

	void getMaxPrincipalStressMinMax(vis_State *state);
	void getMaxPrincipalStressAtElemMinMax(vis_State *state);

	long getMaxNode();

public:
	//lman:输出dataset信息到 文件 屏幕 
	void printLManDatasetToFile(char *fileName);
	void printLManDataset();

	//lib
	void printLibDataset();


	SETTYPE getDatasetType(char *datasetName);
	vdm_Dataset *getDataset(char *attrVal,SETTYPE setType);


protected:
//utils func

	//read file to datafun
	Vint suftst(Vchar inputfile[], Vchar suffix[]);
	void GetFileType(Vchar* inputfile);
	void ReadVKISupportFile(Vchar *fileName,vdm_DataFun *datafun);
	void CloseVKISupportFile(vdm_DataFun *datafun);

public:
	//get dataset from datafun

	void getDatasetNameFromDatasetAttrName(char *datasetAttrVal,list<string> &dataSetName,char* datasetSubName);
	int getDatasetIndexFromName(char *datasetName);
	void getDatasetFromName(char *datasetName,vdm_Dataset** dataset);
	void printDatasetint(char *datesetName);
	void printDatasetint(vdm_Dataset *dateset);

	void printDatasetfloat(char *datesetName);

	//get state from dataset
	void getStateFromDatasetIndex(int index,vis_State *state,int type);
	void getStateFromDatasetName(char *datasetName,vis_State *state,int type);


	//通过标签得到节点集合
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

	Vint filetype;//读取的文件类型

};

#endif