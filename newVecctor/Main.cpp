#include <iostream>

#include <vector>

#include "newVector_Header.h"


void test0(size_t s)
{
	newvector<int> v(s);
	v.push_back(5);
	std::cout << v[5000] << std::endl;
}

void test1()
{
	newvector<char> v;
	v.assign({'g', 'f', 'g', 'f' , 'g', 'f' , 'g', 'f' , 'g', 'f' });
	v.push_back('l');
	std::cout << v.size() << std::endl;
	std::cout << v.max_size() << std::endl;
}

void test2()
{
	newvector<int> v{10, 4, 1 };
	v.erase(v.begin()+1, v.end()-1);
	std::cout << v[0] << std::endl;
	std::cout << v.size() << std::endl;
}


int main()
{
	for (int i = 0; i < 100; i++)
	{
		//test1();
	}
	test2();
	system("pause");
	return 0;
}