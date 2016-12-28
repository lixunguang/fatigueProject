#ifndef Model_H_
#define Model_H_

#include <QString>
#include <QStringList>
#include <QAbstractTableModel>

class  Model : public QAbstractTableModel
{
	Q_OBJECT
public:
	Model( QString type,QObject *parent = 0);
	~Model();
public:
	const QStringList& modelData()const;
public :
	int	rowCount(const QModelIndex & parent = QModelIndex())const;
	int	columnCount(const QModelIndex & parent = QModelIndex())const;
	QVariant data(const QModelIndex & index, int role = Qt::DisplayRole)const;
	bool setData(const QModelIndex & index, const QVariant & value, int role = Qt::EditRole);
	QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole);
	Qt::ItemFlags flags(const QModelIndex & index);

private:
	QStringList modelData_duty;
	QStringList modelData_Hidstory;
	QStringList modelData_Super;
	QStringList headers;

	QString _type;
};

#endif