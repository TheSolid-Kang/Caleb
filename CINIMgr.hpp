#pragma once
#include <iostream>
#include <string>
#include <Windows.h>
#include <atlconv.h>
#include <filesystem>

class CINIMgr {
public:
#if UNICODE //unicode == ANSI == W
	static std::string GetDefaultConfigPath() {
		//1. �������� ��� ���ϱ� 
		std::wstring wstr = _GetDefaultConfigPathW();
		return std::string(wstr.begin(), wstr.end());
	}
	static std::wstring GetDefaultConfigPathW() {
		//1. �������� ��� ���ϱ� 
		return _GetDefaultConfigPathW();
	}
	static void WritePrivateProfileString_INI(std::string _section, std::string _key, std::string _value, std::string _path = "") {
		_WritePrivateProfileStringW_INI(_section.c_str(), _key.c_str(), _value.c_str(), _path.c_str());
	}
	static std::string GetPrivateProfileString_INI(std::string _section, std::string _key, std::string _path = GetDefaultConfigPath()) {
		return  _GetPrivateProfileStringA_INI(_section.c_str(), _key.c_str(), _path.c_str());
	}
	static std::wstring GetPrivateProfileStringW_INI(std::string _section, std::string _key, std::string _path = GetDefaultConfigPath()) {
		return  _GetPrivateProfileStringW_INI(_section.c_str(), _key.c_str(), _path.c_str());
	}
#else
	static std::string GetDefaultConfigPath() {
		//1. �������� ��� ���ϱ� 
		return _GetDefaultConfigPathA();
	}
	static void WritePrivateProfileString_INI(std::string _section, std::string _key, std::string _value, std::string _path = "") {
		_WritePrivateProfileStringA_INI(_section.c_str(), _key.c_str(), _value.c_str(), _path.c_str());
	}
	static std::string GetPrivateProfileString_INI(std::string _section, std::string _key, std::string _path = GetDefaultConfigPath()) {
		return  _GetPrivateProfileStringA_INI(_section.c_str(), _key.c_str(), _path.c_str());
	}
#endif

private:
	static std::wstring _GetDefaultConfigPathW() {
		//1. �������� ��� ���ϱ� 
		wchar_t path[MAX_PATH] = { 0, };
		GetModuleFileNameW(NULL, path, MAX_PATH);
		std::wstring exe_path = path;
		exe_path = exe_path.substr(0, exe_path.find_last_of(L"\\/"));

		//221123 ����: ��ɿ��� | ��� ini ������ config ���� �ȿ��� �����Ǳ⿡ ����
		exe_path += L"\\configINI";//������
		if (false == std::filesystem::exists(exe_path)) //������ ���ٸ� �����.
			std::filesystem::create_directory(exe_path);
		exe_path += L"\\config.ini";//ini ���� ��

		return exe_path;
	}
	static void _WritePrivateProfileStringW_INI(std::string _section, std::string _key, std::string _value, std::string _path = "") {
		USES_CONVERSION;
		if (_path != "")
			WritePrivateProfileStringW(A2W(_section.c_str()), A2W(_key.c_str()), A2W(_value.c_str()), A2W(_path.c_str()));
		else {
			std::wstring exe_path = _GetDefaultConfigPathW();
			WritePrivateProfileStringW(A2W(_section.c_str()), A2W(_key.c_str()), A2W(_value.c_str()), exe_path.c_str());
		}
	}
	static std::wstring _GetPrivateProfileStringW_INI(std::string _section, std::string _key, std::string _path = GetDefaultConfigPath()) {
		USES_CONVERSION;
		wchar_t szBuffer[1024] = { NULL , };
		GetPrivateProfileStringW(A2W(_section.c_str()), A2W(_key.c_str()), L"0", szBuffer, 1024, A2W(_path.c_str()));
		std::wstring wstr(szBuffer);
		return wstr;
	}
	
	static std::string _GetDefaultConfigPathA() {
		//1. �������� ��� ���ϱ� 
		char path[MAX_PATH] = { 0, };
		GetModuleFileNameA(NULL, path, MAX_PATH);
		std::string exe_path = path;
		exe_path = exe_path.substr(0, exe_path.find_last_of("\\/"));

		//221123 ����: ��ɿ��� | ��� ini ������ config ���� �ȿ��� �����Ǳ⿡ ����
		exe_path += "\\configINI";//������
		if (false == std::filesystem::exists(exe_path)) //������ ���ٸ� �����.
			std::filesystem::create_directory(exe_path);
		exe_path += "\\config.ini";//ini ���� ��

		return exe_path;
	}
	static void _WritePrivateProfileStringA_INI(std::string _section, std::string _key, std::string _value, std::string _path = "") {
		if (_path != "")
			WritePrivateProfileStringA(_section.c_str(), _key.c_str(), _value.c_str(), _path.c_str());
		else {
			std::string exe_path = GetDefaultConfigPath();
			WritePrivateProfileStringA(_section.c_str(), _key.c_str(), _value.c_str(), exe_path.c_str());
		}
	}
	static std::string _GetPrivateProfileStringA_INI(std::string _section, std::string _key, std::string _path = GetDefaultConfigPath()) {
		char szBuffer[1024] = { NULL , };
		GetPrivateProfileStringA(_section.c_str(), _key.c_str(), "0", szBuffer, 1024, _path.c_str());
		return szBuffer;
	}
};