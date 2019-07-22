#pragma once
#include <memory>
#include <vector>
std::vector<size_t> getSizesOfKernels(size_t n)
{
	size_t p = n;
	size_t buf = 1;
	std::vector<size_t> rs;
	while (p > 0)
	{
		if (p % 2)
			rs.push_back(buf);
		p >>= 1;
		buf *= 2;
	}
	return rs;
}
void randArr(int * a, size_t _s)
{
	for (size_t i = 0; i < _s; i++)
	{
		a[i] = rand() % 20;
	}
}
int dotProduct(const int *a, const int *b, size_t _s);
int addVec(int * v, size_t _s);
void __addVec(int * v, size_t _s);
void _mullVecs(int *c, const int *a, const int *b, size_t s)
{
	for (size_t i = 0; i < s; i++)
	{
		c[i] = a[i] * b[i];
	}
}
void _addVec(int *a, unsigned int _s)
{
	
	for (size_t i = 0; i < _s; i++)
	{
		a[i] = a[i] + a[i + _s];
	}
}


int dotProduct(const int *a, const int *b, size_t _s)
{
	int * c = new int[_s] {0};
	_mullVecs(c, a, b, _s);
	int res = addVec(c, _s);
	delete[] c;
	return res;
}

int addVec(int * v, size_t _s)
{
	int * h_v = new int [_s] {0};
	memcpy(h_v, v, _s * sizeof(int));
	int * res = new int[_s] {0};
	auto sizes = getSizesOfKernels(_s);
	size_t off = 0;
	for (size_t i = 0; i < sizes.size(); i++)
	{
		if (sizes[i] == 1) {
			off += sizes[i];
			continue;
		}
		__addVec(h_v + off, sizes[i]);
		memcpy(h_v + i, h_v + off, sizeof(int));
		off += sizes[i];
	}
	if (sizes.size() == 2)
		__addVec(h_v, 2);
	memcpy(res, h_v, _s * sizeof(int));

	int result = res[0];
	delete[] res;
	delete[] h_v;
	return result;
}

void __addVec(int * v, size_t _s)
{
	size_t s = _s;
	while (_s != 1) {
		_s /= 2;
		_addVec(v, _s);
	}
}