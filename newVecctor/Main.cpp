#include <iostream>

#include <vector>
#include <string>
#include <fstream>
#include <algorithm>
#include <iomanip>
#include <numeric>

#include "newVector_Header.h"
#include "Timer.h"


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
template< class T>
void test3()
{
	T v1;
	Timer t;
	const std::string somestring = "asdgds";
	for (size_t i = 0; i < 100000; i++)
	{
		v1.push_back(somestring);
		v1.push_back(somestring);
		v1.push_back(somestring);
		v1.push_back(somestring);
		v1.erase(v1.begin(), v1.end()-i);
	}
	std::cout << t.elapsed() << std::endl;
}

template< class T>
double push_back_test(const size_t & size)
{
	T v;
	Timer t;
	const std::string s = "adfghdhdh";
	for (size_t i = 0; i < size; i++)
	{
		v.insert(v.begin(), s);
	}
	return t.elapsed();
}

void tester(const unsigned int& limit, const unsigned int& repeat)
{
	std::vector<double> diff;
	double stdv;
	double newv;
	std::ofstream of("Log_insert_od.txt");

	for (unsigned int j = 0; j < repeat; j++)
	{
		of << j+1 << "---------------------------------------------" << std::endl;
		std::cout << j+1 << "---------------------------------------------" << std::endl;
		for (size_t i = 10; i <= pow(10, limit); i *= 10)
		{
			stdv = push_back_test<std::vector<std::string>>(i);
			newv = push_back_test<newvector<std::string>>(i);
			diff.push_back((1 - newv / stdv) * 100);
			of << "std::vector .size: " << std::setw(8) << std::left << i << " " << stdv << std::endl;
			of << "newvector   .size: " << std::setw(8) << std::left << i << " " << newv << std::endl;
			of << "   " << diff[diff.size() - 1] << "% performance increase" << std::endl;
			std::cout << "std::vector .size: " << std::setw(8) << std::left << i << " " << stdv << std::endl;
			std::cout << "newvector   .size: " << std::setw(8) << std::left << i << " " << newv << std::endl;
			std::cout << "   " << diff[diff.size() - 1] << "% performance increase" << std::endl;

		}
	}
	of << "----------------------------------------------------------------" << std::endl;
	std::cout << "---------------------------------------------------------" << std::endl;
	of << (double)std::accumulate(diff.begin(), diff.end(), 0) / diff.size() << "% performance increase" << std::endl;
	std::cout << (double)std::accumulate(diff.begin(), diff.end(), 0) / diff.size() << "% performance increase" << std::endl;
	of << "----------------------------------------------------------------" << std::endl;
	std::cout << "---------------------------------------------------------" << std::endl;
}

int main()
{
	tester(4, 100);
	system("pause");
	return 0;
}