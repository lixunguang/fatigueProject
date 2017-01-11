
#include <QComboBox>
#include <QLineEdit>
#include <QString>
#include <QDebug>

#include "delegate.h"

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
	if (index.column() == 1)
	{
		//caseNames = GLOBAL_CM.getCaseName()
			//#print 'createEditor', caseNames
		QComboBox* comb = (QComboBox*)parent;
		comb->addItems(QStringList("caseNames"));
		return comb;
	}
	else
	{
		return (QLineEdit*)parent;
	}
	
}

void Delegate::setEditorData(QWidget * editor, const QModelIndex & index) const
{
//#set val
	
	QString text = table->model->modelData()[index.row()];
	QStringList l = text.split('##');
	if (index.column() == 1)
	{
		QComboBox* control = (QComboBox*)editor;
		//caseNames = GLOBAL_CM.getCaseName()
		if (l[index.column()] != "" )
		{
		//	control->setCurrentIndex(caseNames.index(l[index.column()]));
			control->setCurrentIndex(1);
		}
		
	}
	else
	{
		QLineEdit* control = (QLineEdit*)editor;
		//control->setText(l[index.column()]);
		control->setText("xx");
	}
	
}

void Delegate::setModelData(QWidget * editor, QAbstractItemModel * model, const QModelIndex & index) const
{
	QString ori_text = table->model->modelData_()[index.row()];
	QStringList l = ori_text.split('##');
	if (index.column() == 1)
	{
		QComboBox* control = (QComboBox*)editor;
		l[index.column()] = QString(control->currentText());
	}
	else
	{
		QLineEdit* control = (QLineEdit*)editor;
		l[index.column()] = QString(control->text());
	}
	
	QString	s = '##';
	//s = s.join(l);

	table->model->modelData_()[index.row()] = s;
}
