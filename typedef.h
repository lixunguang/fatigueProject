#ifndef _typedef___
#define _typedef___

#include <QString>
enum InputObjType
{//3种输入类型，edit,comb,text
	InputObjType_NONE,
	InputObjType_edit,
	InputObjType_combo,
	InputObjType_text,
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
	SetType_OPER,//Operator
};

 struct LabelItemInfo//定义标签节点
 {
 	QString objName;
 	QString text;
 
 };
 
 struct SolverItemInfo//定义求解设置节点
 {
 	QString objName;
 	//ObjType type;
 	QString text;
 };
#endif