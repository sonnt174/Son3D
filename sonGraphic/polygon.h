#ifndef __POLYGON_H__
#define __POLYGON_H__

#include "stdlib.h"

#ifndef __POINT3D_H__
#include "point3d.h"
#endif

#ifndef __VECTOR3D_H__
#include "vector3d.h"
#endif

#include "assert.h"

#ifndef __SON_COMMON_TYPE_H__
#include "son_common_type.h"
#endif

#ifndef __LINK_2_H__
#include "link2.h"
#endif


#ifdef __cplusplus
extern "C" {
#endif

#define MIN_VERTEX_OF_POLYGON 3

typedef enum _classi_poly_plane_t {
	CLASSI_POLY_UNKNOWN = 0,
	CLASSI_POLY_FRONT,
	CLASSI_POLY_BACK,
	CLASSI_POLY_COINCIDE,
	CLASSI_POLY_SPANNING
}classi_poly_plane_t;

#define SPANNING_MASK 1
#define COINCIDE_MASK 1 << 1
#define FRONT_MASK    1 << 2
#define BACK_MASK	  1 << 3

/******************************************************************************
** Name: polygon3d_init
** Params:
** Return:
** Descriptions:
**			Initialize polygon @pol
*******************************************************************************/
son_bool_t polygon3d_init(son_polygon3d_t *pol);

/******************************************************************************
** Name: polygon3d_is_valid
** Params:
** Return:
** Descriptions:
**			Check to see a polygon is valid or not
*******************************************************************************/
son_bool_t polygon3d_is_valid(const son_polygon3d_t *pol);

/******************************************************************************
** Name: polygon3d_get_plane
** Params:
** Return:
** Descriptions:
**			A plane is described by normal vector and a point lie on it
*******************************************************************************/
son_bool_t polygon3d_get_plane(const son_polygon3d_t *pol,
							   son_vector3d_t *n,
							   son_point3d_t *p);

/******************************************************************************
** Name: polygon3d_cpy
** Params:
** Return:
** Descriptions:
**			
*******************************************************************************/
son_bool_t polygon3d_cpy(const son_polygon3d_t *src,
						 son_polygon3d_t *dst);

/******************************************************************************
** Name: polygon3d_get_point
** Params:
** Return:
** Descriptions:
**			Get the point at index i
*******************************************************************************/
son_point3d_t *polygon3d_get_point(const son_polygon3d_t *h_poly,
								   unsigned int i);

/******************************************************************************
** Name: polygon3d_get_points
** Params:
** Return:
** Descriptions:
**			Get all point
*******************************************************************************/
son_bool_t polygon3d_get_points(const son_polygon3d_t *poly,
									son_point3d_t *points);

/******************************************************************************
** Name: polygon3d_add_point
** Params:
** Return:
** Descriptions:
**			Add a point to the polygon
*******************************************************************************/
son_bool_t polygon3d_add_point(son_polygon3d_t *poly,
							   const son_point3d_t *np);

/******************************************************************************
** Name: polygon3d_add_points
** Params:
** Return:
** Descriptions:
**			Add a points to the polygon
*******************************************************************************/
son_bool_t polygon3d_add_points(son_polygon3d_t *poly,
								const son_point3d_t *np,
								unsigned short num_point);

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
	son_polygon3d_t *back);

/******************************************************************************
** Name: polygon3d_clear
** Params:
** Return:
** Descriptions:
**			Clear all point inside polygon
*******************************************************************************/
son_bool_t polygon3d_clear(son_polygon3d_t *poly);

#ifdef __cplusplus
}
#endif

#endif