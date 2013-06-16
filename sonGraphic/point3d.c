#include "point3d.h"
#include "math.h"

#ifndef __MATRIX4X4_H__
#include "matrix4x4.h"
#endif

/******************************************************************************
** Name: x_val
** Params:
** Return: 
** Descriptions:
**
*******************************************************************************/
double x_val(const son_point3d_t *my_point3d)
{
	assert(my_point3d);
	return my_point3d->v[0];
}

/******************************************************************************
** Name: y_val
** Params:
** Return: 
** Descriptions:
**
*******************************************************************************/
double y_val(const son_point3d_t *my_point3d)
{
	assert(my_point3d);
	return my_point3d->v[1];
}

/******************************************************************************
** Name: z_val
** Params:
** Return: 
** Descriptions:
**
*******************************************************************************/
double z_val(const son_point3d_t *my_point3d)
{
	assert(my_point3d);
	return my_point3d->v[2];
}

/******************************************************************************
** Name: add_2_point
** Params:
**		@left_point:
**		@right_point:
** Return: 
** Descriptions:
**
*******************************************************************************/
void add_2_point3d(const son_point3d_t *left_point, 
	const son_point3d_t *right_point,
	son_point3d_t *result)
{
	result->v[0] = left_point->v[0] + right_point->v[0];
	result->v[1] = left_point->v[1] + right_point->v[1];
	result->v[2] = left_point->v[2] + right_point->v[2];
}

/******************************************************************************
** Name: minus_2_point
** Params:
** Return:
** Descriptions:
**
*******************************************************************************/
void minus_2_point3d(const son_point3d_t *left_point,
	const son_point3d_t *right_point,
	son_point3d_t *result)
{
	result->v[0] = left_point->v[0] - right_point->v[0];
	result->v[1] = left_point->v[1] - right_point->v[1];
	result->v[2] = left_point->v[2] - right_point->v[2];
}

/******************************************************************************
** Name: is_same_point
** Params:
** Return:
** Descriptions:
**
*******************************************************************************/
son_bool_t is_same_point3d(const son_point3d_t *point1,
	const son_point3d_t *point2)
{
	return (point1->v[0] == point2->v[0] && 
			point1->v[1] == point2->v[1] && 
			point1->v[2] == point2->v[2]) ? son_TRUE : son_FALSE;
}

/******************************************************************************
** Name: point_offset
** Params:
** Return:
** Descriptions:
**
*******************************************************************************/
void point3d_offset(son_point3d_t *orig_point,
					double offset_val)
{
	orig_point->v[0] = orig_point->v[0] + offset_val;
	orig_point->v[1] = orig_point->v[1] + offset_val;
	orig_point->v[2] = orig_point->v[2] + offset_val;
}

/******************************************************************************
** Name: duplicate_point
** Params:
** Return:
** Descriptions:
**
*******************************************************************************/
void duplicate_point3d(const son_point3d_t *orig_point,
	son_point3d_t *replicate)
{
	memcpy(replicate, orig_point, sizeof(son_point3d_t));
}

/******************************************************************************
** Name: duplicate_point
** Params:
** Return: TRUE if project successfully
** Descriptions:
**			This function get the projection of one point on to a plane given 
**			by normal vector n and point p inside the plane
*******************************************************************************/
son_bool_t proj_point_2_plane(const son_point3d_t *orig_point,
						      const son_vector3d_t *n,
						      const son_point3d_t *p,
							  son_point3d_t *prjPnt)
{
	double t, n_mag2;

	vector3d_magnitude2(n, &n_mag2);

	t = (n->v[0] * p->v[0] - n->v[0] * orig_point->v[0]  + \
		 n->v[1] * p->v[1] - n->v[1] * orig_point->v[1]  + \
		 n->v[2] * p->v[2] - n->v[2] * orig_point->v[2]) / \
		 n_mag2;

	prjPnt->v[0] = orig_point->v[0] + t * n->v[0];
	prjPnt->v[1] = orig_point->v[1] + t * n->v[1];
	prjPnt->v[2] = orig_point->v[2] + t * n->v[2];

	return son_TRUE;
}

/******************************************************************************
** Name: duplicate_point
** Params:
** Return: TRUE if project successfully
** Descriptions:
**		This function get the distance from point p0(x0, y0, z0) to a plane given
**		by normal vector n(a,b,c) and point p(x,y,z) inside the plane
**			D = (|ax_0+by_0+cz_0+d|)/(sqrt(a^2+b^2+c^2))
*******************************************************************************/
son_bool_t dis_point_2_plane(const son_point3d_t *p0,
							 const son_vector3d_t *n,
							 const son_point3d_t *p,
							 double *dis)
{
	double d;
	double n_mag;

	vector3d_magnitude(n, &n_mag);
	if (0 == n_mag)
		return son_FALSE;

	d = -(n->v[0] * p->v[0] +
		  n->v[1] * p->v[1] +
		  n->v[2] * p->v[2]);
	
	*dis = fabs(n->v[0] * p0->v[0] + 
				n->v[1] * p0->v[1] + 
				n->v[2] * p0->v[2] + 
				d) / n_mag;
	
	return son_TRUE;
}

/******************************************************************************
** Name: point3d_rotatev
** Params:
** Return:
** Descriptions:
**
*******************************************************************************/
void point3d_rotatev(son_point3d_t *p,
					const son_vector3d_t *v,
					double theta)
{
	son_matrix4x4_t r; /* rotate matrix */

	/* Get the rotation matrix*/
	get_r_matrix4(v, theta, &r);

	/* multiply the rotation matrix and p */
	matrix4_mult_vector(&r, p, p);
}

/******************************************************************************
** Name: point3d_rotate
** Params:
** Return:
** Descriptions:
**
*******************************************************************************/
void point3d_rotate(son_point3d_t *p,
					double x, double y, double z,
					double theta)
{
	son_vector3d_t v = {x, y, z};
	point3d_rotatev(p, &v, theta);
}

/******************************************************************************
** Name: point3d_classify
** Params:
** Return:
** Descriptions:
**			classify the point @p with given plane of normal vector @n and pass
**				through point @m
*******************************************************************************/
double point3d_classify(const son_point3d_t *clpnt,
						const son_vector3d_t *n,
						const son_point3d_t *p)
{
	double rc = INVALID_32;
	double d;
	double n_mag;

	vector3d_magnitude(n, &n_mag);
	if (0 == n_mag)
		return rc;

	d = -(n->v[0] * p->v[0] +
		  n->v[1] * p->v[1] +
		  n->v[2] * p->v[2]);

	rc = n->v[0] * clpnt->v[0] +
		 n->v[1] * clpnt->v[1] +
		 n->v[2] * clpnt->v[2] +
		 d;
	
	return rc;
}

/******************************************************************************
** Name: point3d_classify
** Params:
** Return:
** Descriptions:
**			classify the point @p with given plane of normal vector @n and pass
**				through point @m
*******************************************************************************/
double point3d_classify_poly(const son_point3d_t *point,
							  const son_polygon3d_t *poly)
{
	son_vector3d_t n;
	son_point3d_t p;
	if (!poly || !point) return 0;
	polygon3d_get_plane(poly, &n, &p);
	return point3d_classify(point, &n, &p);
}