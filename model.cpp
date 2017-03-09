
#include <QDebug>
#include "qstringlist.h"

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
{
	return headers.size();
}

QVariant Model::data(const QModelIndex & index, int role) const
{ 
	QVariant res;

	if (role == Qt::DisplayRole )//|| role == Qt::EditRole
	{
		const QStringList& data = modelData();
		QString text = data[index.row()];
		QStringList l = text.split("##");
		res = l[index.column()];
	}

 
	if (role == Qt::EditRole)
	{
		res = "xxx";
	}
	return res;
		 
}


bool Model::setData(const QModelIndex & index, const QVariant & value, int role )
{

	if (role == Qt::EditRole)
	{
		qDebug() << "Model::setData";
		return true;
	}
	return true;
// 	else
// 		return false;

}


QVariant Model::headerData(int section, Qt::Orientation orientation, int role )const
{
	QVariant  res;
	if (role == Qt::DisplayRole && orientation == Qt::Horizontal)
		res = headers[section];
	else if ( role == Qt::DisplayRole && orientation == Qt::Vertical )
		res = "remove";

	return res;
}

//从python复制过来的，少了个const，导致出现很奇怪的问题（delegate没有起作用）
Qt::ItemFlags Model::flags(const QModelIndex & index)const
 {
//#
	return Qt::ItemIsEditable | Qt::ItemIsEnabled;
 }

bool Model::removeRows(int row, int count, const QModelIndex &parent)
{
	beginRemoveRows(QModelIndex(), row, row);
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
		modelData_().append(QString("Event%1## ##1 ##10 ##1 ").arg(_eventID));
	}
	else if (_type == "Loading History")
	{
		modelData_().append(QString("Event%1## ##1 ").arg(_eventID));
	}
	else if (_type == "Superimposed")
	{
		modelData_().append(QString("Event%1## ##1 ").arg(_eventID));
	}
	_eventID += 1;

	endInsertRows();

	return true;
}

bool Model::moveRows(const QModelIndex & sourceParent, int sourceRow, int count, const QModelIndex & destinationParent, int destinationChild)
{
	beginMoveRows(sourceParent, sourceRow, sourceRow , destinationParent, destinationChild);
	//#

	//QStringList sl = modelData_();
	modelData_().swap(sourceRow, destinationChild);

	endMoveRows();
	return true;
}
 