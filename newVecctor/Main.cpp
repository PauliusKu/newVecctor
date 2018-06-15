#include <iostream>

#include <vector>
#include <string>

#include "newVector_Header.h"


void test0()
{
	newvector<std::string> v1 (4, "dfbdnftgft");
	newvector<std::string> v2(4, "dfbdnft6454645645656456gft");
	newvector<std::string> v{"rssgsgsregg", "fdgsdgdghehherherhre"};
	v.assign(v1.begin(), v1.end());
	std::cout << v.size() << std::endl;
	v.reserve(1000);
	v.shrink_to_fit();
	std::cout << v.capacity() << std::endl;
	//v.clear();
	std::string s = "qwqw";
	v.insert(v.begin() + 4, s);
	v.insert(v.begin() + 4, 2, "f");
	v.insert(v.begin(), 2, "a");
	v.insert(v.begin(), v2.begin(), v2.end());
	v.insert(v.begin(), { "q", "q" });
	v.emplace(v.begin(), "dv");
	for (size_t i = 0; i < v.size(); i++)
	{
		std::cout << v.at(i) << std::endl;
	}
	std::cout << "-------" << std::endl;
	v.emplace_back("fvffdddfbffb");
	//v.pop_back();
	//v.resize(100);
	v.swap(v2);
	for (size_t i = 0; i < v.size(); i++)
	{
		std::cout << v.at(i) << std::endl;
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