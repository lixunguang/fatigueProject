#ifndef propertyview_H_
#define propertyview_H_

#include <QTextEdit>
#include <QMap>
#include <QDockWidget>

class  PropertyViewer : public QDockWidget
{
	Q_OBJECT
public:
	PropertyViewer(QWidget *parent = 0);
	~PropertyViewer();
public:
	void setData(QString& strData);
	void setData(QMap<QString, QString> &mapData);

private:
	QTextEdit *nodeEdit;
};

#endif