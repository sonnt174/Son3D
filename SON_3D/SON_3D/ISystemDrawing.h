#ifndef __ISYTEM_DRAWING_H__
#define __ISYTEM_DRAWING_H__

#pragma once
#include "stdlib.h"
#include "point3d.h"
#include "bsp_tree.h"
#include "matrix4x4.h"

class ISystemDrawing
{
public:
	ISystemDrawing(void);
	virtual ~ISystemDrawing(void);

	virtual void DrawPoint(const son_point3d_t &) = 0;
	virtual void DrawLine(const son_point3d_t &, const son_point3d_t &) = 0;
	virtual void DrawTriangle(const son_point3d_t &, const son_point3d_t &, const son_point3d_t &) = 0;
	virtual void DrawPolygon(const son_point3d_t *points, unsigned num_points, unsigned char *color = NULL) = 0;

	static bsp_node *s_world;
	static son_matrix4x4_t m_modelViewMatrix;
	static son_vector3d_t eye;
	double side_eye;
private:
	//son_polygon3d_t polyl;
};

#endif