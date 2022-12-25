#pragma once
#include <iostream>
#include <string>
#include <tchar.h>

#include <vector>
#include <sstream>
#if UNICODE 
using TString = std::wstring;
using tistringstream = std::wistringstream;
#else
using TString = std::string;
using tistringstream = std::istringstream;
#endif;
class CMyEtc
{
public:
	static std::vector<TString> Split(TString& str, TCHAR _delimiter)
	{
		std::vector<TString> vec_result;
		vec_result.reserve(1024);
		tistringstream iss(str);
		TString str_buffer;

		while (std::getline(iss, str_buffer, _delimiter))
			vec_result.push_back(str_buffer);

		return vec_result;
	}
};