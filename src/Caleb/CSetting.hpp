#pragma once
#include "CFIOMgr.hpp"
#include "CINIMgr.hpp"
#include <map>
#if UNICODE 
using MyString = std::wstring;
#define tcout  wcout
#define tcin  wcin
#else
using MyString = std::string;
#define tcout  cout
#define tcin  wcin
#endif;
class CSetting
{
private:
	CSetting() {
		ExePath = CFIOMgr::_GetEXEFilePath();
		DiaryFormPath = CINIMgr::_GetPrivateProfileString_INI(_T("PATH"), _T("DIRY_FORM_PATH"));
		DiaryDirPath = CINIMgr::_GetPrivateProfileString_INI(_T("PATH"), _T("DIRY_PATH"));
		CalebFormPath = CINIMgr::_GetPrivateProfileString_INI(_T("PATH"), _T("CALEB_FORM_PATH"));
		CalebDirPath = CINIMgr::_GetPrivateProfileString_INI(_T("PATH"), _T("CALEB_PATH"));
	}
public:
	static CSetting& GetInstance(void) noexcept { static CSetting instance; return instance; }

private:
	MyString ExePath;
	MyString DiaryFormPath;
	MyString DiaryDirPath;
	MyString CalebFormPath;
	MyString CalebDirPath;

	std::map<MyString, std::map<MyString, MyString>> m_map_config;


};