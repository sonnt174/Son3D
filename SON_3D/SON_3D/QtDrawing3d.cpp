#include "QtDrawing3d.h"
#include "stack"

QtDrawing3d::QtDrawing3d(void):
						ISystemDrawing(),
						painter(NULL)
{
	m_n.v[0] = 0.0f;
	m_n.v[1] = 0.0f;
	m_n.v[2] = 1.0f;

	m_p.v[0] = 0.0f;
	m_p.v[1] = 0.0f;
	m_p.v[2] = 100.0f;

	m_color = QColor(Qt::blue);
}

QtDrawing3d::QtDrawing3d(QPainter *pPainter, SON3DView *view)
{
	m_n.v[0] = 0.0f;
	m_n.v[1] = 0.0f;
	m_n.v[2] = 1.0f;

	m_p.v[0] = 0.0f;
	m_p.v[1] = 0.0f;
	m_p.v[2] = 100.0f;

	painter = pPainter;
	refView = view;
	m_color = QColor(Qt::blue);
	eye.v[0] = 0;
	eye.v[1] = 0;
	eye.v[2] = 7000;
	side_eye = 0;
	//QtDrawing3d::Look ();
}

void	QtDrawing3d::Look ()						//	assign the viewing parameters
{																																								//	begin												//	compute the tangent of the field of view half-angle
	double distance = 1;																						//	compute the distance from the eye to the view plane_3d
	son_vector3d_t	vpn,u,v,tmp,tmp1;
	son_point3d_t vrp;

	unit_vector3d(&eye, &vpn);
	tmp.v[0] = 0.0; tmp.v[1] = 1.0; tmp.v[2] = 0.0;
	cross_product_3d(&tmp, &vpn, &tmp1);
	unit_vector3d(&tmp1, &u);
	cross_product_3d(&vpn, &u, &v);//	calculate the y' axis vector_3d
	mult_vector3d(&vpn, -distance);
	add_2_point3d(&eye, &vpn, &vrp);
	
	//viewing = ViewMatrix (u, v, vpn, vrp) * Perspective (distance);								//	set up the viewing transformation matrix_3d
	/* View matrix */
	double dot = 0;
	m_modelViewMatrix.m[0][0] = u.v[0];																															//	assign the first column
	m_modelViewMatrix.m[1][0] = u.v[1];
	m_modelViewMatrix.m[2][0] = u.v[2];
	
	dot_product_3d(&vrp, &u, &dot);
	m_modelViewMatrix.m[3] [0] = -(dot);

	m_modelViewMatrix.m[0][1] = v.v[0];																															//	assign the second column
	m_modelViewMatrix.m[1][1] = v.v[1];
	m_modelViewMatrix.m[2][1] = v.v[2];
	dot_product_3d(&vrp, &v, &dot);
	m_modelViewMatrix.m[3][1] = -(dot);

	m_modelViewMatrix.m[0][2] = vpn.v[0];																															//	assign the third column
	m_modelViewMatrix.m[1][2] = vpn.v[1];
	m_modelViewMatrix.m[2][2] = vpn.v[2];
	dot_product_3d(&vrp, &vpn, &dot);
	m_modelViewMatrix.m[3][2] = -(dot);

	m_modelViewMatrix.m[0][3] = (0.0);																														//	assign the fourth column
	m_modelViewMatrix.m[1][3] = (0.0);
	m_modelViewMatrix.m[2][3] = (0.0);
	m_modelViewMatrix.m[3][3] = (1.0);
}	

QtDrawing3d::~QtDrawing3d(void)
{
}

void QtDrawing3d::DrawPoint(const son_point3d_t &)
{

}

void QtDrawing3d::DrawLine(const son_point3d_t & startP, 
						   const son_point3d_t &endP)
{

}

void QtDrawing3d::DrawTriangle(const son_point3d_t &, 
							   const son_point3d_t &, 
							   const son_point3d_t &)
{

}

void QtDrawing3d::DrawPolygon(const son_point3d_t *points, unsigned num_point, unsigned char *color)
{
	son_polygon3d_t polygon;
	
	polygon3d_init(&polygon);
	memcpy(&polygon.color[0], &color[0], sizeof(unsigned char)* 3);
	polygon3d_add_points(&polygon, points, num_point);

	bsp_add(&s_world, &polygon);

	polygon3d_clear(&polygon);
}

