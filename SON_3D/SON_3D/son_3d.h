#ifndef SON_3D_H
#define SON_3D_H

#include <QtGui/QMainWindow>
#include "ui_son_3d.h"

class SON_3D : public QMainWindow
{
	Q_OBJECT

public:
	SON_3D(QWidget *parent = 0, Qt::WFlags flags = 0);
	~SON_3D();

private:
	Ui::SON_3DClass ui;
};

#endif // SON_3D_H
