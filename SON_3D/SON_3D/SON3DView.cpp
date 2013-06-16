#include "SON3DView.h"
#include "qpainter.h"
#include "point3d.h"
#include "vector3d.h"
#include "qevent.h"

SON3DView::SON3DView(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
	m_painter = &QPainter(this);
	qtDrawing3d = new QtDrawing3d(&QPainter(this), this);
	/* Set Background */
	QPalette bgPallete(QColor(0,0,0));
	setPalette(bgPallete);
	setAutoFillBackground(true);

	this->resize(800, 600);

	m_mouse_prss_type = mouse_pressed_t::UNKNOWN;
	dd = 10;
	/* Init triangle */
	initTriangle();

	/* drawcube */
	for (int i = 0; i < 10; i++)
		drawCube(i);

	m_lastX = 0;
	m_lastY = 0;
	m_xRot = 3*PI/4 * RAD2DEG;
	m_yRot = -PI/4 * RAD2DEG;//-PI/4;

	matrix4_identity(&QtDrawing3d::m_modelViewMatrix);
	son_vector3d_t vx = {1.0, 0.0, 0.0};
	son_vector3d_t vy = {0.0, 1.0, 0.0};
	rotate(m_xRot, vx);
	rotate(m_yRot, vy);

	m_translateX = m_translateY = 0;
	m_factor = 1.0;
}

SON3DView::~SON3DView()
{
	if (qtDrawing3d) {
		delete(qtDrawing3d);
	}
}

void SON3DView::initTriangle()
{
	int vl = 50;
	m_triangle[0].v[0] = vl;
	m_triangle[0].v[1] = 0;
	m_triangle[0].v[2] = 0;

	m_triangle[1].v[0] = 0;
	m_triangle[1].v[1] = vl;
	m_triangle[1].v[2] = 0;

	m_triangle[2].v[0] = 0;
	m_triangle[2].v[1] = 0;
	m_triangle[2].v[2] = vl;

	son_point3d_t tri[3];
	tri[0].v[0] = 0;
	tri[0].v[1] = 0;
	tri[0].v[2] = 0;

	tri[1].v[0] = 00;
	tri[1].v[1] = vl;
	tri[1].v[2] = vl;

	tri[2].v[0] = vl;
	tri[2].v[1] = vl;
	tri[2].v[2] = 00;
	
	unsigned char color10[3] = {255, 255, 0};
	//qtDrawing3d->DrawPolygon(m_triangle, 3, color10);
	unsigned char color20[3] = {0, 255, 255};
	//qtDrawing3d->DrawPolygon(tri, 3, color20);
}

void SON3DView::drawCube(int i)
{
	/* cube */
	 dd += i;
	son_point3d_t p3d0 = {0,0,0};
	son_point3d_t p3d1 = {0, dd, 0};
	son_point3d_t p3d2 = {dd, dd, 0};
	son_point3d_t p3d3 = {dd, 0, 0};
	son_point3d_t p3d4 = {0, 0, dd};
	son_point3d_t p3d5 = {0, dd, dd};
	son_point3d_t p3d6 = {dd, dd, dd};
	son_point3d_t p3d7 = {dd, 0, dd};

	son_point3d_t f[4];
	/* face 1*/
	duplicate_point3d(&p3d0, &f[0]);
	duplicate_point3d(&p3d1, &f[1]);
	duplicate_point3d(&p3d2, &f[2]);
	duplicate_point3d(&p3d3, &f[3]);
	unsigned char color1[3] = {255, 255, 0};
	qtDrawing3d->DrawPolygon(f, 4, color1);

	/* face 2*/
	duplicate_point3d(&p3d4, &f[0]);
	duplicate_point3d(&p3d5, &f[1]);
	duplicate_point3d(&p3d6, &f[2]);
	duplicate_point3d(&p3d7, &f[3]);
	unsigned char color2[3] = {0, 255, 0};
	qtDrawing3d->DrawPolygon(f, 4, color1);

	/* face 3*/
	duplicate_point3d(&p3d1, &f[0]);
	duplicate_point3d(&p3d5, &f[1]);
	duplicate_point3d(&p3d6, &f[2]);
	duplicate_point3d(&p3d2, &f[3]);
	unsigned char color3[3] = {0, 0, 255};
	qtDrawing3d->DrawPolygon(f, 4, color1);

	/* face 4*/
	duplicate_point3d(&p3d0, &f[0]);
	duplicate_point3d(&p3d3, &f[1]);
	duplicate_point3d(&p3d7, &f[2]);
	duplicate_point3d(&p3d4, &f[3]);
	unsigned char color4[3] = {255, 255, 255};
	qtDrawing3d->DrawPolygon(f, 4, color4);

	/* face 5*/
	duplicate_point3d(&p3d0, &f[0]);
	duplicate_point3d(&p3d1, &f[1]);
	duplicate_point3d(&p3d5, &f[2]);
	duplicate_point3d(&p3d4, &f[3]);
	unsigned char color5[3] = {255, 0, 100};
	qtDrawing3d->DrawPolygon(f, 4, color5);

	/* face 6*/
	duplicate_point3d(&p3d3, &f[0]);
	duplicate_point3d(&p3d2, &f[1]);
	duplicate_point3d(&p3d6, &f[2]);
	duplicate_point3d(&p3d7, &f[3]);
	unsigned char color6[3] = {255, 100, 0};
	qtDrawing3d->DrawPolygon(f, 4, color6);
}

