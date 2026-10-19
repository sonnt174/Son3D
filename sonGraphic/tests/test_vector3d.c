#include <stdio.h>
#include <math.h>
#include "vector3d.h"

#define EPS 1e-9

static int tap_no;
static int tap_fails;

static void ok(int cond, const char *name)
{
	printf("%s %d - %s\n", cond ? "ok" : "not ok", ++tap_no, name);
	if (!cond)
		tap_fails++;
}

/* every component must match; a wrong sign or factor in any of them fails */
static int v_eq(const son_vector3d_t *got, double x, double y, double z)
{
	return fabs(got->v[0] - x) < EPS &&
		   fabs(got->v[1] - y) < EPS &&
		   fabs(got->v[2] - z) < EPS;
}

int main(void)
{
	/* pairwise distinct, non trivial components: +/- and * / give distinct results */
	static const son_vector3d_t A = {{2, 3, 5}};
	static const son_vector3d_t B = {{7, 11, 13}};
	static const son_vector3d_t C = {{2, 3, 6}};	/* magnitude 7 exactly */
	static const son_point3d_t P1 = {{1, 2, 3}};
	static const son_point3d_t P2 = {{9, 5, 11}};

	son_vector3d_t v, r;
	double d;

	printf("1..14\n");

	/* mult_vector3d: scales in place by k */
	v = (son_vector3d_t){{3, -5, 7}};
	mult_vector3d(&v, 2.5);
	ok(v_eq(&v, 7.5, -12.5, 17.5), "mult_vector3d scales each component by k");

	/* k < 1: result shrinks, so k/x would be clearly different */
	v = (son_vector3d_t){{3, -5, 7}};
	mult_vector3d(&v, 0.25);
	ok(v_eq(&v, 0.75, -1.25, 1.75), "mult_vector3d with fractional k");

	/* div_vector3d: divides in place by k */
	v = (son_vector3d_t){{3, -5, 7}};
	div_vector3d(&v, 2.0);
	ok(v_eq(&v, 1.5, -2.5, 3.5), "div_vector3d divides each component by k");

	v = (son_vector3d_t){{3, -5, 7}};
	div_vector3d(&v, 0.5);
	ok(v_eq(&v, 6.0, -10.0, 14.0), "div_vector3d with k < 1 grows the vector");

	/* dot_product_3d: 2*7 + 3*11 + 5*13 = 14 + 33 + 65 = 112 */
	d = -1;
	dot_product_3d(&A, &B, &d);
	ok(fabs(d - 112.0) < EPS, "dot_product_3d sums the three products");

	/* sign sensitive: 2*7 + (-3)*11 + 5*(-13) = 14 - 33 - 65 = -84 */
	v = (son_vector3d_t){{7, -11, -13}};
	d = -1;
	dot_product_3d(&A, &v, &d);
	ok(fabs(d - (-84.0)) < EPS, "dot_product_3d with mixed signs");

	/* cross_product_3d: (3*13-5*11, 5*7-2*13, 2*11-3*7) = (-16, 9, 1) */
	r = (son_vector3d_t){{-1, -1, -1}};
	cross_product_3d(&A, &B, &r);
	ok(v_eq(&r, -16.0, 9.0, 1.0), "cross_product_3d of A x B");

	/* opposite order negates every component */
	r = (son_vector3d_t){{-1, -1, -1}};
	cross_product_3d(&B, &A, &r);
	ok(v_eq(&r, 16.0, -9.0, -1.0), "cross_product_3d of B x A is the negation");

	/* vector3d_magnitude2: 4 + 9 + 36 = 49 */
	d = -1;
	vector3d_magnitude2(&C, &d);
	ok(fabs(d - 49.0) < EPS, "vector3d_magnitude2 is the sum of squares");

	/* vector3d_magnitude: sqrt(49) = 7 */
	d = -1;
	vector3d_magnitude(&C, &d);
	ok(fabs(d - 7.0) < EPS, "vector3d_magnitude is sqrt of the sum of squares");

	/* unit_vector3d: C / 7, source untouched */
	r = (son_vector3d_t){{-1, -1, -1}};
	unit_vector3d(&C, &r);
	ok(v_eq(&r, 2.0 / 7.0, 3.0 / 7.0, 6.0 / 7.0) &&
	   v_eq(&C, 2.0, 3.0, 6.0),
	   "unit_vector3d divides a copy by the magnitude");

	/* vector3d_inverse: negates into the destination */
	v = (son_vector3d_t){{3, -5, 7}};
	r = (son_vector3d_t){{-1, -1, -1}};
	vector3d_inverse(&v, &r);
	ok(v_eq(&r, -3.0, 5.0, -7.0), "vector3d_inverse negates each component");

	/* vector3d_make: p2 - p1 = (8, 3, 8) */
	r = (son_vector3d_t){{-1, -1, -1}};
	vector3d_make(&P1, &P2, &r);
	ok(v_eq(&r, 8.0, 3.0, 8.0), "vector3d_make is p2 minus p1");

	/* reversed arguments give the opposite vector */
	r = (son_vector3d_t){{-1, -1, -1}};
	vector3d_make(&P2, &P1, &r);
	ok(v_eq(&r, -8.0, -3.0, -8.0), "vector3d_make reversed is the negation");

	return tap_fails != 0;
}
