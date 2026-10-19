#include "son_3d.h"
#include "SON3DView.h"
#include <QApplication>

#include "point3d.h"
#include "vector3d.h"
#include "matrix4x4.h"

#ifdef _MSC_VER
#define _CRTDBG_MAP_ALLOC
#endif
#include <stdlib.h>
#ifdef _MSC_VER
#include <crtdbg.h>
#endif

int main(int argc, char *argv[])
{
#ifdef _MSC_VER
	_CrtSetDbgFlag ( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );
#endif

	QApplication a(argc, argv);
	/*SON_3D w;
	w.show();*/
	SON3DView mainView;
	mainView.show();

	return a.exec();
}