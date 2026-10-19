#include <stdio.h>
#include <math.h>
#include "bsp_tree.h"

#define EPS 1e-9
static int tap_no;
static int tap_fails;
#define ok(c, name) do { int r_ = (c); \
	printf("%s %d - %s\n", r_ ? "ok" : "not ok", ++tap_no, name); tap_fails += !r_; } while (0)

/* counter clockwise unit square in the z = 0 plane, used as the root polygon */
static const son_point3d_t SQ[4] = {{{0, 0, 0}}, {{1, 0, 0}}, {{1, 1, 0}}, {{0, 1, 0}}};
static const son_point3d_t FRONT3[3] = {{{0, 0, 1}}, {{1, 0, 1}}, {{0, 1, 1}}};
static const son_point3d_t BACK3[3] = {{{0, 0, -1}}, {{1, 0, -1}}, {{0, 1, -1}}};
static const son_point3d_t SPAN3[3] = {{{0, 0, 2}}, {{0, 0, -2}}, {{1, 0, -2}}};

/* the polygon stored at the head of @node, or NULL when @node holds none */
static son_polygon3d_t *held(const bsp_node *node)
{
	if (!node || node->polygons.count == 0) return NULL;
	return &((polygon3d_node *)head_node(&node->polygons))->polygon3d;
}

static int v_eq(const son_polygon3d_t *poly, unsigned int i, double x, double y, double z)
{
	son_point3d_t *g = poly ? polygon3d_get_point(poly, i) : NULL;
	return g && fabs(g->v[0] - x) <= EPS && fabs(g->v[1] - y) <= EPS && fabs(g->v[2] - z) <= EPS;
}

/* fill @poly with @n points */
static void make(son_polygon3d_t *poly, const son_point3d_t *pts, unsigned short n)
{
	polygon3d_init(poly);
	polygon3d_add_points(poly, pts, n);
}

/* fresh heap tree whose root holds the unit square */
static bsp_node *rooted(void)
{
	bsp_node *tree = NULL;
	son_polygon3d_t sq;

	make(&sq, SQ, 4);
	bsp_add(&tree, &sq);
	polygon3d_clear(&sq);
	return tree;
}

int main(void)
{
	bsp_node stack_tree, *tree, *node;
	son_polygon3d_t poly;
	son_bool_t rc;

	printf("1..27\n");

	ok(bsp_init(NULL) == son_FALSE, "bsp_init on a NULL pointer returns son_FALSE");
	stack_tree.front = stack_tree.back = &stack_tree;
	ok(bsp_init(&stack_tree) == son_TRUE, "bsp_init on a real node returns son_TRUE");
	ok(stack_tree.front == NULL && stack_tree.back == NULL, "bsp_init clears both branches");
	ok(stack_tree.polygons.count == 0, "bsp_init starts with no polygon");
	ok(bsp_empty(&stack_tree) == son_TRUE, "bsp_empty is son_TRUE for a freshly initialized node");
	ok(bsp_empty((bsp_node *)NULL) == son_TRUE, "bsp_empty is son_TRUE for a NULL tree");

	/* add into an already initialized node: no root is allocated, rc stays son_FALSE */
	node = (bsp_node *)malloc(sizeof(bsp_node));
	bsp_init(node);
	make(&poly, SQ, 4);
	rc = bsp_add(&node, &poly);
	polygon3d_clear(&poly);
	ok(rc == son_FALSE, "bsp_add into an empty node returns son_FALSE");
	ok(node->polygons.count == 1, "bsp_add into an empty node stores one polygon");
	ok(node->front == NULL && node->back == NULL, "bsp_add into an empty node grows no branch");
	ok(bsp_empty(node) == son_FALSE, "bsp_empty is son_FALSE once a polygon is stored");
	ok(held(node)->n_ver == 4 && v_eq(held(node), 2, 1, 1, 0),
		"bsp_add copies every vertex of the polygon");
	ok(bsp_clear(node) == son_TRUE, "bsp_clear on a node with one polygon returns son_TRUE");

	tree = NULL;
	make(&poly, SQ, 4);
	rc = bsp_add(&tree, &poly);
	polygon3d_clear(&poly);
	ok(rc == son_FALSE, "bsp_add through a NULL tree pointer returns son_FALSE");
	ok(tree != NULL && tree->polygons.count == 1 && tree->front == NULL && tree->back == NULL,
		"bsp_add through a NULL tree pointer allocates the root node");

	/* front */
	make(&poly, FRONT3, 3);
	rc = bsp_add(&tree, &poly);
	polygon3d_clear(&poly);
	ok(rc == son_TRUE, "bsp_add of a polygon in front returns son_TRUE");
	ok(tree->polygons.count == 1 && tree->back == NULL && tree->front != NULL &&
		tree->front->polygons.count == 1, "a polygon in front goes to the front branch only");
	ok(held(tree->front)->n_ver == 3 && v_eq(held(tree->front), 0, 0, 0, 1),
		"the front branch holds the front half of the polygon");
	bsp_clear(tree);

	/* back */
	tree = rooted();
	make(&poly, BACK3, 3);
	bsp_add(&tree, &poly);
	polygon3d_clear(&poly);
	ok(tree->polygons.count == 1 && tree->front == NULL && tree->back != NULL &&
		tree->back->polygons.count == 1, "a polygon behind goes to the back branch only");
	ok(held(tree->back)->n_ver == 3 && v_eq(held(tree->back), 0, 0, 0, -1),
		"the back branch holds the back half of the polygon");
	bsp_clear(tree);

	/* coincident */
	tree = rooted();
	make(&poly, SQ, 4);
	rc = bsp_add(&tree, &poly);
	polygon3d_clear(&poly);
	ok(rc == son_TRUE, "bsp_add of a coincident polygon returns son_TRUE");
	ok(tree->polygons.count == 2 && tree->front == NULL && tree->back == NULL,
		"a coincident polygon is stored at the same node");
	bsp_clear(tree);

	/* spanning */
	tree = rooted();
	make(&poly, SPAN3, 3);
	rc = bsp_add(&tree, &poly);
	polygon3d_clear(&poly);
	ok(rc == son_TRUE, "bsp_add of a spanning polygon returns son_TRUE");
	ok(tree->polygons.count == 1 && tree->front != NULL && tree->back != NULL &&
		tree->front->polygons.count == 1 && tree->back->polygons.count == 1,
		"a spanning polygon is split over both branches");
	ok(held(tree->front)->n_ver == 3 && held(tree->back)->n_ver == 4 &&
		v_eq(held(tree->front), 0, 0.5, 0, 0) && v_eq(held(tree->back), 3, 1, 0, -2),
		"both halves of a spanning polygon keep the crossing point");
	ok(bsp_clear(tree) == son_TRUE, "bsp_clear on a tree with branches returns son_TRUE");

	ok(bsp_clear(NULL) == son_FALSE, "bsp_clear on a NULL tree returns son_FALSE");
	bsp_init(&stack_tree);
	ok(bsp_clear(&stack_tree) == son_FALSE, "bsp_clear on an empty node returns son_FALSE");

	return tap_fails == 0 ? 0 : 1;
}
