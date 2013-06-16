#include "polygon.h"

/******************************************************************************
** Name: polygon3d_init
** Params:
** Return:
** Descriptions:
**			Initialize polygon @pol
*******************************************************************************/
son_bool_t polygon3d_init(son_polygon3d_t *pol)
{
	pol->n_ver = 0;
	init_list(&pol->vertex_list);
	pol->color[0] = pol->color[1] = pol->color[2] = 255;
	return son_TRUE;
}

/******************************************************************************
** Name: polygon3d_is_valid
** Params:
** Return:
** Descriptions:
**			Check to see a polygon is valid or not
*******************************************************************************/
son_bool_t polygon3d_is_valid(const son_polygon3d_t *pol)
{
	
	son_bool_t rc = son_TRUE;
	assert(pol);

	if (pol->n_ver < MIN_VERTEX_OF_POLYGON) {
		rc = son_FALSE;
	}

	return rc;
}

/******************************************************************************
** Name: polygon3d_get_plane
** Params:
** Return:
** Descriptions:
**			A plane is described by normal vector and a point lie on it
*******************************************************************************/
son_bool_t polygon3d_get_plane(const son_polygon3d_t *pol,
								son_vector3d_t *n,
								son_point3d_t *p)
{

	son_bool_t rc = son_TRUE;
	son_vector3d_t v1, v2;

	assert(pol && n && p);

	if (!polygon3d_is_valid(pol)) {
		rc = son_FALSE;
		goto exit;
	}

	vector3d_make(polygon3d_get_point(pol, 0), 
				  polygon3d_get_point(pol,1), &v1);

	vector3d_make(polygon3d_get_point(pol, 1), 
				  polygon3d_get_point(pol, 2), &v2);

	cross_product_3d(&v1, &v2, n);
	unit_vector3d(n, n);
	duplicate_point3d(polygon3d_get_point(pol, 0), p);

	exit:
	return rc;
}

/******************************************************************************
** Name: polygon3d_cpy
** Params:
** Return:
** Descriptions:
**			
*******************************************************************************/
son_bool_t polygon3d_cpy(const son_polygon3d_t *src,
						 son_polygon3d_t *dst)
{
	unsigned int n_ver = src->n_ver;
	node2 *n = NULL;

	init_list(&dst->vertex_list);
	
	/*copy vertext list*/
	n = head_node(&src->vertex_list);
	while (n_ver--) {
		son_point3d_t *np = &((point3d_node*)n)->point3d;
		polygon3d_add_point(dst, np);
		n = next_node(&src->vertex_list, n);
	}

	dst->n_ver = dst->vertex_list.count;
	/* color copy */
	memcpy(dst->color, src->color, sizeof(dst->color));

	return son_TRUE;
}

/******************************************************************************
** Name: polygon3d_add_point
** Params:
** Return:
** Descriptions:
**			Add a point to the polygon
*******************************************************************************/
son_bool_t polygon3d_add_point(son_polygon3d_t *poly,
							   const son_point3d_t *p)
{
	point3d_node *new_node = (point3d_node *)malloc(sizeof(point3d_node));
	duplicate_point3d(p, &new_node->point3d);
	add_2_tail(&poly->vertex_list, (node2 *)new_node);
	poly->n_ver = poly->vertex_list.count;

	return son_TRUE;
}

/******************************************************************************
** Name: polygon3d_add_points
** Params:
** Return:
** Descriptions:
**			Add a points to the polygon
*******************************************************************************/
son_bool_t polygon3d_add_points(son_polygon3d_t *poly,
								const son_point3d_t *p,
								unsigned short num_point)
{
	int i = 0;
	for (; i < num_point; ++i)
	{
		polygon3d_add_point(poly, &p[i]);
	}

	return son_TRUE;
}
/******************************************************************************
** Name: polygon3d_get_point
** Params:
** Return:
** Descriptions:
**			Get the point at index i
*******************************************************************************/
son_point3d_t *polygon3d_get_point(const son_polygon3d_t *h_poly,
									unsigned int i)
{
	node2 *n = NULL;
	unsigned int count = i;

	if (count >= h_poly->n_ver)
		return NULL;

	n = (node2 *)head_node(&h_poly->vertex_list);
	
	while(count) {
		n = next_node(&h_poly->vertex_list, n);
		count--;
	}

	return &((point3d_node *)n)->point3d;
}