void QtDrawing3d::DrawSence(const bsp_node *tree, QPainter *painter)
{
	
	polygon3d_node *pn;
	std::stack<polygon3d_node *> drawingStack;
	const bsp_node *nxt = tree;

	if (bsp_empty(tree)) {
		return;
	}

		pn = (polygon3d_node *)head_node(&tree->polygons);
		side_eye = point3d_classify_poly(&eye, &pn->polygon3d);
	//drawingStack.push(pn);
	//while(!drawingStack.empty()) {
	//	if (nxt->front) {
	//		pn = (polygon3d_node *)head_node(&(tree->front)->polygons);
	//		nxt = tree->front;
	//	}
	//}
	if (side_eye > 0) {
		DrawSence(tree->back, painter);
		DrawPolygon(tree, painter);
		DrawSence(tree->front, painter);
	} else if (side_eye < 0){
		DrawSence(tree->front, painter);
		DrawPolygon(tree, painter);
		DrawSence(tree->back, painter);
	} else {
		DrawSence(tree->back, painter);
		DrawSence(tree->front, painter);
	}
}

void QtDrawing3d::clear_node()
{
	bsp_clear(s_world);
}

void QtDrawing3d::DrawSence(QPainter *painter)
{
	if (bsp_empty(s_world)) 
		return;

	polygon3d_node * pn = (polygon3d_node *)head_node(&s_world->polygons);
	side_eye = point3d_classify_poly(&eye, &pn->polygon3d);
	DrawSence(s_world, painter);
}

void QtDrawing3d::DrawPolygon(const bsp_node *tree, QPainter *painter)
{
	if (!tree) 
		return;

	polygon3d_node *pn;
	unsigned short n_pol = tree->polygons.count;
	son_point3d_t points[MAX_POINT_PER_POLY];
	unsigned short i,j;
	son_point3d_t prjPnt[MAX_POINT_PER_POLY];
	static int colorflag = 0; 
	double shade;

	QPainterPath path;
	son_point3d_t mv;
	pn = (polygon3d_node *)head_node(&tree->polygons);

	for (j = 0; j < n_pol; j++) {
		polygon3d_get_points(&pn->polygon3d, points);
		matrix4_mult_vector(&m_modelViewMatrix, &points[0], &mv);
		proj_point_2_plane(&mv,
			&m_n,
			&m_p, &prjPnt[0]);
		path.moveTo(prjPnt[0].v[0], prjPnt[0].v[1]);
	
		for (i = 1; i < pn->polygon3d.n_ver; ++i) {
			matrix4_mult_vector(&m_modelViewMatrix, &points[i], &mv);
			proj_point_2_plane(&mv,
				&m_n,
				&m_p, &prjPnt[i]);
			path.lineTo(prjPnt[i].v[0], prjPnt[i].v[1]);
		}
		path.closeSubpath();
	
		shade = ComputeLight(&pn->polygon3d);
		m_color = QColor(pn->polygon3d.color[0] * shade, 
						 pn->polygon3d.color[1] * shade, 
						 pn->polygon3d.color[2] * shade);
		painter->fillPath(path, QBrush(m_color));

		pn = (polygon3d_node *)next_node(&tree->polygons, (node2 *)pn);
	}
}

double QtDrawing3d::ComputeLight(const son_polygon3d_t *polygon) {
	son_vector3d_t n;
	son_point3d_t p;
	double dot;
	static	son_vector3d_t light ={(400.0), (800.0), (600.0)};	//	lighting vector
	unit_vector3d(&light, &light);
	polygon3d_get_plane(polygon, &n, &p);
	matrix4_mult_vector(&m_modelViewMatrix, &n, &n);
	dot_product_3d(&n, &light, &dot);

	double	shade = (dot) * (0.8) + (0.2);  //	compute the lighting on this polygon
	if (shade < (0.3)) shade = (0.3);  //	clamp it to the ambient factor
	if (shade > (0.9)) shade = (0.9);  //	and no brighter than white
	return shade;
}