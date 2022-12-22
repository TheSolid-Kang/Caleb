#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <sstream>

class CMyEtc
{
public:
	static std::vector<std::string> Split(std::string str, char _delimiter)
	{
		std::vector<std::string> vec_result;
		vec_result.reserve(1024);
		std::istringstream iss(str);
		std::string str_buffer;

		while (std::getline(iss, str_buffer, _delimiter))
			vec_result.push_back(str_buffer);

		return vec_result;
	}
};