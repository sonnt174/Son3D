#pragma once
#include "isystemdrawing.h"
#include "qpainter.h"

class SON3DView;

class QtDrawing3d :
	public ISystemDrawing
{
public:
	QtDrawing3d(void);
	QtDrawing3d(QPainter *pPainter, SON3DView *view);
	virtual ~QtDrawing3d(void);

	virtual void DrawPoint(const son_point3d_t &);
	virtual void DrawLine(const son_point3d_t &, const son_point3d_t &);
	virtual void DrawTriangle(const son_point3d_t &, const son_point3d_t &, const son_point3d_t &);
	virtual void DrawPolygon(const son_point3d_t *points, unsigned num_points, unsigned char *color);
	virtual void DrawSence(QPainter *painter);
	virtual void Look ();
	void clear_node();
	double QtDrawing3d::ComputeLight(const son_polygon3d_t *polygon);

private:
	virtual void DrawSence(const bsp_node *tree, QPainter *painter);
	virtual void DrawPolygon(const bsp_node *tree, QPainter *painter);

	QPainter *painter;
	SON3DView *refView;
	QColor m_color;
	/* screen plane*/
	son_vector3d_t m_n;
	son_point3d_t m_p;

	/* */
	son_vector3d_t m_rv;
};

