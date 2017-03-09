
#include <QComboBox>
#include <QLineEdit>
#include <QString>
#include <QDebug>

#include "delegate.h"
#include "mainwindow.h"

Delegate::Delegate(QObject *parent)
:QStyledItemDelegate(parent)
{
	table = (Table *)parent;
}

Delegate::~Delegate()
{

}

QWidget *Delegate::createEditor(QWidget * parent, const QStyleOptionViewItem & option, const QModelIndex & index) const
{
	if (index.column() == 0)
	{
		return NULL;
	}
	else if (index.column() == 1)
	{
		MainWindow *mw = MainWindow::getMainWindow();
		QStringList caseNames = mw->getOpViewer()->getCaseManager()->getCaseName();

		QComboBox* comb = new QComboBox(parent);
		comb->addItems(caseNames);
		return comb;
	}
	else
	{
		return new QLineEdit(parent);
	}
	
}

void Delegate::setEditorData(QWidget * editor, const QModelIndex & index) const
{
	QString text = table->model->modelData()[index.row()];
	QStringList li = text.split("##");
	if (index.column() == 0)
	{
	}
	else if (index.column() == 1)
	{
		QComboBox* control = (QComboBox*)editor;

	}
	else
	{
		QLineEdit* control = (QLineEdit*)editor;
		control->setText("1");
	}
	
}

void Delegate::setModelData(QWidget * editor, QAbstractItemModel * model, const QModelIndex & index) const
{
	QString ori_text = table->model->modelData_()[index.row()];
	QStringList li = ori_text.split("##");
	if (index.column() == 1)
	{
		QComboBox* control = (QComboBox*) editor;
		li[index.column()] = QString(control->currentText());
	}
	else
	{
		QLineEdit* control = (QLineEdit*) editor;
		li[index.column()] = QString(control->text());
	}
	
	QString	s;
	s = li.join("##");

	table->model->modelData_()[index.row()] = s;
}
