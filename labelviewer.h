#ifndef labelview_H_
#define labelview_H_

#include <QTreeWidget>
#include <QDockWidget>

class  LabelViewer : public QDockWidget
{
	Q_OBJECT
public:
	LabelViewer(QWidget *parent = 0);
	~LabelViewer();
public slots:
	void onAddBtn();
	void onRemoveBtn();

};

#endif