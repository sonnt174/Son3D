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

/* every element must match, so a loop running short or long fails too */
static int m_eq(const son_matrix4x4_t *got, const double exp[4][4])
{
	int i, j;
	for (i = 0; i < 4; ++i)
		for (j = 0; j < 4; ++j)
			if (fabs(got->m[i][j] - exp[i][j]) > EPS)
				return 0;
	return 1;
}

/* non singular, pairwise distinct entries, row 3 non zero, det = 16 */
static const son_matrix4x4_t P = {{{ 2,  3,  5,  7},
								   {11, 13, 17, 19},
								   {23, 29, 31, 41},
								   {37, 43, 47, 53}}};

int main(void)
{
	son_matrix4x4_t m, r;
	/* axis neither unit nor aligned with a coordinate axis */
	son_vector3d_t axis = {{1, 2, 3}};
	double cof;
	int i;

	printf("1..15\n");

	{
		static const double zero[4][4] = {{0, 0, 0, 0}, {0, 0, 0, 0},
										  {0, 0, 0, 0}, {0, 0, 0, 0}};
		son_bool_t rc;
		m = P;
		rc = matrix4_init(&m);
		ok(m_eq(&m, zero), "matrix4_init zeroes every element");
		ok(rc == son_TRUE, "matrix4_init returns son_TRUE");
	}

	{
		static const double id[4][4] = {{1, 0, 0, 0}, {0, 1, 0, 0},
										{0, 0, 1, 0}, {0, 0, 0, 1}};
		m = P;
		matrix4_identity(&m);
		ok(m_eq(&m, id), "matrix4_identity builds the identity matrix");
	}

	{
		static const double exp[4][4] = {{ 2,  3,  5,  7}, {11, 13, 17, 19},
										 {23, 29, 31, 41}, {37, 43, 47, 53}};
		matrix4_init(&m);
		matrix4_cpy(&P, &m);
		ok(m_eq(&m, exp), "matrix4_cpy copies every element of the source");
	}

	/* rotation about (1,2,3) by theta = 0.7, element by element */
	{
		static const double exp[4][4] = {
			{0.78163917390702498, -0.48292928421421222, 0.39473979817379978, 0},
			{0.55011723070435836, 0.83203013377463464, -0.071392499417875857, 0},
			{-0.29395787843858051, 0.27295633888831433, 0.91601506688731738, 0},
			{0, 0, 0, 1}};
		get_r_matrix4(&axis, 0.7, &r);
		ok(m_eq(&r, exp), "get_r_matrix4 fills the whole rotation matrix");
	}

	/* matrix4_rotate post-multiplies: m becomes P * R */
	{
		static const double exp[4][4] = {
			{1.7438406477342223, 2.895013527337051, 5.1553774325305586, 7},
			{10.752270978678066, 10.144427373815258, 18.986291424563806, 19},
			{24.818406458691975, 21.483146848075268, 35.405099948385832, 41},
			{38.759670068234044, 30.737860164134212, 54.588203201165847, 53}};
		m = P;
		matrix4_rotate(&m, &axis, 0.7);
		ok(m_eq(&m, exp), "matrix4_rotate replaces m with m * R");
	}

	{
		static const double exp[4][4] = {{   57.5,    67.5,    75.5,    86.5},
										 { -72.25,  -83.75,  -88.75, -100.25},
										 { 161.75,  190.25,  207.25,  239.75},
										 {     37,      43,      47,      53}};
		m = P;
		matrix4_translate(&m, 1.5, -2.25, 3.75);
		ok(m_eq(&m, exp), "matrix4_translate adds row 3 scaled by x,y,z");
	}

	{
		static const double exp[4][4] = {{   5,    7.5,  12.5,  7},
										 {27.5,   32.5,  42.5, 19},
										 {57.5,   72.5,  77.5, 41},
										 {92.5,  107.5, 117.5, 53}};
		m = P;
		matrix4_scale(&m, 2.5);
		ok(m_eq(&m, exp), "matrix4_scale scales the first three columns");
	}

	{
		static const struct { int i, j; double exp; } c[5] = {
			{0, 0,  720.0}, {1, 2,  146.0}, {2, 1,  168.0},
			{3, 3,   70.0}, {1, 3, -162.0}};
		static const char *names[5] = {
			"matrix4_cofactor(0,0)", "matrix4_cofactor(1,2)",
			"matrix4_cofactor(2,1)", "matrix4_cofactor(3,3)",
			"matrix4_cofactor(1,3) is negated for odd i+j"};
		for (i = 0; i < 5; ++i) {
			cof = 0.0;
			matrix4_cofactor(&P, c[i].i, c[i].j, &cof);
			ok(fabs(cof - c[i].exp) < EPS, names[i]);
		}
	}

	{
		static const double exp[4][4] = {{ 45, -30.5,  -11,  13.5},
										 {-43, 28.75, 10.5, -12.75},
										 {-13, 9.125,    3, -3.875},
										 { 15, -10.125, -3.5, 4.375}};
		son_bool_t rc;
		matrix4_init(&m);
		rc = matrix4_inverse(&P, &m);
		ok(m_eq(&m, exp), "matrix4_inverse computes every element");
		ok(rc == son_TRUE, "matrix4_inverse returns son_TRUE");
	}

	return tap_fails != 0;
}
