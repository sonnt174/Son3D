#include <stdio.h>
#include <math.h>
#include "point3d.h"

#define EPS 1e-9
static int tap_no;
static int tap_fails;
#define ok(c, name) do { int r_ = (c); \
	printf("%s %d - %s\n", r_ ? "ok" : "not ok", ++tap_no, name); tap_fails += !r_; } while (0)

/* every component is checked, so a wrong sign or operator in any of them fails */
static int p_eq(const son_point3d_t *g, double x, double y, double z)
{
	return fabs(g->v[0] - x) < EPS && fabs(g->v[1] - y) < EPS && fabs(g->v[2] - z) < EPS;
}

int main(void)
{
	/* plane: normal (1,2,2) (magnitude 3), through (1,0,0), so d = -1 */
	static const son_vector3d_t N = {{1, 2, 2}};
	static const son_vector3d_t ZERO = {{0, 0, 0}};
	static const son_point3d_t PL = {{1, 0, 0}};	/* on the plane */
	static const son_point3d_t ON = {{3, 1, -2}};	/* on the plane too */
	static const son_point3d_t FRONT = {{4, 5, 6}};	/* n.q + d = 25 */
	static const son_point3d_t BACK = {{-1, -1, -1}};	/* n.q + d = -6 */
	static const son_point3d_t A = {{2, 3, 5}};
	static const son_point3d_t B = {{7, 11, 13}};
	/* triangle in that same plane; its edges cross to (2,4,4) ~ (1,2,2) */
	static const son_point3d_t TRI[3] = {{{1, 0, 0}}, {{3, -1, 0}}, {{3, 1, -2}}};

	const double th = 0.7, c = cos(0.7), s = sin(0.7);
	son_point3d_t r, p;
	son_polygon3d_t poly;
	double d;

	printf("1..29\n");

	ok(fabs(x_val(&A) - 2.0) < EPS && fabs(y_val(&A) - 3.0) < EPS &&
	   fabs(z_val(&A) - 5.0) < EPS, "x_val/y_val/z_val read v[0], v[1], v[2]");

	add_2_point3d(&A, &B, &r);
	ok(p_eq(&r, 9.0, 14.0, 18.0), "add_2_point3d adds component wise");

	minus_2_point3d(&A, &B, &r);
	ok(p_eq(&r, -5.0, -8.0, -8.0), "minus_2_point3d subtracts component wise");

	/* is_same_point3d: TRUE only when all three components match */
	r = A;
	ok(is_same_point3d(&A, &r) == son_TRUE, "is_same_point3d is TRUE for equal points");
	r = A; r.v[0] = 2.5;
	ok(is_same_point3d(&A, &r) == son_FALSE, "is_same_point3d is FALSE when only x differs");
	r = A; r.v[1] = 3.5;
	ok(is_same_point3d(&A, &r) == son_FALSE, "is_same_point3d is FALSE when only y differs");
	r = A; r.v[2] = 5.5;
	ok(is_same_point3d(&A, &r) == son_FALSE, "is_same_point3d is FALSE when only z differs");

	p = A;
	point3d_offset(&p, 1.5);
	ok(p_eq(&p, 3.5, 4.5, 6.5), "point3d_offset adds the offset to every component");
	p = A;
	point3d_offset(&p, -1.5);
	ok(p_eq(&p, 0.5, 1.5, 3.5), "point3d_offset with a negative offset");

	r = B;
	duplicate_point3d(&A, &r);
	ok(p_eq(&r, 2.0, 3.0, 5.0) && p_eq(&A, 2.0, 3.0, 5.0),
	   "duplicate_point3d copies the source and leaves it alone");

	/* t = (n.p - n.q)/|n|^2 = (1 - 26)/9 = -25/9; q + t*n = (11/9, -5/9, 4/9) */
	r = B;
	ok(proj_point_2_plane(&FRONT, &N, &PL, &r) == son_TRUE &&
	   p_eq(&r, 11.0 / 9.0, -5.0 / 9.0, 4.0 / 9.0),
	   "proj_point_2_plane projects a point in front of the plane");

	/* a point already on the plane gives t = 0 and is returned unchanged */
	r = B;
	ok(proj_point_2_plane(&ON, &N, &PL, &r) == son_TRUE && p_eq(&r, 3.0, 1.0, -2.0),
	   "proj_point_2_plane keeps a point that lies on the plane");

	/* |25|/3 in front, |-4|/3 behind, 0 on the plane */
	d = -7;
	ok(dis_point_2_plane(&FRONT, &N, &PL, &d) == son_TRUE && fabs(d - 25.0 / 3.0) < EPS,
	   "dis_point_2_plane for a point in front of the plane");
	d = -7;
	ok(dis_point_2_plane(&BACK, &N, &PL, &d) == son_TRUE && fabs(d - 2.0) < EPS,
	   "dis_point_2_plane is the absolute distance for a point behind");
	d = -7;
	ok(dis_point_2_plane(&ON, &N, &PL, &d) == son_TRUE && fabs(d) < EPS,
	   "dis_point_2_plane is 0 for a point on the plane");
	d = -7;
	ok(dis_point_2_plane(&FRONT, &ZERO, &PL, &d) == son_FALSE && fabs(d - (-7.0)) < EPS,
	   "dis_point_2_plane rejects a zero normal and writes nothing");

	ok(fabs(point3d_classify(&FRONT, &N, &PL) - 25.0) < EPS,
	   "point3d_classify is positive in front of the plane");
	ok(fabs(point3d_classify(&BACK, &N, &PL) - (-6.0)) < EPS,
	   "point3d_classify is negative behind the plane");
	ok(fabs(point3d_classify(&ON, &N, &PL)) < EPS,
	   "point3d_classify is 0 on the plane");
	ok(fabs(point3d_classify(&FRONT, &ZERO, &PL) - (double)INVALID_32) < EPS,
	   "point3d_classify returns INVALID_32 for a zero normal");

	/* about x: (x, y*cos - z*sin, y*sin + z*cos) */
	p = A;
	point3d_rotate(&p, 1, 0, 0, th);
	ok(p_eq(&p, 2.0, 3 * c - 5 * s, 3 * s + 5 * c), "point3d_rotate about the x axis");

	/* about y: (x*cos + z*sin, y, -x*sin + z*cos) */
	p = A;
	point3d_rotate(&p, 0, 1, 0, th);
	ok(p_eq(&p, 2 * c + 5 * s, 3.0, -2 * s + 5 * c), "point3d_rotate about the y axis");

	/* about z: (x*cos - y*sin, x*sin + y*cos, z) */
	p = A;
	point3d_rotate(&p, 0, 0, 1, th);
	ok(p_eq(&p, 2 * c - 3 * s, 2 * s + 3 * c, 5.0), "point3d_rotate about the z axis");

	/* a half turn about z maps (2,3,5) to (-2,-3,5) */
	p = A;
	point3d_rotate(&p, 0, 0, 1, M_PI);
	ok(p_eq(&p, -2.0, -3.0, 5.0), "point3d_rotate by pi about z negates x and y");

	p = A;
	point3d_rotatev(&p, &N, 0.0);
	ok(p_eq(&p, 2.0, 3.0, 5.0), "point3d_rotatev by 0 leaves the point in place");

	/* 2*pi/3 about (1,1,1) cycles the components */
	p = A;
	point3d_rotatev(&p, &(son_vector3d_t){{1, 1, 1}}, 2.0 * M_PI / 3.0);
	ok(p_eq(&p, 5.0, 2.0, 3.0), "point3d_rotatev about (1,1,1) permutes the components");

	/* the triangle spans the same plane, but with a unit normal: 25/3 */
	polygon3d_init(&poly);
	polygon3d_add_points(&poly, TRI, 3);
	ok(fabs(point3d_classify_poly(&FRONT, &poly) - 25.0 / 3.0) < EPS,
	   "point3d_classify_poly classifies against the polygon plane");
	ok(fabs(point3d_classify_poly(&BACK, &poly) - (-2.0)) < EPS,
	   "point3d_classify_poly is negative behind the polygon plane");
	ok(fabs(point3d_classify_poly(&ON, &poly)) < EPS,
	   "point3d_classify_poly is 0 for a point on the polygon plane");
	polygon3d_clear(&poly);

	return tap_fails != 0;
}
