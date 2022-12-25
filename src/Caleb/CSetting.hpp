#pragma once
#include "CFIOMgr.hpp"
#include "CINIMgr.hpp"
#include <map>
#if UNICODE 
using TString = std::wstring;
#define tcout  wcout
#define tcin  wcin
#else
using TString = std::string;
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
	TString ExePath;
	TString DiaryFormPath;
	TString DiaryDirPath;
	TString CalebFormPath;
	TString CalebDirPath;

	std::map<TString, std::map<TString, TString>> m_map_config;


};