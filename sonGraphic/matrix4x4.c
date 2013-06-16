#include "matrix4x4.h"

/******************************************************************************
** Name: matrix4_init
** Params:
** Return:
** Descriptions:
**			Initialize a matrix to zero
*******************************************************************************/
son_bool_t matrix4_init(son_matrix4x4_t *m)
{
	memset(m, 0, sizeof(son_matrix4x4_t));

	return son_TRUE;
}

/******************************************************************************
** Name: matrix4_plus
** Params:
** Return:
** Descriptions:
**			add two matrix-es and output is sum of them
*******************************************************************************/
son_bool_t matrix4_add(son_matrix4x4_t * const m1,
					   const son_matrix4x4_t * const m2,
					   son_matrix4x4_t *m)
{
	int i,j;

	for (i = 0; i < MATRIX_4X4_SIZE; ++i)
		for (j = 0; j < MATRIX_4X4_SIZE; ++j) {
			m->m[i][j] = m1->m[i][j] + m2->m[i][j];
	}

	return son_TRUE;
}

/******************************************************************************
** Name: matrix4_minus
** Params:
** Return:
** Descriptions:
**			minus two matrix-es and output is the result
*******************************************************************************/
son_bool_t matrix4_minus(const son_matrix4x4_t * const m1,
						 const son_matrix4x4_t * const m2,
						 son_matrix4x4_t *m)
{
	int i,j;

	for (i = 0; i < MATRIX_4X4_SIZE; ++i)
		for (j = 0; j < MATRIX_4X4_SIZE; ++j) {
			m->m[i][j] = m1->m[i][j] - m2->m[i][j];
	}

	return son_TRUE;
}

/******************************************************************************
** Name: matrix4_mult_matrix
** Params:
** Return:
** Descriptions:
**			multiply two matrix-es and output is the result
*******************************************************************************/
son_bool_t matrix4_mult(const son_matrix4x4_t * const m1,
						const son_matrix4x4_t * const m2,
						son_matrix4x4_t *m)
{
	int i,j,k;

	for (i = 0; i < MATRIX_4X4_SIZE; ++i)
		for (j = 0; j < MATRIX_4X4_SIZE; ++j) {
			m->m[i][j] = 0.0;
			for (k = 0; k < MATRIX_4X4_SIZE; ++k) {
				m->m[i][j] += m1->m[i][k] * m2->m[k][j];
			}
	}

	return son_TRUE;
}

/******************************************************************************
** Name: matrix4_mult_matrix
** Params:
** Return:
** Descriptions:
**			multiply a matrix with a number and output is the result
*******************************************************************************/
son_bool_t matrix4_mult_scalar(const son_matrix4x4_t * const m,
							   double n,
							   son_matrix4x4_t *mn)
{
	int i,j;

	for (i = 0; i < MATRIX_4X4_SIZE; ++i)
		for (j = 0; j < MATRIX_4X4_SIZE; ++j) {
			mn->m[i][j] = n * m->m[i][j];
	}

	return son_TRUE;
}

/******************************************************************************
** Name: matrix4_tranpose
** Params:
** Return:
** Descriptions:
**			Transpose a matrix and output is the result
*******************************************************************************/
son_bool_t matrix4_tranpose(const son_matrix4x4_t * const m,
							son_matrix4x4_t *mT)
{
	int i,j;

	for (i = 0; i < MATRIX_4X4_SIZE; ++i)
		for (j = 0; j < MATRIX_4X4_SIZE; ++j) {
			mT->m[i][j] = m->m[j][i];
	}
	return son_TRUE;
}

/******************************************************************************
** Name: matrix4_mult_vector
** Params:
** Return:
** Descriptions:
**			multiplication of a matrix and a vector then output is the result
*******************************************************************************/
son_bool_t matrix4_mult_vector(const son_matrix4x4_t * const m,
							   const son_vector3d_t * const v,
							   son_vector3d_t *mv)
{
	int i,j;
	double tv[4];
	double dv[4];

	memcpy (&tv[0], &v->v[0], sizeof(v->v));
	tv[3] = 1;
	i = 0; j = 0;
	for (i = 0; i < MATRIX_4X4_SIZE; ++i) {
		dv[i] = 0;
		for (j = 0; j < MATRIX_4X4_SIZE; ++j)
			dv[i] += m->m[i][j] * tv[j];
	}

	for (i = 0; i < 3; ++i)
		mv->v[i] = dv[i];

	return son_TRUE;
}

