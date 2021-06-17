/*
 * main.cpp
 *
 *  Created on: Jun 16, 2021
 *      Author: Ahmed Nasr
 */

#include <iostream>
#include <sstream>
#include "Hashmap.h"

void stringIntHashmap()
{
	mymap<std::string, int> m;
	std::ostringstream oss;

	for(int i = 0; i <= 30; i++)
	{
		oss.str("");
		oss << "abc" << i;
		m.insert(oss.str(), i);
		std::cout << "inserting: " << oss.str() << " load factor: " << m.getLoadFactor() << std::endl;
	}

	std::cout << "============================================" << std::endl;

	for(int i = 0; i <= 35; i++)
	{
		oss.str("");
		oss << "abc" << i;
		std::cout << "key: abc" << i << " value: " << m.getValue(oss.str()) << std::endl;
	}

	std::cout << "size: " << m.size() << std::endl;

	std::cout << "============================================" << std::endl;

	for(int i = 10; i <= 20; i++)
	{
		oss.str("");
		oss << "abc" << i;
		std::cout << "removing key: abc" << i << " value: " << m.remove(oss.str()) << std::endl;
	}

	std::cout << "============================================" << std::endl;

	for(int i = 0; i <= 35; i++)
	{
		oss.str("");
		oss << "abc" << i;
		std::cout << "key: abc" << i << " value: " << m.getValue(oss.str()) << std::endl;
	}

	std::cout << "size: " << m.size() << std::endl;

	std::cout << "============================================" << std::endl;

	m["abc1"] = -846;
	std::cout << "key: abc1 value: " << m["abc1"] << std::endl;
	std::cout << "size: " << m.size() << std::endl;

	m["testString"] = 35;
	std::cout << "key: testString value: " << m["testString"] << std::endl;
	std::cout << "size: " << m.size() << std::endl;

	std::cout << "key: notExist value: " << m["notExist"] << std::endl;
	std::cout << "size: " << m.size() << std::endl;

	std::cout << "============================================" << std::endl;

	std::cout << "=== Keys ===" << std::endl;
	for(const auto &key: m.getAllKeys())
		std::cout << key << std::endl;

	std::cout << "============================================" << std::endl;
}

void unsignedIntStringHashmapTest()
{
	mymap<unsigned int, std::string> m;
	std::ostringstream oss;

	for(unsigned int i = 0; i <= 30; i++)
	{
		oss.str("");
		oss << "abc" << i;
		m.insert(i, oss.str());
		std::cout << "inserting: " << oss.str() << " load factor: " << m.getLoadFactor() << std::endl;
	}

	std::cout << "============================================" << std::endl;

	for(unsigned int i = 0; i <= 35; i++)
		std::cout << "key: " << i << " value: " << m.getValue(i) << std::endl;

	std::cout << "size: " << m.size() << std::endl;

	std::cout << "============================================" << std::endl;

	for(unsigned int i = 10; i <= 20; i++)
		std::cout << "removing key: " << i << " value: " << m.remove(i) << std::endl;

	std::cout << "============================================" << std::endl;

	for(unsigned int i = 0; i <= 35; i++)
		std::cout << "key: " << i << " value: " << m.getValue(i) << std::endl;

	std::cout << "size: " << m.size() << std::endl;

	std::cout << "============================================" << std::endl;

	m[21] = "I changed this string";
	std::cout << "key: 21 value: " << m[21] << std::endl;
	std::cout << "size: " << m.size() << std::endl;

	m[58] = "new record";
	std::cout << "key: 58 value: " << m[58] << std::endl;
	std::cout << "size: " << m.size() << std::endl;

	std::cout << "key: 95 value: " << m[95] << std::endl;
	std::cout << "size: " << m.size() << std::endl;

	std::cout << "============================================" << std::endl;

	std::cout << "=== Keys ===" << std::endl;
	for(const auto &key: m.getAllKeys())
		std::cout << key << std::endl;

	std::cout << "============================================" << std::endl;
}

void printMajorityElement(const std::vector<short int> nums)
{
	mymap<short int, size_t> m;

	for(const auto &i: nums)
		m[i]++;

	std::pair<short int, size_t> max = m.max_element();

	std::cout << "major element ---> key: " << max.first << " value: " << max.second << std::endl;

	std::cout << "============================================" << std::endl;
}

int main()
{
	stringIntHashmap();
	unsignedIntStringHashmapTest();
	printMajorityElement(std::vector<short int>{1,2,3,4,5,6,6,6,7,8,9,10,1,1,6,2,3,4});

	return 0;
}
