#include <stdio.h>
#include <math.h>
#include "polygon.h"

#define EPS 1e-9
static int tap_no;
static int tap_fails;
#define ok(c, name) do { int r_ = (c); \
	printf("%s %d - %s\n", r_ ? "ok" : "not ok", ++tap_no, name); tap_fails += !r_; } while (0)

static int p_eq(const son_point3d_t *g, double x, double y, double z)
{
	return g && fabs(g->v[0] - x) <= EPS && fabs(g->v[1] - y) <= EPS && fabs(g->v[2] - z) <= EPS;
}

/* vertex @i of @poly compared against (x, y, z) */
static int v_eq(const son_polygon3d_t *poly, unsigned int i, double x, double y, double z)
{
	return p_eq(polygon3d_get_point(poly, i), x, y, z);
}

static void drop(son_polygon3d_t *f, son_polygon3d_t *b)
{
	if (f->vertex_list.count) polygon3d_clear(f);
	if (b->vertex_list.count) polygon3d_clear(b);
}

/* counter clockwise unit square in the z = 0 plane, used as the hyperplane */
static const son_point3d_t SQ[4] = {{{0, 0, 0}}, {{1, 0, 0}}, {{1, 1, 0}}, {{0, 1, 0}}};

/* classify @pts against the plane of @base, reporting the two halves */
static classi_poly_plane_t run(const son_polygon3d_t *base, const son_point3d_t *pts,
	unsigned short n, son_polygon3d_t *front, son_polygon3d_t *back, son_bool_t *rc)
{
	son_polygon3d_t poly;
	classi_poly_plane_t c = CLASSI_POLY_UNKNOWN;

	polygon3d_init(&poly);
	polygon3d_init(front);
	polygon3d_init(back);
	polygon3d_add_points(&poly, pts, n);
	*rc = polygon3d_classify(base, &poly, &c, front, back);
	polygon3d_clear(&poly);
	return c;
}

int main(void)
{
	son_polygon3d_t base, front, back;
	son_vector3d_t n;
	son_point3d_t p, got[4];
	classi_poly_plane_t classi;
	son_bool_t rc;
	static const son_point3d_t FRONT3[3] = {{{0, 0, 1}}, {{1, 0, 1}}, {{0, 1, 1}}};
	static const son_point3d_t BACK3[3] = {{{0, 0, -1}}, {{1, 0, -1}}, {{0, 1, -1}}};
	static const son_point3d_t SPAN3[3] = {{{0, 0, 2}}, {{0, 0, -2}}, {{1, 0, -2}}};

	printf("1..24\n");
	polygon3d_init(&base);
	ok(base.n_ver == 0 && base.vertex_list.count == 0 && base.color[0] == 255 &&
		base.color[1] == 255 && base.color[2] == 255, "polygon3d_init gives an empty white polygon");
	polygon3d_add_point(&base, &SQ[0]);
	polygon3d_add_point(&base, &SQ[1]);
	ok(polygon3d_is_valid(&base) == son_FALSE, "is_valid is FALSE with 2 vertices");
	ok(polygon3d_get_plane(&base, &n, &p) == son_FALSE, "get_plane returns son_FALSE on an invalid polygon");
	polygon3d_add_points(&base, &SQ[2], 2);
	ok(base.n_ver == 4 && base.vertex_list.count == 4, "add_point and add_points append and keep n_ver in sync");
	ok(polygon3d_is_valid(&base) == son_TRUE, "is_valid is TRUE with 4 vertices");
	ok(v_eq(&base, 0, 0, 0, 0) && v_eq(&base, 1, 1, 0, 0) && v_eq(&base, 3, 0, 1, 0), "get_point follows insertion order");
	ok(polygon3d_get_point(&base, 4) == NULL, "get_point out of range is NULL");
	polygon3d_get_points(&base, got);
	ok(p_eq(&got[0], 0, 0, 0) && p_eq(&got[2], 1, 1, 0) && p_eq(&got[3], 0, 1, 0), "get_points copies every vertex in order");
	n.v[0] = n.v[1] = n.v[2] = -9;
	rc = polygon3d_get_plane(&base, &n, &p);
	ok(rc == son_TRUE, "get_plane returns son_TRUE on a valid polygon");
	ok(p_eq(&n, 0, 0, 1), "get_plane normal of the CCW square points to +z");
	ok(p_eq(&p, 0, 0, 0), "get_plane uses vertex 0 as the plane point");
	classi = run(&base, FRONT3, 3, &front, &back, &rc);
	ok(rc == son_TRUE, "classify returns son_TRUE");
	ok(classi == CLASSI_POLY_FRONT, "a polygon on the +z side is FRONT");
	ok(front.n_ver == 3 && back.n_ver == 0, "FRONT puts all 3 vertices in front");
	drop(&front, &back);
	classi = run(&base, BACK3, 3, &front, &back, &rc);
	ok(classi == CLASSI_POLY_BACK, "a polygon on the -z side is BACK");
	ok(front.n_ver == 0 && back.n_ver == 3, "BACK puts all 3 vertices behind");
	drop(&front, &back);
	classi = run(&base, SPAN3, 3, &front, &back, &rc);
	ok(classi == CLASSI_POLY_SPANNING, "a polygon crossing the plane is SPANNING");
	ok(front.n_ver == 3 && back.n_ver == 4, "SPANNING yields 3 front and 4 back vertices");
	ok(v_eq(&front, 0, 0.5, 0, 0) && v_eq(&front, 1, 0, 0, 2) && v_eq(&front, 2, 0, 0, 0),
		"SPANNING front half starts at the first crossing point");
	ok(v_eq(&back, 0, 0.5, 0, 0) && v_eq(&back, 1, 0, 0, 0) && v_eq(&back, 2, 0, 0, -2) &&
		v_eq(&back, 3, 1, 0, -2), "SPANNING back half keeps both crossing points");
	drop(&front, &back);
	classi = run(&base, SQ, 4, &front, &back, &rc);
	ok(classi == CLASSI_POLY_COINCIDE, "the polygon of the plane itself is COINCIDE");
	ok(front.n_ver == 4 && back.n_ver == 4, "COINCIDE copies every vertex to both halves");
	ok(v_eq(&front, 0, 0, 0, 0) && v_eq(&back, 3, 0, 1, 0), "COINCIDE halves start at vertex 0, not the last one");
	drop(&front, &back);
	polygon3d_clear(&base);
	ok(base.n_ver == 4 && base.vertex_list.count == 4, "polygon3d_clear frees the nodes but leaves n_ver and count");
	return tap_fails == 0 ? 0 : 1;
}
