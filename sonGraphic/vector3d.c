#include "vector3d.h"
#include "math.h"


/******************************************************************************
** Name: mult_vector3d
** Params:
** Return:
** Descriptions:
**
*******************************************************************************/
void mult_vector3d(son_vector3d_t *vect3d,
				   double k)
{
	vect3d->v[0] = k * vect3d->v[0];
	vect3d->v[1] = k * vect3d->v[1];
	vect3d->v[2] = k * vect3d->v[2];
}

/******************************************************************************
** Name: div_vector3d
** Params:
** Return:
** Descriptions:
**
*******************************************************************************/
void div_vector3d(son_vector3d_t *vect3d,
				  double k)
{
	assert(k != 0);
	vect3d->v[0] = vect3d->v[0] / k;
	vect3d->v[1] = vect3d->v[1] / k;
	vect3d->v[2] = vect3d->v[2] / k;
}

/******************************************************************************
** Name: dot_product_3d
** Params:
** Return:
** Descriptions:
**
*******************************************************************************/
void dot_product_3d(const son_vector3d_t *left, 
					const son_vector3d_t *right,
					double *dot)
{
	*dot = left->v[0] * right->v[0] + \
		   left->v[1] * right->v[1] + \
		   left->v[2] * right->v[2];
}

/******************************************************************************
** Name: cross_product_3d
** Params:
** Return:
** Descriptions:
**				|i	 j	 k |
**	cross = 	|x1	 y1	 z1|
**				|x2	 y2	 z2|
*******************************************************************************/
void cross_product_3d(const son_vector3d_t *left, 
					  const son_vector3d_t *right,
					  son_vector3d_t *result)
{
	result->v[0] = left->v[1] * right->v[2] - left->v[2] * right->v[1];
	result->v[1] = left->v[2] * right->v[0] - left->v[0] * right->v[2];
	result->v[2] = left->v[0] * right->v[1] - left->v[1] * right->v[0];
}

/******************************************************************************
** Name: vector3d_magnitude
** Params:
** Return:
** Descriptions:
**
*******************************************************************************/
void vector3d_magnitude2(const son_vector3d_t *vect3d,
						 double *magnitude2)
{
	*magnitude2 = vect3d->v[0] * vect3d->v[0] +
				  vect3d->v[1] * vect3d->v[1] +
				  vect3d->v[2] * vect3d->v[2];
}
/******************************************************************************
** Name: vector3d_magnitude
** Params:
** Return:
** Descriptions:
**
*******************************************************************************/
void vector3d_magnitude(const son_vector3d_t *vect3d,
						double *magnitude)
{
	*magnitude = sqrt(vect3d->v[0] * vect3d->v[0] +
					  vect3d->v[1] * vect3d->v[1] +
					  vect3d->v[2] * vect3d->v[2]);
}

/******************************************************************************
** Name: unit_vector3d
** Params:
** Return:
** Descriptions:
**
*******************************************************************************/
void unit_vector3d(const son_vector3d_t *vect3d,
				   son_vector3d_t *unit_vect3d)
{
	double magnitude = 0;
	duplicate_vector3d(vect3d, unit_vect3d);
	vector3d_magnitude(vect3d, &magnitude);
	div_vector3d(unit_vect3d, magnitude);

} 

/******************************************************************************
** Name: vector3d_inverse
** Params:
** Return:
** Descriptions:
**
*******************************************************************************/
void vector3d_inverse(const son_vector3d_t *vect3d,
					  son_vector3d_t *rev_3d)
{
	double magnitude = 0;
	rev_3d->v[0] = - vect3d->v[0];
	rev_3d->v[1] = - vect3d->v[1];
	rev_3d->v[2] = - vect3d->v[2];
}

/******************************************************************************
** Name: vector3d_make
** Params:
** Return:
** Descriptions:
**
*******************************************************************************/
void vector3d_make(const son_point3d_t *p1,
				   const son_point3d_t *p2,
				   son_vector3d_t *v)
{
	v->v[0] = p2->v[0] - p1->v[0];
	v->v[1] = p2->v[1] - p1->v[1];
	v->v[2] = p2->v[2] - p1->v[2];
}