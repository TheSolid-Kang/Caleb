#pragma once
#include <iostream>
#include <string>
#include <Windows.h>
#include <atlconv.h>
#include <filesystem>

class CINIMgr {
public:
	static std::string GetDefaultConfigPath() {
		//1. 실행파일 경로 구하기 
#if Multibyte
		char path[MAX_PATH] = { 0, };
		GetModuleFileName(NULL, path, MAX_PATH);
		USES_CONVERSION;
		std::string exe_path = path;
#else
		wchar_t path[MAX_PATH] = { 0, };
		GetModuleFileName(NULL, path, MAX_PATH);
		USES_CONVERSION;
		std::string exe_path = W2A(path);
#endif
		exe_path = exe_path.substr(0, exe_path.find_last_of("\\/"));

		//221123 삭제: 기능오류 | 모든 ini 파일이 config 폴더 안에만 생성되기에 삭제
		exe_path += "\\configINI";//폴더명
		if (false == std::filesystem::exists(exe_path)) //폴더가 없다면 만든다.
			std::filesystem::create_directory(exe_path);
		exe_path += "\\config.ini";//ini 파일 명

		return exe_path;
	}

public:
	static void WritePrivateProfileStringA_INI(std::string _section, std::string _key, std::string _value, std::string _path = "") {
		if (_path != "")
			WritePrivateProfileStringA(_section.c_str(), _key.c_str(), _value.c_str(), _path.c_str());
		else {
			std::string exe_path = GetDefaultConfigPath();
			WritePrivateProfileStringA(_section.c_str(), _key.c_str(), _value.c_str(), exe_path.c_str());
		}
	}
	static std::string GetPrivateProfileStringA_INI(std::string _section, std::string _key, std::string _path = GetDefaultConfigPath()) {
//#ifdef _WIN32
//		SetConsoleOutputCP(CP_UTF8);
//#endif
		char szBuffer[1024] = { NULL , };
		GetPrivateProfileStringA(_section.c_str(), _key.c_str(), "0", szBuffer, 1024, _path.c_str());
		return szBuffer;
	}

};