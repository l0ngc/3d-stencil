#include <stdlib.h>
#include <stdio.h>
#include <arm_sve.h>
#include <omp.h>
typedef float real_t;
typedef real_t ***arr_t;
#define MAX_TIME 10
#define INNER -1.0
#define OUTER 1.0
#define BLOCK_SIZE 256

void setElement(float ***array, float value, int x, int y, int z)
{
	array[x][y][z] = value;
}

void initValues(float ***array, int sx, int sy, int sz, float inner_temp, float outer_temp)
{
	for (int i = 1; i < (sx - 1); i++)
	{
		for (int j = 1; j < (sy - 1); j++)
		{
			for (int k = 1; k < (sz - 1); k++)
			{
				setElement(array, inner_temp, i, j, k);
			}
		}
	}

	for (int j = 0; j < sy; j++)
	{
		for (int k = 0; k < sz; k++)
		{
			setElement(array, outer_temp, 0, j, k);
			setElement(array, outer_temp, sx - 1, j, k);
		}
	}

	for (int i = 0; i < sx; i++)
	{
		for (int k = 0; k < sz; k++)
		{
			setElement(array, outer_temp, i, 0, k);
			setElement(array, outer_temp, i, sy - 1, k);
		}
	}

	for (int i = 0; i < sx; i++)
	{
		for (int j = 0; j < sy; j++)
		{
			setElement(array, outer_temp, i, j, 0);
			setElement(array, outer_temp, i, j, sz - 1);
		}
	}
}

void stencil_3d_7point(arr_t A, arr_t B, const int nx, const int ny, const int nz)
{
	int i, j, k;
	svfloat32_t vec_mid, vec_Result, vec_Seven;
	vec_Seven = svdup_n_f32(7.0);
	
	for (int timestep = 0; timestep < MAX_TIME; ++timestep)
	{
		#pragma omp parallel for private(vec_mid, vec_Result) shared(vec_Seven)
		for (i = 1; i < nx - 1; i++)
		{
			for (j = 1; j < ny - 1; j += 1)
			{
				for (k = 1; k < nz - 1; k += svcntw())
				{
					svbool_t pg = svwhilelt_b32(k, nz - 2);
					// load A[i-1][j][k], A[i][j-1][k], and A[i][j][k-1] vectors
					vec_mid = svld1(pg, &A[i - 1][j][k]);
					vec_Result = svld1(pg, &A[i][j - 1][k]);
					// keep adding on
					vec_Result = svadd_f32_m(pg, vec_mid, vec_Result);
					vec_mid = svld1(pg, &A[i][j][k - 1]);
					vec_Result = svadd_f32_m(pg, vec_mid, vec_Result);
					// load A[i][j][k] vector
					vec_mid = svld1(pg, &A[i][j][k]);
					vec_Result = svadd_f32_m(pg, vec_mid, vec_Result);
					// load A[i][j][k+1], A[i][j+1][k], and A[i+1][j][k] vectors
					vec_mid = svld1(pg, &A[i][j][k + 1]);
					vec_Result = svadd_f32_m(pg, vec_mid, vec_Result);
					vec_mid = svld1(pg, &A[i][j + 1][k]);
					vec_Result = svadd_f32_m(pg, vec_mid, vec_Result);
					vec_mid = svld1(pg, &A[i + 1][j][k]);
					vec_Result = svadd_f32_m(pg, vec_mid, vec_Result);
					// divide the result by 7
					vec_Result = svdiv_f32_m(pg, vec_Result, vec_Seven);
					// store the result to B[i][j][k]
					svst1(pg, &B[i][j][k], vec_Result);
				}
			}
		}
		for (i = 1; i < nx - 1; i++)
			for (j = 1; j < ny - 1; j++)
				for (k = 1; k < nz - 1; k++)
					A[i][j][k] = B[i][j][k];
	}
}


int main()
{
	int i, j, k;
	int size = 64;
	int sz = size;
	int sx = size;
	int sy = size;
	float ***A = (float ***)malloc(sx * sizeof(float **));
	for (i = 0; i < sy; i++)
	{
		A[i] = (float **)malloc(sy * sizeof(float *));
		for (j = 0; j < sz; j++)
		{
			A[i][j] = (float *)malloc(sz * sizeof(float));
		}
	}
	float ***B = (float ***)malloc(sx * sizeof(float **));
	for (i = 0; i < sy; i++)
	{
		B[i] = (float **)malloc(sy * sizeof(float *));
		for (j = 0; j < sz; j++)
		{
			B[i][j] = (float *)malloc(sz * sizeof(float));
		}
	}
	// initialize the A matrix

	initValues(A, sx, sy, sz, INNER, OUTER);

	stencil_3d_7point(A, B, sz, sy, sz);
	free(A);
	// delete[] A;
	return 0;
}