/******************************************************************************
** Name: matrix4_identity
** Params:
** Return:
** Descriptions:
**			
*******************************************************************************/
son_bool_t matrix4_identity(son_matrix4x4_t *iM)
{
	int i = 0;
	int j = 0;
	memset (iM, 0, sizeof (son_matrix4x4_t));
	iM->m[0][0] = iM->m[1][1] = iM->m[2][2] = iM->m[3][3] = 1.0f;

	return son_TRUE;
}

/******************************************************************************
** Name: get_r_matrix4
** Params:
** Return:
** Descriptions:
**			This function get the rotate matrix in 3d about a vector U(x, y, z)
*******************************************************************************/
son_bool_t get_r_matrix4(const son_vector3d_t *v,
						 double theta,
						 son_matrix4x4_t *r)
{ 
	double xx, yy, zz;
	double xyz2, sqr_xyz2;
	double cos_th, sin_th;
	double x,y,z;

	x = v->v[0]; y = v->v[1]; z = v->v[2];

	xx = v->v[0] * v->v[0];
	yy = v->v[1] * v->v[1];
	zz = v->v[2] * v->v[2];

	xyz2 = xx + yy + zz;
	sqr_xyz2 = sqrt(xyz2);

	cos_th = cos(theta);
	sin_th = sin(theta);

	r->m[0][0] = (xx + (yy + zz) * cos_th) / xyz2;
	r->m[0][1] = (x * y * (1 - cos_th) - z * sqr_xyz2 * sin_th) / xyz2;
	r->m[0][2] = (x * z * (1 - cos_th) + y * sqr_xyz2 * sin_th) / xyz2;
	r->m[0][3] = 0;

	r->m[1][0] = (x * y * (1 - cos_th) + z * sqr_xyz2 * sin_th ) / xyz2;
	r->m[1][1] = (yy + (xx + zz) * cos_th) / xyz2;
	r->m[1][2] = (y * z * (1 - cos_th) - x * sqr_xyz2 * sin_th) / xyz2;
	r->m[1][3] = 0;

	r->m[2][0] = (x * z * (1 - cos_th) - y * sqr_xyz2 * sin_th) / xyz2;
	r->m[2][1] = (y * z * (1 - cos_th) + x * sqr_xyz2 * sin_th) / xyz2;
	r->m[2][2] = (zz + (xx + yy) * cos_th) / xyz2;
	r->m[2][3] = 0;

	r->m[3][0] = r->m[3][1] = r->m[3][2] = 0;
	r->m[3][3] = 1;

	return son_TRUE;
}

/******************************************************************************
** Name: get_r_matrix4
** Params:
** Return:
** Descriptions:
**			This function rotate a matrix m in 3d 
**				about a arbitrary vector U(x, y, z)
*******************************************************************************/
son_bool_t matrix4_rotate(son_matrix4x4_t *m,
						  const son_vector3d_t *v,
						  double theta)
{
	son_matrix4x4_t r, rm;
	get_r_matrix4(v, theta, &r);

	matrix4_mult(m, &r, &rm);
	matrix4_cpy(&rm, m);

	return son_TRUE;
}

/******************************************************************************
** Name: matrix4_translate
** Params:
** Return:
** Descriptions:
**			This function get the translate matrix by x,y,z 
*******************************************************************************/
son_bool_t  matrix4_translate(son_matrix4x4_t *m,
							  double x, double y, double z)
{
	m->m[0][0] = m->m[0][0] + m->m[3][0] * x;
	m->m[0][1] = m->m[0][1] + m->m[3][1] * x;
	m->m[0][2] = m->m[0][2] + m->m[3][2] * x;
	m->m[0][3] = m->m[0][3] + m->m[3][3] * x;
	
	m->m[1][0] = m->m[1][0] + m->m[3][0] * y;
	m->m[1][1] = m->m[1][1] + m->m[3][1] * y;
	m->m[1][2] = m->m[1][2] + m->m[3][2] * y;
	m->m[1][3] = m->m[1][3] + m->m[3][3] * y;
	
	m->m[2][0] = m->m[2][0] + m->m[3][0] * z;
	m->m[2][1] = m->m[2][1] + m->m[3][1] * z;
	m->m[2][2] = m->m[2][2] + m->m[3][2] * z;
	m->m[2][3] = m->m[2][3] + m->m[3][3] * z;

	m->m[3][0] = m->m[3][0]; 
	m->m[3][1] = m->m[3][1]; 
	m->m[3][2] = m->m[3][2]; 
	m->m[3][3] = m->m[3][3]; 
	
	return son_TRUE;
}

