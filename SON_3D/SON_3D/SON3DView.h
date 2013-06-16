#ifndef SON_3D_VIEW_H
#define SON_3D_VIEW_H

#include <QWidget>
#include "ui_son_3d_view.h"
#include "point3d.h"
#include "matrix4x4.h"
#include "QtDrawing3d.h"

typedef enum {
	UNKNOWN = -1,
	MOUSE_LEFT_PRESSED,
	MOUSE_MIDLE_PRESSED,
	MOUSE_RIGHT_PRESSED
} mouse_pressed_t;

class SON3DView : public QWidget
{
	Q_OBJECT

public:
	SON3DView(QWidget *parent = 0);
	~SON3DView();
	void SON3DView::paintEvent(QPaintEvent *painter);
	void drawAxises(QPainter *painter);
	void SON3DView::drawGrid(float size, float step, QPainter *painter);
	void drawTriangle(QPainter *painter);
	void initTriangle();
	void rotate(double angle, const son_vector3d_t &v);
	void translate(double x, double y, double z);
	void SON3DView::scale(double factor);

	void SON3DView::drawLine3d(double x1, double y1, double z1,
		double x2, double y2, double z2,
		QPainter *painter);

	void mouseMoveEvent(QMouseEvent *);
	void mousePressEvent(QMouseEvent *);
	void wheelEvent(QWheelEvent *);
	void mouseReleaseEvent(QMouseEvent *);
	void drawCube(int i = 0);

private:
	Ui::son_3d_view ui;
	mouse_pressed_t m_mouse_prss_type;
	double m_lastX, m_lastY;
	double m_xRot, m_yRot;
	double m_translateX, m_translateY;
	double m_factor;

	son_point3d_t m_triangle[3];
	//son_matrix4x4_t m_modelViewMatrix;
	QtDrawing3d *qtDrawing3d;
	QPainter *m_painter;
	int dd;
};

#endif // SON_3D_VIEW_H
