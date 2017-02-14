#ifndef typedef___
#define typedef___


enum OBJECTTYPE
{//2种输入，edit,comb
	type_NONE,
	type_edit,
	type_combo,
};

enum Select_Type
{
	Select_Type_None,
	Select_Type_Point,
	Select_Type_Cell,
};

enum SETTYPE
{
	SETTYPE_NONE,
	SETTYPE_NODE,
	SETTYPE_ELEM,
	SETTYPE_FACE,
};

#endif