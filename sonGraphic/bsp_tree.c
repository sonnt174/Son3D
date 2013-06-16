#include "bsp_tree.h"

/******************************************************************************
** Name: bsp_init
** Params:
** Return:
** Descriptions:
**			Initialize a bsp tree
*******************************************************************************/
son_bool_t bsp_init(bsp_node *tree)
{
	if (!tree) return son_FALSE;
	tree->front = NULL;
	tree->back = NULL;
	init_list(&tree->polygons);

	return son_TRUE;
}

/******************************************************************************
** Name: bsp_add
** Params:
** Return:
** Descriptions:
**			Add one node to the tree
*******************************************************************************/
son_bool_t bsp_add(bsp_node **tree, const son_polygon3d_t *polygon)
{
	son_bool_t rc = son_FALSE;
	polygon3d_node *poly_node;
	bsp_node *n;
	classi_poly_plane_t classi;
	son_polygon3d_t front, back;
	node2 *h_poly;

	polygon3d_init(&front);
	polygon3d_init(&back);
	memcpy(front.color, polygon->color, sizeof(polygon->color));
	memcpy(back.color, polygon->color, sizeof(polygon->color));

	if (son_TRUE == bsp_empty(*tree)) {
		if (NULL == *tree) {
			*tree = (bsp_node *)malloc(sizeof(bsp_node));
			bsp_init(*tree);
		}
		poly_node = (polygon3d_node *)malloc(sizeof(polygon3d_node));
		polygon3d_init(&poly_node->polygon3d);
		polygon3d_cpy(polygon, &poly_node->polygon3d);
		add_2_tail(&(*tree)->polygons, (node2*)(poly_node));
	} else {
		h_poly = head_node(&(*tree)->polygons);
		rc = polygon3d_classify(&((polygon3d_node *)h_poly)->polygon3d, 
								polygon, &classi, &front, &back);

		switch (classi) {
			case CLASSI_POLY_BACK:
					bsp_add(&((*tree)->back), &back);
				break;
			case CLASSI_POLY_FRONT:
					bsp_add(&(*tree)->front, &front);
				break;
			case CLASSI_POLY_COINCIDE:
				{
					poly_node = (polygon3d_node *)malloc(sizeof(polygon3d_node));
					polygon3d_init(&poly_node->polygon3d);
					rc = polygon3d_cpy(polygon, &poly_node->polygon3d);
					add_2_tail(&(*tree)->polygons, (node2*)(poly_node));
				}
				break;
			case CLASSI_POLY_SPANNING:
				{
					if (front.n_ver) {
						bsp_add(&(*tree)->front, &front);
					}

					if (back.n_ver) {
						bsp_add(&(*tree)->back, &back);
					}
				}
				break;

			case CLASSI_POLY_UNKNOWN:
			default:
				break;
		}
		
		if (front.n_ver)
			polygon3d_clear(&front);
		if (back.n_ver)
			polygon3d_clear(&back);
	}

	return rc;
}

/******************************************************************************
** Name: bsp_clear
** Params:
** Return:
** Descriptions:
**			Clear all polygon in tree
*******************************************************************************/
son_bool_t bsp_clear(bsp_node *tree)
{
	bsp_node *nxt = tree;
	polygon3d_node *pol_node, *tmp;
	unsigned short n, i;

	if (NULL == nxt || nxt->polygons.count == 0) 
		return son_FALSE;
	
	bsp_clear(nxt->back);
	bsp_clear(nxt->front);

	n = nxt->polygons.count;
	pol_node = (polygon3d_node *)head_node(&nxt->polygons);

	for (i = 1; i < n; ++i) {
		tmp = pol_node;
		pol_node = \
			(polygon3d_node *)next_node(&nxt->polygons, (node2 *)pol_node);
		polygon3d_clear(&tmp->polygon3d);
		free(tmp);
		nxt->polygons.count--;
	}

	polygon3d_clear(&pol_node->polygon3d);
	nxt->polygons.count--;
	free(pol_node);

	
	free(tree);
	return son_TRUE;
}