/******************************************************************************
** Name: polygon3d_get_points
** Params:
** Return:
** Descriptions:
**			Get all point
*******************************************************************************/
son_bool_t polygon3d_get_points(const son_polygon3d_t *poly,
								son_point3d_t *points)
{
	unsigned short n_ver = poly->n_ver;
	point3d_node *pn;
	unsigned short i;
	pn = (point3d_node *)head_node(&poly->vertex_list);
	for (i = 0; i < n_ver; i++)
	{
		duplicate_point3d(&pn->point3d, &points[i]);
		pn = (point3d_node *)next_node(&poly->vertex_list, (node2 *)pn);
	}

	return son_TRUE;
}

/******************************************************************************
** Name: polygon3d_classify
** Params:
** Return:
** Descriptions:
**			Dose classification polygon with the plane of other polygon
*******************************************************************************/
son_bool_t polygon3d_classify(const son_polygon3d_t *h_poly,
							  const son_polygon3d_t *clss_poly,
							  classi_poly_plane_t *classi,
							  son_polygon3d_t *front,
							  son_polygon3d_t *back)
{
	/* hyperplane */
	son_bool_t rc;
	son_vector3d_t n, v;
	son_point3d_t p;
	son_point3d_t A, B;
	double sideA, sideB, dot, sect;
	unsigned short num_ver = clss_poly->n_ver;
	unsigned short i;
	unsigned char classi_flag = 0;

	polygon3d_get_plane(h_poly, &n, &p);

	/* 
	** get the first point
	*/
	duplicate_point3d(polygon3d_get_point(clss_poly, num_ver - 1), &A); 
	sideA = point3d_classify(&A, &n, &p);

	/* 
	** compare every two points each other in polygon
	*/
	for (i = 0; i < num_ver; ++i) {
		duplicate_point3d(polygon3d_get_point(clss_poly, i), &B);
		sideB = point3d_classify(&B, &n, &p);
		if (sideB < 0) {
			if (sideA > 0) {
				/* compute the intersection point of the line
				** from point A to point B with the partition
				** plane. This is a simple ray-plane intersection. 
				*/
				vector3d_make(&A, &B, &v);
				dot_product_3d(&n, &v, &dot);
				sect = - (point3d_classify(&A, &n, &p) / (dot));
				mult_vector3d(&v, sect);
				add_2_point3d(&A, &v, &v);
				polygon3d_add_point(front, &v);
				polygon3d_add_point(back, &v);
				
				classi_flag |= SPANNING_MASK;
			} else {
				classi_flag |= BACK_MASK;
			}
			polygon3d_add_point(back, &B);
		} else if (sideB > 0) {
			if (sideA < 0) {
				/* compute the intersection point of the line
				** from point A to point B with the partition
				** plane. This is a simple ray-plane intersection. 
				*/
				vector3d_make(&A, &B, &v);
				dot_product_3d(&n, &v, &dot);
				sect = - point3d_classify(&A, &n, &p) / (dot);
				mult_vector3d(&v, sect);
				add_2_point3d(&A, &v, &v);
				polygon3d_add_point(front, &v);
				polygon3d_add_point(back, &v);

				classi_flag |= SPANNING_MASK;
			} else {
				classi_flag |= FRONT_MASK;
			}

			polygon3d_add_point(front, &B);
		} else {
			classi_flag |= COINCIDE_MASK;
			polygon3d_add_point(front, &B);
			polygon3d_add_point(back, &B);
		}

		duplicate_point3d(&B, &A);
		sideA = sideB;
	}
	
	*classi = classi_flag & SPANNING_MASK ? CLASSI_POLY_SPANNING : \
			  classi_flag & FRONT_MASK ? CLASSI_POLY_FRONT : \
			  classi_flag & BACK_MASK ? CLASSI_POLY_BACK :\
			  classi_flag & COINCIDE_MASK ? CLASSI_POLY_COINCIDE :
			  CLASSI_POLY_UNKNOWN;

	return son_TRUE;
}

/******************************************************************************
** Name: polygon3d_clear
** Params:
** Return:
** Descriptions:
**			Clear all point inside polygon
*******************************************************************************/
son_bool_t polygon3d_clear(son_polygon3d_t *poly)
{
	unsigned short n = poly->vertex_list.count;
	unsigned short i;
	point3d_node *pn, *tmp;
	pn = (point3d_node*)head_node(&poly->vertex_list);
	
	for (i = 1; i < n; ++i) {
		tmp = pn;
		pn = (point3d_node *)next_node(&poly->vertex_list, (node2 *)pn);
		free(tmp);
	}
	free(pn);
}