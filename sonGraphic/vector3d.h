#ifndef __VECTOR3D_H__
#define __VECTOR3D_H__

#ifndef __POINT3D_H__
#include "point3d.h"
#endif

#ifndef __SON_COMMON_TYPE_H__
#include "son_common_type.h"
#endif

#ifdef __cplusplus
extern "C" {
#endif

//typedef son_point3d_t son_vector3d_t;

#define add_2_vector3d(left, right, result)\
	add_2_point3d(left, right, result)

#define minus_2_vector3d(left, right, result)\
	minus_2_point3d(left, right, result)

#define is_same_vector3d(left, right)\
	is_same_point3d(left, right)

#define vector3d_offset(orig_vec3d, offset_val)\
	point3d_offset(orig_vec3d, offset_val)

#define duplicate_vector3d(orig_vec3d, replicate_vec3d)\
	duplicate_point3d(orig_vec3d, replicate_vec3d)

/******************************************************************************
** Name: mult_vector3d
** Params:
** Return:
** Descriptions:
**
*******************************************************************************/
void mult_vector3d(son_vector3d_t *vect3d,
				   double k);

/******************************************************************************
** Name: div_vector3d
** Params:
** Return:
** Descriptions:
**
*******************************************************************************/
void div_vector3d(son_vector3d_t *vect3d,
				  double k);

/******************************************************************************
** Name: dot_product_3d
** Params:
** Return:
** Descriptions:
**
*******************************************************************************/
void dot_product_3d(const son_vector3d_t *left, 
					const son_vector3d_t *right,
					double *result);

/******************************************************************************
** Name: cross_product_3d
** Params:
** Return:
** Descriptions:
**
*******************************************************************************/
void cross_product_3d(const son_vector3d_t *left, 
					  const son_vector3d_t *right,
					  son_vector3d_t *result);

/******************************************************************************
** Name: vector3d_magnitude
** Params:
** Return:
** Descriptions:
**
*******************************************************************************/
void vector3d_magnitude2(const son_vector3d_t *vect3d,
						double *magnitude2);

/******************************************************************************
** Name: vector3d_magnitude
** Params:
** Return:
** Descriptions:
**
*******************************************************************************/
void vector3d_magnitude(const son_vector3d_t *vect3d,
					    double *magnitude);

/******************************************************************************
** Name: unit_vector3d
** Params:
** Return:
** Descriptions:
**
*******************************************************************************/
void unit_vector3d(const son_vector3d_t *vect3d,
				   son_vector3d_t *unit_vect3d);

/******************************************************************************
** Name: vector3d_inverse
** Params:
** Return:
** Descriptions:
**
*******************************************************************************/
void vector3d_inverse(const son_vector3d_t *vect3d,
					  son_vector3d_t *rev_3d);

/******************************************************************************
** Name: vector3d_make
** Params:
** Return:
** Descriptions:
**
*******************************************************************************/
void vector3d_make(const son_point3d_t *p1,
	const son_point3d_t *p2,
	son_vector3d_t *v);
#ifdef __cplusplus
}
#endif

#endif