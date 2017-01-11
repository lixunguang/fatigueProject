
#include <QDebug>

#include "model.h"

Model::Model(QString type, QObject *parent)
:QAbstractTableModel(parent)
{
	QStringList h;
	h << "Event" << "Result Set" << "Scale" << "Scale History" << "Repeats" << "Radio";

	_type = type;

	if (type == "Duty Cycle")
		headers << h[0] << h[1] << h[2] << h[4] << h[5];
	else if (type == "Loading History")
		headers << h[0] << h[1] << h[2];
	else if (type == "Superimposed")
		headers << h[0] << h[1] << h[3];

	
	_eventID = 0;
}

Model::~Model()
{

}

const QStringList& Model::modelData()const
{

	if (_type == "Duty Cycle")
		return modelData_duty;
	else if (_type == "Loading History")
		return modelData_History;
	else if (_type == "Superimposed")
		return modelData_Super;
	else
		return modelData_duty;//todo
}

QStringList& Model::modelData_()
{
	if (_type == "Duty Cycle")
		return modelData_duty;
	else if (_type == "Loading History")
		return modelData_History;
	else if (_type == "Superimposed")
		return modelData_Super;
	else
		return modelData_duty;//todo
}

int	Model::rowCount(const QModelIndex & parent) const
{
	return modelData().size();

}


int	Model::columnCount(const QModelIndex & parent) const
{//Return the number of columns to Qt mechanism when the table	changes

	return headers.size();
}


QVariant Model::data(const QModelIndex & index, int role) const
{//Provide data for each cell in the display role
 
	QString res;

	if (role == Qt::DisplayRole || role == Qt::EditRole)
	{
		const QStringList& data = modelData();
		QString text = data[index.row()];
		QStringList l = text.split("##");
		res = l[index.column()];
	}

	return res;
		 
}


bool Model::setData(const QModelIndex & index, const QVariant & value, int role )
{

	if (role == Qt::EditRole)
		return true;
	else
		return false;

}


QVariant Model::headerData(int section, Qt::Orientation orientation, int role )
{

	//	"""Tell to Qt mechanism the header data for building table view"""
	QString  res;
	if (role == Qt::DisplayRole && orientation == Qt::Horizontal)
		res = headers[section];
	else if ( role == Qt::DisplayRole && orientation == Qt::Vertical )
		res = "remove";

	return res;
}


Qt::ItemFlags Model::flags(const QModelIndex & index)
 {	//	  Tell to Qt mechanism that each cell is enabled and editable

//#readonly
	return Qt::ItemIsEditable | Qt::ItemIsEnabled;
 }

bool Model::removeRows(int row, int count, const QModelIndex &parent)
{
	beginRemoveRows(QModelIndex(), row, count);
	modelData_().removeAt(row);
	endRemoveRows();
	return true;
}

bool Model::insertRows(int row, int count, const QModelIndex & parent)
{
	int cnt = this->modelData().size();
	beginInsertRows(QModelIndex(), cnt, cnt);

	if (_type == "Duty Cycle")
	{
		modelData_().append(QString("Event%d## ## ## ## ").arg(_eventID));
	}
	else if (_type == "Loading History")
	{
		modelData_().append(QString("Event%d## ## ").arg(_eventID));
	}
	else if (_type == "Superimposed")
	{
		modelData_().append(QString("Event%1## ## ").arg(_eventID));
	}



	_eventID += 1;
	endInsertRows();
	return true;
}

bool Model::moveRows(const QModelIndex & sourceParent, int sourceRow, int count, const QModelIndex & destinationParent, int destinationChild)
{
	beginMoveRows(sourceParent, sourceRow, sourceRow + count, destinationParent, destinationChild);
	//#change
// 	first = model->modelData()[r];
// 	second = model->modelData()[r1];
// 	model->modelData()[r] = second;
// 	model->modelData()[r1] = first;

	endMoveRows();
	return true;
}
 