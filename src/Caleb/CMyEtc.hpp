#pragma once
#include <iostream>
#include <string>
#include <tchar.h>

#include <vector>
#include <sstream>
#if UNICODE 
using MyString = std::wstring;
using tistringstream = std::wistringstream;
#else
using MyString = std::string;
using tistringstream = std::istringstream;
#endif;
class CMyEtc
{
public:
	static std::vector<MyString> Split(MyString& str, TCHAR _delimiter)
	{
		std::vector<MyString> vec_result;
		vec_result.reserve(1024);
		tistringstream iss(str);
		MyString str_buffer;

		while (std::getline(iss, str_buffer, _delimiter))
			vec_result.push_back(str_buffer);

		return vec_result;
	}
};