#include "cache.h"
#include <ctime>
#include <iostream>

int impTestFunc(int i)
{
	return -i;
}

int TestFunc(int i)
{
	static Cache<int , int> cache(&impTestFunc);
	return cache.find(std::move(i));
}

int main()
{
	srand(time(NULL));
		
	for (int i{ 0 }; i < 100; i++)
	{
		int r = rand() % 20;
		std::cout << "for key: " << r << " value is: " << TestFunc(r) <<  std::endl;
	}
	return 0;
}
