#include <QApplication>
#include <QDebug>
#include <qwidget>

#include <Python.h>

#include <base/base.h>
#include <vis/visdata.h>
#include <vdm/vdm.h>

#include "widget3d.h"
#include "mainwindow.h"
//#include "HighlightInteractorStyle.h"

extern  int numargs;

/* Return the number of arguments of the application command line */
static PyObject* emb_numargs(PyObject *self, PyObject *args)
{
	if (!PyArg_ParseTuple(args, ":numargs"))
		return NULL;
	return Py_BuildValue("i", numargs);
}

static PyMethodDef EmbMethods[] = {
	{ "numargs", emb_numargs, METH_VARARGS,
	"Return the number of arguments received by the process." },
	{ NULL, NULL, 0, NULL }
};
//vtkStandardNewMacro(HighlightInteractorStyle);

//MainWindow * p = NULL;
int main(int argc, char** argv)
{
	QApplication app(argc, argv);

	MainWindow widget;
	widget.resize(800, 600);
	//p = &widget;
	
	Py_SetProgramName(argv[0]);  /* optional but recommended */
	Py_Initialize();

	Py_InitModule("emb", EmbMethods);
	//numargs = this->getPolyData()->GetCellData()->GetNumberOfArrays();
	numargs = 1111;
	PyRun_SimpleString("import emb\nprint \"Number of arguments\", emb.numargs()\n");


	widget.show();
	
	app.exec();

	Py_Finalize();

	return EXIT_SUCCESS;
}