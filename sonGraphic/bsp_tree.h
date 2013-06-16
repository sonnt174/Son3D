#ifndef __BSP_TREE_H__
#define __BSP_TREE_H__

#include "stdlib.h"

#ifndef __LINK_2_H__
#include "link2.h"
#endif

#ifndef __POLYGON_H__
#include "polygon.h"
#endif


#ifdef __cplusplus
extern "C" {
#endif

typedef struct _polygon3d_node{
	node2 link;
	son_polygon3d_t polygon3d;
} polygon3d_node;

typedef struct _bsp_node{
	list_header polygons;  /* header of list of polygons */
	struct _bsp_node *front;
	struct _bsp_node *back;
}bsp_node;

#define bsp_empty(tree) (NULL == (tree) || (tree)->polygons.count == 0 \
						 ? son_TRUE : son_FALSE)


/******************************************************************************
** Name: bsp_init
** Params:
** Return:
** Descriptions:
**			Initialize a bsp tree
*******************************************************************************/
son_bool_t bsp_init(bsp_node *tree);

/******************************************************************************
** Name: bsp_add
** Params:
** Return:
** Descriptions:
**			Add one node to the tree
*******************************************************************************/
son_bool_t bsp_add(bsp_node **tree, const son_polygon3d_t *polygon);

/******************************************************************************
** Name: bsp_clear
** Params:
** Return:
** Descriptions:
**			Clear all polygon in tree
*******************************************************************************/
son_bool_t bsp_clear(bsp_node *tree);

#ifdef __cplusplus
}
#endif

#endif