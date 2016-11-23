
#include "widget3d.h"


int numargs = 0;

void Widget3D::update()
{
	numargs++;
	//numargs = style->SelectedActor->GetMapper()->GetInputAsDataSet()->GetNumberOfCells();
	PyRun_SimpleString("print \"Number of argumen11223ts\", emb.numargs()\n");




}