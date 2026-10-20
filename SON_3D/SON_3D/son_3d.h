#ifndef SON_3D_H
#define SON_3D_H

#include <QMainWindow>
#include "ui_son_3d.h"

class SON_3D : public QMainWindow
{
	Q_OBJECT

public:
	SON_3D(QWidget *parent = 0, Qt::WindowFlags flags = Qt::WindowFlags());
	~SON_3D();

private:
	Ui::SON_3DClass ui;
};

#endif // SON_3D_H
