#include <iostream>

#include <vector>

#include "newVector_Header.h"


void test(size_t s)
{
	newvector<int> v(s);
	v.push_back(5);
	std::cout << v[5000] << std::endl;
}


int main()
{
	for (int i = 0; i < 100; i++)
	{
		test(5000000);
	}
	system("pause");
	return 0;
}