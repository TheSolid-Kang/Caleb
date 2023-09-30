#pragma once
#include <iostream>
#include <string>
#include <tchar.h>
#include <vector>
#include <Windows.h>
#include <sstream>
#include <chrono>

class StringEditor {
#define DEFAULT_CAP 2048
#if UNICODE 
	using TString = std::wstring;
	using tifstream = std::wifstream;
	using tofstream = std::wofstream;
	using tistringstream = std::wistringstream;
	using tstring_view = std::wstring_view;
	using tstringstream = std::wstringstream;
#define tcout  wcout
#define tcin  wcin
#else
	using TString = std::string;
	using tifstream = std::ifstream;
	using tofstream = std::ofstream;
	using tistringstream = std::istringstream;
	using tstring_view = std::string_view;
	using tstringstream = std::stringstream;
#define tcout  cout
#define tcin  cin
#endif;

public:
	static std::vector<TString> Split(const TString& str, TCHAR _delimiter)
	{
		std::vector<TString> vec_result;
		vec_result.reserve(1024);
		tistringstream iss(str);
		TString str_buffer;

		while (std::getline(iss, str_buffer, _delimiter))
			vec_result.push_back(str_buffer);
		vec_result.shrink_to_fit();
		return vec_result;
	}
	static TString ToLower(TString& _str) {

		for (TCHAR& _ch : _str) {
			if ('A' <= _ch && 'Z' >= _ch) {
				_ch |= 32;
			}
		}
		return _str;
	}

	static TString ToUpper(TString& _str) {

		for (TCHAR& _ch : _str) {
			if ('a' <= _ch && 'z' >= _ch) {
				_ch -= 32;
			}
		}
		return _str;
	}
	template<typename T>
	static TString ToString(const T& _t) {
#if UNICODE
		std::wstringstream ss;
		ss << _t;
		return ss.str();
		//return std::to_wstring(_t);
#else
		std::stringstream ss;
		ss << _t;
		return ss.str();
		//return std::to_string(_t);
#endif;
	}
	static TString GetCurTime() {
		std::chrono::system_clock::time_point beginPoint;
		beginPoint = std::chrono::system_clock::now();
		std::time_t begin_time = std::chrono::system_clock::to_time_t(beginPoint);

		TString strBeginTime;
#if UNICODE
		std::string str = std::ctime(&begin_time);
		strBeginTime.assign(str.begin(), str.end());
#else
		strBeginTime = std::ctime(&begin_time);
#endif;
		size_t index = strBeginTime.find(_T("\n"));
		if (index) {
			strBeginTime.erase(index);
		}
		return strBeginTime;
	}

	static TString GetExceptionInfo(std::exception _e) {
#if UNICODE
		TCHAR* pStr = nullptr;
		int size = MultiByteToWideChar(CP_ACP, 0, _e.what(), -1, NULL, NULL);
		pStr = new TCHAR[size];
		MultiByteToWideChar(CP_ACP, 0, _e.what(), static_cast<int>(strlen(_e.what()) + 1), pStr, size);
		TString str = pStr;
		delete[] pStr;
#else
		TString str = _e.what();
#endif


		return str;
	}

};