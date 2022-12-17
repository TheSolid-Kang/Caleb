#pragma once
#include "CFIOMgr.hpp"
#include "CINIMgr.hpp"
#include <map>

class CSetting
{
private:
	CSetting() {
		ExePath = CFIOMgr::GetEXEFilePath();
		DiaryFormPath = CINIMgr::GetPrivateProfileString_INI("PATH", "DIRY_FORM_PATH");
		DiaryDirPath = CINIMgr::GetPrivateProfileString_INI("PATH", "DIRY_PATH");
		CalebFormPath = CINIMgr::GetPrivateProfileString_INI("PATH", "CALEB_FORM_PATH");
		CalebDirPath = CINIMgr::GetPrivateProfileString_INI("PATH", "CALEB_PATH");
	}
public:
	static CSetting& GetInstance(void) noexcept { static CSetting instance; return instance; }

private:
	std::string ExePath;
	std::string DiaryFormPath;
	std::string DiaryDirPath;
	std::string CalebFormPath;
	std::string CalebDirPath;

	std::map<std::string, std::map<std::string, std::string>> m_map_config;


};