void SON3DView::drawTriangle(QPainter *painter)
{
	QPoint p1, p2, p3;
	son_vector3d_t prj_pnt1, prj_pnt2, prj_pnt3;
	son_vector3d_t n = {0.0f, 0.0f, 1.0f}; /* normalize vector of OXY*/
	son_point3d_t p = {0.0f, 0.0f, 100.0f}; /* Original point*/
	son_vector3d_t mv[3];

	for (int i = 0; i < 3; i++)
		matrix4_mult_vector(&QtDrawing3d::m_modelViewMatrix, &m_triangle[i], &mv[i]);

	/* project 3d to OXY n(0, 0, 1)*/
	proj_point_2_plane(&mv[0], &n, &p, &mv[0]);
	proj_point_2_plane(&mv[1], &n, &p, &mv[1]);
	proj_point_2_plane(&mv[2], &n, &p, &mv[2]);

	/* map to QPoint*/
	p1.setX(mv[0].v[0]);
	p1.setY(mv[0].v[1]);
			
	p2.setX(mv[1].v[0]);
	p2.setY(mv[1].v[1]);
			
	p3.setX(mv[2].v[0]);
	p3.setY(mv[2].v[1]);

	QPainterPath triPath;
	triPath.moveTo(p1);
	triPath.lineTo(p2);
	triPath.lineTo(p3);
	triPath.lineTo(p1);
	//triPath.closeSubpath();
	/* draw 2d*/
	painter->fillPath(triPath, QBrush(QColor(Qt::blue)));
	painter->drawLine(p1, p2);
	painter->drawLine(p2, p3);
	painter->drawLine(p3, p1);

	update();
}

void SON3DView::drawLine3d(double x1, double y1, double z1,
							double x2, double y2, double z2,
							QPainter *painter)
{
	son_point3d_t p3d1 = {x1, y1, z1};
	son_point3d_t p3d2 = {x2, y2, z2};
	son_vector3d_t n = {0.0f, 0.0f, 1.0f}; /* normalize vector of OXY*/
	son_point3d_t p = {0.0f, 0.0f, 100.0f}; /* Original point*/

	QPoint p1, p2;
	matrix4_mult_vector(&QtDrawing3d::m_modelViewMatrix, &p3d1, &p3d1);
	matrix4_mult_vector(&QtDrawing3d::m_modelViewMatrix, &p3d2, &p3d2);
	
	/* project 3d to OXY n(0, 0, 1)*/
	proj_point_2_plane(&p3d1, &n, &p, &p3d1);
	proj_point_2_plane(&p3d2, &n, &p, &p3d2);

	p1.setX(p3d1.v[0]);
	p1.setY(p3d1.v[1]);
			
	p2.setX(p3d2.v[0]);
	p2.setY(p3d2.v[1]);

	painter->drawLine(p1, p2);
	
	update();
}

void SON3DView::drawAxises(QPainter *painter)
{
	painter->save();
	painter->setPen(QPen(Qt::yellow));

	// Draw X-axis
	painter->drawLine(0, -9999, 0, 9999);

	// Draw Y-axis
	painter->drawLine(-9999, 0, 9999, 0);
	painter->restore();
}

