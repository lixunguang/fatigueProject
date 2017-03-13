#ifndef _typedef___
#define _typedef___

#include <QString>
enum ObjType
{//3种输入，edit,comb,text
	ObjType_NONE,
	ObjType_edit,
	ObjType_combo,
	ObjType_text,
};

enum SelectType
{
	SelectType_None,
	SelectType_Point,
	SelectType_Cell,
};

enum SetType
{
	SetType_NONE,
	SetType_NODE,
	SetType_ELEM,
	SetType_FACE,
};

 struct LabelItemInfo//定义标签节点
 {
 	QString objName;
 	QString text;
 
 };
 
 struct SolverItemInfo//定义求解设置节点
 {
 	QString objName;
 	ObjType type;
 	QString text;
 };
#endif