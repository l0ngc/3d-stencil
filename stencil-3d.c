#include <stdlib.h>
#include <stdio.h>
typedef float real_t;
typedef real_t ***arr_t;
#define MAX_TIME 1000
#define INNER -1.0
#define OUTER 1.0

// after we implemented vector, we could use vectorization to optimize this code.
// two ways: using SVE to generate SVE code autotimatically,
// hands on the code and apply exact vector instructions
// 
// 
void setElement(float ***array, float value, int x, int y, int z)
{
	// 在一个三维数组里面，指定位置里面设置位置
	array[x][y][z] = value;
}

void initValues(float ***array, int sx, int sy, int sz, float inner_temp, float outer_temp)
{
	// 初始化数值，对一个3维空间表层赋值为outer_temp，对内层设置为inner_temp
	// arg explains
	// array: 提前分配好的三维空间
	// sx: 一维空间的大小
	// sy: 二维空间大小
	// sz: 三维空间大小
	// inner temp: 最外圈以内的矩阵，初始化进去数值,如果放在二维，如图

	// ######
	// #****#
	// #****#
	// #****#
	// #****#
	// ######

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
	// 接下来三个循环，是分别对每一个维度的第一片和最后一片进行赋值
	
	// @@@@@@
	// #****#
	// #****#
	// #****#
	// #****#
	// @@@@@@

	for (int j = 0; j < sy; j++)
	{
		for (int k = 0; k < sz; k++)
		{
			setElement(array, outer_temp, 0, j, k);
			setElement(array, outer_temp, sx - 1, j, k);
		}
	}
	//
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
	// 这个stencil其实很简单
	__m128 a, b, c, d, e, f, g;

	for (int timestep = 0; timestep < MAX_TIME; ++timestep)
	{
        for (int k = 1; k < nz - 1; k++)
            for (int j = 1; j < ny - 1; j++)
                for (int i = 1; i < nx - 1; i += 4)
                {
                    a = _mm_load_ps(&A[i-1][j][k]);
                    b = _mm_load_ps(&A[i][j-1][k]);
                    c = _mm_load_ps(&A[i][j][k-1]);
                    d = _mm_load_ps(&A[i][j][k]);
                    e = _mm_load_ps(&A[i][j][k+1]);
                    f = _mm_load_ps(&A[i][j+1][k]);
                    g = _mm_load_ps(&A[i+1][j][k]);

                    d = _mm_add_ps(a, _mm_add_ps(b, _mm_add_ps(c, _mm_add_ps(d, _mm_add_ps(e, _mm_add_ps(f, g))))));
                    d = _mm_div_ps(d, _mm_set1_ps(7.0));

                    _mm_store_ps(&B[i][j][k], d);
                }
		for (int i = 1; i < nx - 1; i++)
			for (int j = 1; j < ny - 1; j++)
				for (int k = 1; k < nz - 1; k++)
					A[i][j][k] = B[i][j][k];
	}
}




int main()
{
	int i, j, k;
	int sz = 100;
	int sx = 100;
	int sy = 100;
	// 为三维数组A分配内存空间
	float ***A = (float ***)malloc(sx * sizeof(float **));
	for (i = 0; i < sy; i++)
	{
		A[i] = (float **)malloc(sy * sizeof(float *));
		for (j = 0; j < sz; j++)
		{
			A[i][j] = (float *)malloc(sz * sizeof(float));
		}
	}
	// 为三维数组A分配内存空间
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
	// please
	// doodle jump
	// delete[] A;
	return 0;
}
