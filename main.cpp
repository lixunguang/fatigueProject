#include <QApplication>
#include <QDebug>

#include <Python.h>

#include "mainwindow.h"

extern int numargs;

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
	app.setStyle(new Qtitan::RibbonStyle());
	app.setApplicationName("Fatigue Project");
	app.setOrganizationName("Demx");

	MainWindow widget;
	widget.resize(800, 600);
	
	Py_SetProgramName(argv[0]);  /* optional but recommended */
	Py_Initialize();

	Py_InitModule("emb", EmbMethods);
	//numargs = this->getPolyData()->GetCellData()->GetNumberOfArrays();



	widget.show();
	
	app.exec();

	Py_Finalize();

	return EXIT_SUCCESS;
}