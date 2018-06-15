#include <iostream>

#include <vector>
#include <string>

#include "newVector_Header.h"


void test0()
{
	newvector<std::string> v1 (400, "dfbdnftgft");
	newvector<std::string> v{"rssgsgsregg", "fdgsdgdghehherherhre"};
	v.assign(v1.begin(), v1.end());
	std::cout << v.size() << std::endl;
	for (size_t i = 0; i < v.size(); i++)
	{
		std::cout << v[i] << std::endl;
	}
}

void test1()
{
	newvector<char> v;
	v.assign({'g', 'f', 'g', 'f' , 'g', 'f' , 'g', 'f' , 'g', 'f' });
	v.push_back('l');
	v.assign(400, '4');
	for (size_t i = 0; i < v.size(); i++)
	{
		std::cout << v[i] << std::endl;
	}
	std::cout << v.size() << std::endl;
}

void test2()
{
	newvector<char> v{'a', 'b', 'c', 'd'};
	v.insert(v.begin()+0, 'z');
	for (size_t i = 0; i < v.size(); i++)
	{
		std::cout << v[i] << std::endl;
	}
	v.erase(v.begin() + 0);
	v.pop_back();
	for (size_t i = 0; i < v.size(); i++)
	{
		std::cout << v[i] << std::endl;
	}
}


int main()
{
	test0();
	system("pause");
	return 0;
}