#ifndef __SON_COMMON_TYPE_H__
#define __SON_COMMON_TYPE_H__

#include "stdlib.h"
#include "link2.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum son_bool{ 
	son_FALSE = 0,
	son_TRUE
} son_bool_t;

typedef struct _son_point3d_t 
{
	double v[3];
} son_point3d_t;

typedef son_point3d_t son_vector3d_t;

typedef struct _point_node {
	node2 link;
	son_point3d_t point3d;
}point3d_node;

typedef struct polygon3d {
	list_header vertex_list;
	unsigned short n_ver;  /* number of vertex of polygon */
	unsigned char color[3];
} son_polygon3d_t;

#define PI 3.14159265359
#define DEG2RAD (PI/180)
#define DEG2RAD_(x) ((x)*DEG2RAD)
#define RAD2DEG (180/PI)
#define RAD2DEG_(x) ((x)*RAD2DEG)
#define INVALID_32 0xffffffff
#define MAX_POINT_PER_POLY 100

#define odd(x) ((x)%2)// != 0 ? 1 : 0) 

#ifdef __cplusplus
}
#endif

#endif