/******************************************************************************
** Name: matrix4_scale
** Params:
** Return:
** Descriptions:
**			This function scale the matrix m4x4 with s
*******************************************************************************/
son_bool_t matrix4_scale(son_matrix4x4_t *m,
						 double s)
{
	son_matrix4x4_t sm;
	son_matrix4x4_t s_matrix =  {s, 0.0, 0.0, 0.0,
								 0.0, s, 0.0, 0.0,
								 0.0, 0.0, s, 0.0,
								 0.0, 0.0, 0.0, 1.0};

	matrix4_mult(m, &s_matrix, &sm);
	matrix4_cpy(&sm, m);
	return son_TRUE;
}

/******************************************************************************
** Name: matrix4_cpy
** Params:
** Return:
** Descriptions:
**			This function copy from mSrc to mDst
*******************************************************************************/
son_bool_t matrix4_cpy(const son_matrix4x4_t *mSrc,
					   son_matrix4x4_t *mDst)
{
	memcpy(mDst, mSrc, sizeof(son_matrix4x4_t));
	/*int i, j;
	for (i = 0; i < 4; i++)
		for (j = 0; j < 4; j++) {
		mDst->m[i][j] = mSrc->m[i][j];
	}*/

	return son_TRUE;
}

/******************************************************************************
** Name: matrix4_cofactor
** Params:
** Return:
** Descriptions:
**
*******************************************************************************/
son_bool_t matrix4_cofactor(const son_matrix4x4_t *mat,
							int i, int j,
							double *cof)
{
	int		i0, i1, i2, j0, j1, j2;							//	indexing values
	double	det;																//	determinant of the sub-matrix_3d (the cofactor)
	switch (i)																//	switch on the desired row
	{																					//	begin
	case 0:	i0 = 1; i1 = 2; i2 = 3; break;	//	set appropriate indexing values
	case 1:	i0 = 0; i1 = 2; i2 = 3; break;	//	set appropriate indexing values
	case 2:	i0 = 0; i1 = 1; i2 = 3; break;	//	set appropriate indexing values
	case 3:	i0 = 0; i1 = 1; i2 = 2; break;	//	set appropriate indexing values
	}																					//	end
	switch (j)																//	switch on the desired column
	{																					//	begin
	case 0:	j0 = 1; j1 = 2; j2 = 3; break;	//	set appropriate indexing values
	case 1:	j0 = 0; j1 = 2; j2 = 3; break;	//	set appropriate indexing values
	case 2:	j0 = 0; j1 = 1; j2 = 3; break;	//	set appropriate indexing values
	case 3:	j0 = 0; j1 = 1; j2 = 2; break;	//	set appropriate indexing values
	}																					//	end
	//	compute the determinent of the resulting 3x3 matrix_3d
	det =	mat->m[i0][j0] * (mat->m[i1][j1] * mat->m[i2][j2] - mat->m[i1][j2] * mat->m[i2][j1]) -
			mat->m[i0][j1] * (mat->m[i1][j0] * mat->m[i2][j2] - mat->m[i1][j2] * mat->m[i2][j0]) +
			mat->m[i0][j2] * (mat->m[i1][j0] * mat->m[i2][j1] - mat->m[i1][j1] * mat->m[i2][j0]);
	*cof = odd (i + j) ? -det : det;
	return son_TRUE;
}

/******************************************************************************
** Name: matrix4_determinant
** Params:
** Return:
** Descriptions:
*******************************************************************************/
son_bool_t matrix4_determinant(const son_matrix4x4_t *mat,
							   double *det)
{
	double cof[4];
	int i;
	for (i = 0; i < 4; ++i) {
		matrix4_cofactor(mat, 0, i, &cof[i]);
	}
	
	*det = 	mat->m[0][0] * cof[0] + //	multiply the value by its cofactor
			mat->m[0][1] * cof[1] + //	multiply the value by its cofactor and add the result to the current sum
			mat->m[0][2] * cof[2] + //	multiply the value by its cofactor and add the result to the current sum
			mat->m[0][3] * cof[3];
	return son_TRUE;
}

/******************************************************************************
** Name: matrix4_inverse
** Params:
** Return:
** Descriptions:
*******************************************************************************/
son_bool_t matrix4_inverse(const son_matrix4x4_t *mat,
						   son_matrix4x4_t *inv)
{
	double	det;
	double cof;
	short i, j;
	matrix4_determinant(mat, &det);	//	compute the determinant
	for ( i = 0; i < 4; i++)	//	loop on the rows
		for ( j = 0; j < 4; j++) {
			matrix4_cofactor(mat, i, j, &cof); // loop on the columns
			inv->m[j][i] = cof / det;
		} //	compute the inverse value for the current entry

	return son_TRUE;
}