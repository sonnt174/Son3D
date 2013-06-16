#ifndef __POINT3D_H__
#define __POINT3D_H__

#include "stdlib.h"
#include "assert.h"

#ifndef __POLYGON_H__
#include "polygon.h"
#endif

#ifndef __SON_COMMON_TYPE_H__
#include "son_common_type.h"
#endif

#ifdef __cplusplus
extern "C" {
#endif

extern void vector3d_magnitude2(const son_vector3d_t *vect3d, 
								double *magnitude2);
extern void vector3d_magnitude(const son_vector3d_t *vect3d, 
							   double *magnitude);

/******************************************************************************
** Name: x_val
** Params:
** Return: 
** Descriptions:
**
*******************************************************************************/
double x_val(const son_point3d_t *my_point3d);

/******************************************************************************
** Name: y_val
** Params:
** Return: 
** Descriptions:
**
*******************************************************************************/
double y_val(const son_point3d_t *my_point3d);

/******************************************************************************
** Name: z_val
** Params:
** Return: 
** Descriptions:
**
*******************************************************************************/
double z_val(const son_point3d_t *my_point3d);

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
			   son_point3d_t *result);

/******************************************************************************
** Name: minus_2_point
** Params:
** Return:
** Descriptions:
**
*******************************************************************************/
void minus_2_point3d(const son_point3d_t *left_point,
				    const son_point3d_t *right_point,
				    son_point3d_t *result);

/******************************************************************************
** Name: is_same_point
** Params:
** Return:
** Descriptions:
**
*******************************************************************************/
son_bool_t is_same_point3d(const son_point3d_t *point1,
				   const son_point3d_t *point2);

/******************************************************************************
** Name: point_offset
** Params:
** Return:
** Descriptions:
**
*******************************************************************************/
void point3d_offset(son_point3d_t *orig_point,
				    double offset_val);

/******************************************************************************
** Name: duplicate_point
** Params:
** Return:
** Descriptions:
**
*******************************************************************************/
void duplicate_point3d(const son_point3d_t *orig_point,
					 son_point3d_t *replicate);

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
							son_point3d_t *prjPnt);

/******************************************************************************
** Name: duplicate_point
** Params:
** Return: TRUE if project successfully
** Descriptions:
**			This function get the distance from one point to a plane given 
**			by normal vector n and point p inside the plane
*******************************************************************************/
son_bool_t dis_point_2_plane(const son_point3d_t *p0,
							 const son_vector3d_t *n,
							 const son_point3d_t *p,
							 double *dis);

/******************************************************************************
** Name: duplicate_point
** Params:
** Return:
** Descriptions:
**
*******************************************************************************/
void point3d_rotatev(son_point3d_t *p,
					 const son_vector3d_t *v,
					 double theta);

/******************************************************************************
** Name: duplicate_point
** Params:
** Return:
** Descriptions:
**
*******************************************************************************/
void point3d_rotate(son_point3d_t *p,
					double x, double y, double z,
					double theta);

/******************************************************************************
** Name: point3d_classify
** Params:
** Return:
** Descriptions:
**			classify the point @p with given plane of normal vector @n and pass
**				through point @m
*******************************************************************************/
double point3d_classify(const son_point3d_t *,
						const son_vector3d_t *,
						const son_point3d_t *);

/******************************************************************************
** Name: point3d_classify
** Params:
** Return:
** Descriptions:
**			classify the point @p with given plane of normal vector @n and pass
**				through point @m
*******************************************************************************/
double point3d_classify_poly(const son_point3d_t *point,
							  const son_polygon3d_t *poly);

#ifdef __cplusplus
}
#endif

#endif