#include "son_3d.h"
#include "SON3DView.h"
#include <QtGui/QApplication>

#include "point3d.h"
#include "vector3d.h"
#include "matrix4x4.h"

#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

int main(int argc, char *argv[])
{
	_CrtSetDbgFlag ( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );

	QApplication a(argc, argv);
	/*SON_3D w;
	w.show();*/
	SON3DView mainView;
	mainView.show();

	return a.exec();
}