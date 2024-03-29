#include "stdafx.h"
#include <iostream>
#include <omp.h>


void parallel_mat_mult(int** matr1, int** matr2, int**matr3, size_t dim) {
	#pragma omp parallel for
	for (size_t i = 0; i < dim; ++i) {
		for (size_t j = 0; j < dim; ++j) {
			int temp = 0;
			for (size_t k = 0; k < dim; ++k) {
				temp += matr1[i][k] * matr2[k][j];
			}
			matr3[i][j] = temp;
		}
	}
}

void non_parallel_mat_mult(int** matr1, int** matr2, int**matr3, size_t dim) {
	for (size_t i = 0; i < dim; ++i) {
		for (size_t j = 0; j < dim; ++j) {
			int temp = 0;
			for (size_t k = 0; k < dim; ++k) {
				temp += matr1[i][k] * matr2[k][j];
			}
			matr3[i][j] = temp;
		}
	}
}

int** empty_matr(unsigned int dim)
{
	int** matr = 0;
	matr = new int*[dim];

	for (int i = 0; i < dim; ++i)
	{
		matr[i] = new int[dim];

		for (int j = 0; j < dim; ++j)
		{
			matr[i][j] = 0;
		}
	}

	return matr;
}

int** rand_matr(unsigned int dim) {
	int** matr = 0;
	matr = new int*[dim];

	for (int i = 0; i < dim; ++i)
	{
		matr[i] = new int[dim];

		for (int j = 0; j < dim; ++j)
		{
			matr[i][j] = -100 + (std::rand() % (100 + 100 + 1));
		}
	}

	return matr;
}

int main()
{
	int THREADS = 4;
	omp_set_num_threads(THREADS);

	int dims[4] = { 100, 200, 500, 750 };
	int iters[4] = { 1000, 1000, 75, 50 };

	for (size_t i = 0; i < 4; ++i) {
		int **matr1 = rand_matr(dims[i]);
		int **matr2 = rand_matr(dims[i]);

		double time_sum1 = 0;
		double time_sum2 = 0;

		int j = 0;

		while (j < iters[i]) {
			int **matr3 = empty_matr(dims[i]);
			int **matr4 = empty_matr(dims[i]);
			
			double timein1 = omp_get_wtime();
			parallel_mat_mult(matr1, matr2, matr3, dims[i]);
			double timeout1 = omp_get_wtime();

			double timein2 = omp_get_wtime();
			non_parallel_mat_mult(matr1, matr2, matr4, dims[i]);
			double timeout2 = omp_get_wtime();

			time_sum1 += timeout1 - timein1;
			time_sum2 += timeout2 - timein2;

			for (int k = 0; k < dims[i]; ++k) {
				delete[] matr3[k];
			}

			delete[] matr3;

			for (int k = 0; k < dims[i]; ++k) {
				delete[] matr4[k];
			}

			delete[] matr4;

			++j;
		}

		for (int k = 0; k < dims[i]; ++k) {
			delete[] matr1[k];
			delete[] matr2[k];
		}

		delete[] matr1;
		delete[] matr2;

		printf("Parallel result for %d: %.5f s\n", dims[i], time_sum1 / iters[i]);
		printf("Nonparallel result for %d: %.5f s\n", dims[i], time_sum2 / iters[i]);
	}

	system("pause");
	return 0;
}