void SON3DView::drawGrid(float size, float step, QPainter *painter)
{
	for(float i=step; i <= size; i+= step)
	{
		drawLine3d(-size, 0,  i,   // lines parallel to X-axis
					size, 0,  i, painter);
		drawLine3d(-size, 0, -i,   // lines parallel to X-axis
					size, 0, -i, painter);

		drawLine3d( i, 0, -size,   // lines parallel to Z-axis
					i, 0,  size, painter);
		drawLine3d(-i, 0, -size,   // lines parallel to Z-axis
				   -i, 0,  size, painter);
	}
}

void SON3DView::paintEvent(QPaintEvent *event)
{
	
	QPainter painter(this);
	son_matrix4x4_t inverse, mm;
	son_point3d_t eye = {0.0, 0.0, 70000.0};

	matrix4_identity(&QtDrawing3d::m_modelViewMatrix);
	//qtDrawing3d->Look();
	son_vector3d_t vx = {width()/2.0, 0.0, 0.0};
	son_vector3d_t vy = {0.0, height()/2, 0.0};
	rotate(m_xRot, vx);
	rotate(m_yRot, vy);
	//translate(0, 0, eye.v[2]);
	translate(m_translateX, m_translateY, 0);
	

	painter.setPen(QPen(Qt::blue));
	painter.setBrush(Qt::blue);
	
	painter.translate(width()/2, height()/2);
	//this->drawAxises(&painter);
	//this->drawTriangle(&painter);
	painter.save();
	painter.setPen(QPen(Qt::red));
	//this->drawGrid(100, 10, &painter);
	painter.restore();
	
	matrix4_inverse(&QtDrawing3d::m_modelViewMatrix, &inverse);
	matrix4_mult_vector(&inverse, &eye, &QtDrawing3d::eye);
	//matrix4_mult(&inverse, &QtDrawing3d::m_modelViewMatrix, &mm);
	scale(m_factor*eye.v[2]/700);
	qtDrawing3d->DrawSence(&painter);
}

void SON3DView::rotate(double angle, const son_vector3d_t &v)
{	
	angle *= DEG2RAD;
	son_point3d_t rTri[3];

	son_matrix4x4_t r, r_mdlVwMatrix, i;

	matrix4_rotate(&QtDrawing3d::m_modelViewMatrix, &v, angle);
}

void SON3DView::translate(double x, double y, double z)
{
	son_matrix4x4_t t, translMatrix;
	matrix4_translate(&QtDrawing3d::m_modelViewMatrix, x, y, z);
}

void SON3DView::scale(double factor)
{
	son_matrix4x4_t scl_m;
	matrix4_scale(&QtDrawing3d::m_modelViewMatrix, factor);
}

void SON3DView::mouseMoveEvent(QMouseEvent *event)
{
	printf("%s", __FUNCTION__);
	double delX = event->posF().x() - m_lastX;
	double delY = event->posF().y() - m_lastY;
	
	if (m_mouse_prss_type == mouse_pressed_t::MOUSE_LEFT_PRESSED) // rotate
	{
		m_yRot += (delX);
		m_xRot += (delY);

	} else if (m_mouse_prss_type == mouse_pressed_t::MOUSE_RIGHT_PRESSED) {
		m_translateX += delX;
		m_translateY += delY;
	}

	m_lastX = event->posF().x();
	m_lastY = event->posF().y();

	update();
}

void SON3DView::mousePressEvent(QMouseEvent *event)
{
	printf("%s", __FUNCTION__);
	if (event->button() == Qt::RightButton) {
		m_mouse_prss_type = mouse_pressed_t::MOUSE_RIGHT_PRESSED;
	} else if (event->button() == Qt::LeftButton) {
		m_mouse_prss_type = mouse_pressed_t::MOUSE_LEFT_PRESSED;
	}

	m_lastX = event->posF().x();
	m_lastY = event->posF().y();
}

void SON3DView::mouseReleaseEvent(QMouseEvent *event)
{
	printf("%s", __FUNCTION__);
}

void SON3DView::wheelEvent(QWheelEvent *event)
{
	double numDegrees = event->delta() / 8.0;
	double numSteps = numDegrees / 15.0;
	QPoint point = event->globalPos();
	if (event->orientation() == Qt::Horizontal) {
		printf("%s", __FUNCTION__);
	} else {
		m_factor *= (float)pow( 2, ( numSteps ) * 0.1f );
		printf("%s", __FUNCTION__);
	}
	event->accept();
	update();
}