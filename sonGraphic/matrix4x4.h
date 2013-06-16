#ifndef __MATRIX4X4_H__
#define __MATRIX4X4_H__

#include "stdlib.h"
#include "math.h"
#ifndef __SON_COMMON_TYPE_H__
#include "son_common_type.h"
#endif

#ifndef __VECTOR3D_H__
#include "vector3d.h"
#endif

#ifdef __cplusplus
extern "C" {
#endif

#define MATRIX_4X4_SIZE 4

typedef struct{
	double m[4][4];
}son_matrix4x4_t;

/******************************************************************************
** Name: matrix4_init
** Params:
** Return:
** Descriptions:
**			Initialize a matrix to zero
*******************************************************************************/
son_bool_t matrix4_init(son_matrix4x4_t *m);

/******************************************************************************
** Name: matrix4_plus
** Params:
** Return:
** Descriptions:
**			add two matrix-es and output is sum of them
*******************************************************************************/
son_bool_t matrix4_add(son_matrix4x4_t * const m1,
						const son_matrix4x4_t * const m2,
						son_matrix4x4_t *m);

/******************************************************************************
** Name: matrix4_minus
** Params:
** Return:
** Descriptions:
**			minus two matrix-es and output is the result
*******************************************************************************/
son_bool_t matrix4_minus(const son_matrix4x4_t * const m1,
						 const son_matrix4x4_t * const m2,
						 son_matrix4x4_t *m);

/******************************************************************************
** Name: matrix4_mult_matrix
** Params:
** Return:
** Descriptions:
**			multiply two matrix-es and output is the result
*******************************************************************************/
son_bool_t matrix4_mult(const son_matrix4x4_t * const m1,
						const son_matrix4x4_t * const m2,
						son_matrix4x4_t *m);

/******************************************************************************
** Name: matrix4_mult_matrix
** Params:
** Return:
** Descriptions:
**			multiply a matrix with a number and output is the result
*******************************************************************************/
son_bool_t matrix4_mult_scalar(const son_matrix4x4_t * const m,
							   double n,
							   son_matrix4x4_t *mn);

/******************************************************************************
** Name: matrix4_tranpose
** Params:
** Return:
** Descriptions:
**			Transpose a matrix and output is the result
*******************************************************************************/
son_bool_t matrix4_tranpose(const son_matrix4x4_t * const m,
							son_matrix4x4_t *mT);

/******************************************************************************
** Name: matrix4_mult_vector
** Params:
** Return:
** Descriptions:
**			multiplication of a matrix and a vector and output is the result
*******************************************************************************/
son_bool_t matrix4_mult_vector(const son_matrix4x4_t * const m,
							   const son_vector3d_t * const v,
							   son_vector3d_t *mv);

/******************************************************************************
** Name: matrix4_identity
** Params:
** Return:
** Descriptions:
**			
*******************************************************************************/
son_bool_t matrix4_identity(son_matrix4x4_t *iM);

/******************************************************************************
** Name: get_r_matrix4
** Params:
** Return:
** Descriptions:
**			This function get the rotate matrix in 3d about a vector U(x, y, z)
*******************************************************************************/
son_bool_t get_r_matrix4(const son_vector3d_t *v,
						double theta,
						son_matrix4x4_t *r);

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
						  double theta);

/******************************************************************************
** Name: matrix4_translate
** Params:
** Return:
** Descriptions:
**			This function get the translate matrix by x,y,z 
*******************************************************************************/
son_bool_t  matrix4_translate(son_matrix4x4_t *m,
								double x, double y, double z);

/******************************************************************************
** Name: matrix4_scale
** Params:
** Return:
** Descriptions:
**			This function scale the matrix m4x4 with s
*******************************************************************************/
son_bool_t matrix4_scale(son_matrix4x4_t *m,
						 double s);

/******************************************************************************
** Name: matrix4_cpy
** Params:
** Return:
** Descriptions:
**			This function get the rotate matrix in 3d about a vector U(x, y, z)
*******************************************************************************/
son_bool_t matrix4_cpy(const son_matrix4x4_t *mSrc,
					   son_matrix4x4_t *mDst);

/******************************************************************************
** Name: matrix4_cofactor
** Params:
** Return:
** Descriptions:
**
*******************************************************************************/
son_bool_t matrix4_cofactor(const son_matrix4x4_t *mat,
							int i, int j,
							double *cof);

/******************************************************************************
** Name: matrix4_determinant
** Params:
** Return:
** Descriptions:
*******************************************************************************/
son_bool_t matrix4_determinant(const son_matrix4x4_t *mat,
								double *det);

/******************************************************************************
** Name: matrix4_inverse
** Params:
** Return:
** Descriptions:
*******************************************************************************/
son_bool_t matrix4_inverse(const son_matrix4x4_t *mat,
						   son_matrix4x4_t *inv);
#ifdef __cplusplus
}
#endif

#endif