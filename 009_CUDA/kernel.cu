
#include "cuda_runtime.h"
#include "device_launch_parameters.h"
#include "device_functions.h"
#include "header.h"
#include <stdio.h>
#include <ctime>
__global__ void mullVecsK(int *c, const int *a, const int *b)
{
	int i = threadIdx.x;
	c[i] = a[i] * b[i];
}

__global__ void addVecK(int *a, unsigned int _s)
{
	int i = threadIdx.x;
	a[i] = a[i] + a[i + _s];
	__syncthreads();
}

void mullWithCuda(int *c, const int *a, const int *b, unsigned int size);
int dotProductWithCuda(const int *a, const int *b, size_t _s);
int addVecWithCuda(int * v, size_t _s);
void __addVecCuda(int * v, size_t _s);

int main()
{
	size_t arraySize = 0;
	int *a,*b,*c;		
	clock_t start, end;
	for (int i = 0; i < 3; i++) {
		if (i == 0)
			arraySize = 512;
		if (i == 1)
			arraySize = 1024;
		if (i == 2)
			arraySize = 131072;
		
		a = new int[arraySize];
		b = new int[arraySize];
		c = new int[arraySize];
		randArr(a, arraySize);
		randArr(b, arraySize);
		
		start = clock();
		int dotProdWC = dotProductWithCuda(a, b, arraySize);
		end = clock();
		auto dur = end - start;
		printf("dot product with CUDA for %d elem = %d time: %d\n", arraySize, dotProdWC, dur);

		start =  clock();
		int dotProd = dotProduct(a, b, arraySize);
		end = clock();
		dur = end - start;
		printf("dot product without CUDA for %d elem= %d time: %d\n", arraySize, dotProd, dur);
		
		delete[] a;
		delete[] b;
		delete[] c;
	}
	return 0;
}

void mullWithCuda(int *c, const int *a, const int *b, unsigned int size)
{
	int *dev_a = 0;
	int *dev_b = 0;
	int *dev_c = 0;

	cudaSetDevice(0);

	cudaMalloc((void**)&dev_c, size * sizeof(int));
	cudaMalloc((void**)&dev_a, size * sizeof(int));
	cudaMalloc((void**)&dev_b, size * sizeof(int));


	cudaMemcpy(dev_a, a, size * sizeof(int), cudaMemcpyHostToDevice);
	cudaMemcpy(dev_b, b, size * sizeof(int), cudaMemcpyHostToDevice);

	mullVecsK << <1, size >> > (dev_c, dev_a, dev_b);

	cudaDeviceSynchronize();
	cudaMemcpy(c, dev_c, size * sizeof(int), cudaMemcpyDeviceToHost);

	cudaFree(dev_c);
	cudaFree(dev_a);
	cudaFree(dev_b);
}

void __addVecCuda(int * v, size_t _s)
{
	while (_s != 1) {
		_s /= 2;
		addVecK << <1, _s >> > (v, _s);
	}
}

int addVecWithCuda(int * v, size_t _s)
{
	int * dev_v = 0;
	cudaMalloc((void **)&dev_v, _s * sizeof(int));
	cudaMemcpy(dev_v, v, _s * sizeof(int), cudaMemcpyHostToDevice);
	int * res = new int[_s] {0};
	auto sizes = getSizesOfKernels(_s);
	size_t off = 0;
	for (size_t i = 0; i < sizes.size(); i++)
	{
		if (sizes[i] == 1) {
			off += sizes[i];
			continue;
		}
		__addVecCuda(dev_v + off, sizes[i]);
		cudaMemcpy(dev_v + i, dev_v + off, sizeof(int), cudaMemcpyDeviceToDevice);
		off += sizes[i];
	}
	if (sizes.size() == 2)
		__addVecCuda(dev_v, 2);
	cudaMemcpy(res, dev_v, _s * sizeof(int), cudaMemcpyDeviceToHost);
	cudaFree(dev_v);
	int result = res[0];
	delete[] res;
	return result;
}

int dotProductWithCuda(const int *a, const int *b, size_t _s)
{
	int * c = new int[_s] {0};
	mullWithCuda(c, a, b, _s);
	int res = addVecWithCuda(c, _s);
	delete[] c;
	return res;
}
