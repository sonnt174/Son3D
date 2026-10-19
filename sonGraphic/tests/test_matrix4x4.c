#include <stdio.h>
#include <math.h>
#include "matrix4x4.h"

#define EPS 1e-9

static int tap_no;
static int tap_fails;

static void ok(int cond, const char *name)
{
	printf("%s %d - %s\n", cond ? "ok" : "not ok", ++tap_no, name);
	if (!cond)
		tap_fails++;
}

/* every element of the result must match, so a loop running short or long fails */
static int m_eq(const son_matrix4x4_t *got, const double exp[4][4])
{
	int i, j;
	for (i = 0; i < 4; ++i)
		for (j = 0; j < 4; ++j)
			if (fabs(got->m[i][j] - exp[i][j]) > EPS)
				return 0;
	return 1;
}

/* pairwise distinct entries in both operands */
static const son_matrix4x4_t A = {{{ 1,  2,  3,  4},
								   { 5,  6,  7,  8},
								   { 9, 10, 11, 12},
								   {13, 14, 15, 16}}};
static const son_matrix4x4_t B = {{{17, 19, 23, 29},
								   {31, 37, 41, 43},
								   {47, 53, 59, 61},
								   {67, 71, 73, 79}}};
/* non singular, det = 16 */
static const son_matrix4x4_t M = {{{ 2,  3,  5,  7},
								   {11, 13, 17, 19},
								   {23, 29, 31, 41},
								   {37, 43, 47, 53}}};

int main(void)
{
	son_matrix4x4_t r;
	son_vector3d_t v, mv;
	son_matrix4x4_t a_copy = A;
	double det;
	son_bool_t rc;

	printf("1..14\n");

	{
		static const double exp[4][4] = {{18, 21, 26, 33},
										 {36, 43, 48, 51},
										 {56, 63, 70, 73},
										 {80, 85, 88, 95}};
		rc = matrix4_add(&a_copy, &B, &r);
		ok(m_eq(&r, exp), "matrix4_add sums every element");
		ok(rc == son_TRUE, "matrix4_add returns son_TRUE");
	}

	{
		static const double exp[4][4] = {{-16, -17, -20, -25},
										 {-26, -31, -34, -35},
										 {-38, -43, -48, -49},
										 {-54, -57, -58, -63}};
		rc = matrix4_minus(&A, &B, &r);
		ok(m_eq(&r, exp), "matrix4_minus subtracts every element");
		ok(rc == son_TRUE, "matrix4_minus returns son_TRUE");
	}

	{
		/* row i of the result is row i of m1 against the columns of m2 */
		static const double exp[4][4] = {{ 488,  536,  574,  614},
										 {1136, 1256, 1358, 1462},
										 {1784, 1976, 2142, 2310},
										 {2432, 2696, 2926, 3158}};
		rc = matrix4_mult(&A, &B, &r);
		ok(m_eq(&r, exp), "matrix4_mult computes rows(m1) x cols(m2)");
		ok(rc == son_TRUE, "matrix4_mult returns son_TRUE");
	}

	{
		static const double exp[4][4] = {{-1.5, -3.0,  -4.5,  -6.0},
										 {-7.5, -9.0, -10.5, -12.0},
										 {-13.5, -15.0, -16.5, -18.0},
										 {-19.5, -21.0, -22.5, -24.0}};
		rc = matrix4_mult_scalar(&A, -1.5, &r);
		ok(m_eq(&r, exp), "matrix4_mult_scalar scales every element");
		ok(rc == son_TRUE, "matrix4_mult_scalar returns son_TRUE");
	}

	{
		static const double exp[4][4] = {{1, 5,  9, 13},
										 {2, 6, 10, 14},
										 {3, 7, 11, 15},
										 {4, 8, 12, 16}};
		rc = matrix4_tranpose(&B, &r);
		ok(rc == son_TRUE, "matrix4_tranpose returns son_TRUE");
		rc = matrix4_tranpose(&A, &r);
		ok(m_eq(&r, exp), "matrix4_tranpose swaps rows and columns");
	}

	{
		/* v is extended with w = 1, and only 3 components are written back */
		v.v[0] = 2; v.v[1] = 3; v.v[2] = 5;
		mv.v[0] = mv.v[1] = mv.v[2] = -999;
		rc = matrix4_mult_vector(&A, &v, &mv);
		ok(fabs(mv.v[0] - 27) <= EPS &&
		   fabs(mv.v[1] - 71) <= EPS &&
		   fabs(mv.v[2] - 115) <= EPS,
		   "matrix4_mult_vector applies rows 0..2 with implicit w = 1");
		ok(rc == son_TRUE, "matrix4_mult_vector returns son_TRUE");
	}

	{
		det = -1;
		rc = matrix4_determinant(&M, &det);
		ok(fabs(det - 16.0) <= EPS, "matrix4_determinant of non singular M is 16");
		ok(rc == son_TRUE, "matrix4_determinant returns son_TRUE");
	}

	return tap_fails == 0 ? 0 : 1;
}
