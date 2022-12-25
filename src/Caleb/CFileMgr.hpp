#pragma once
#include <Windows.h>
#include <commdlg.h> //���� ���̾�α׿�.
#include <string>
#include <tchar.h>

#include <tchar.h>//ExecuteFile () 
#include <stdio.h>//ExecuteFile () 

#if UNICODE 
using MyString = std::wstring;
#else
using MyString = std::string;
#endif;

class CFileMgr {
public:
#if UNICODE //unicode == ANSI == W
	static std::string GetOpenFileDialg() {
		std::wstring wstr_path = _GetOpenFileDialgW();
		std::string str_path(wstr_path.begin(), wstr_path.end());
		return str_path;
	}
	static std::wstring GetOpenFileDialgW() {
		return _GetOpenFileDialgW();
	}

	static void ExecuteFile(std::string _path = GetOpenFileDialg()) {
		std::wstring wstr_path(_path.begin(), _path.end());
		_ExecuteFileW(wstr_path);
	}
	static void ExecuteFileW(std::wstring _path = _GetOpenFileDialgW()) {
		_ExecuteFileW(_path);
	}
#else
	static std::string GetOpenFileDialg() {
		return _GetOpenFileDialgA();
	}
	static void ExecuteFile() {
		_ExecuteFileA();
	}
#endif
private:
#if UNICODE //unicode == ANSI == W
	static std::wstring _GetOpenFileDialgW() {
		OPENFILENAME OFN;
		TCHAR filePathName[100] = L"";
		TCHAR lpstrFile[100] = L"";
		static TCHAR filter[] = L"��� ����\0*.*\0�ؽ�Ʈ ����\0*.txt\0fbx ����\0*.fbx";
		HWND hWnd = NULL;

		memset(&OFN, 0, sizeof(OPENFILENAME));
		OFN.lStructSize = sizeof(OPENFILENAME);
		//OFN.hwndOwner = hWnd;
		OFN.hwndOwner = hWnd;
		OFN.lpstrFilter = filter;
		OFN.lpstrFile = lpstrFile;
		OFN.nMaxFile = 100;
		OFN.lpstrInitialDir = L".";

		if (GetOpenFileName(&OFN) != 0) {
			wsprintf(filePathName, L"%s ������ �����Ͻðڽ��ϱ�?", OFN.lpstrFile);
			MessageBox(hWnd, filePathName, L"���� ����", MB_OK);

			std::wstring wstr_result(OFN.lpstrFile); //LPWSTR -> std::wstring

			return wstr_result;//���� ��� ���
		}
		return L"";
	}	
	static void _ExecuteFileW(std::wstring path = _GetOpenFileDialgW()) {
		HWND hConsole = GetConsoleWindow();
		ShowWindow(hConsole, SW_HIDE);

		SHELLEXECUTEINFO sei;
		ZeroMemory(&sei, sizeof(SHELLEXECUTEINFO));

		sei.cbSize = sizeof(SHELLEXECUTEINFO);
		//sei.lpFile =  L"C:\\ManyoneChurch\\msi\\SQLEXPRWT_x64_KOR.exe";
		//sei.lpFile = path.c_str();
		sei.lpParameters = L"";
		sei.nShow = SW_SHOW;
		sei.fMask = SEE_MASK_NOCLOSEPROCESS;
		sei.lpVerb = L"open";
		DWORD result = ShellExecuteEx(&sei);
		if (sei.hProcess != NULL)
			WaitForSingleObject(sei.hProcess, INFINITE); //���⼭ �ش� ���μ����� ����ɶ����� ����ϰԵ�

		//ShellExecute(NULL, L"open", "C:\\ManyoneChurch\\msi\\sql.bat", NULL, NULL, SW_SHOW);
		ShellExecute(NULL, L"open", path.c_str(), NULL, NULL, SW_SHOW);

	}
#else
	static std::string _GetOpenFileDialgA() {
		OPENFILENAME OFN;
		TCHAR filePathName[100] = "";
		TCHAR lpstrFile[100] = "";
		static TCHAR filter[] = "��� ����\0*.*\0�ؽ�Ʈ ����\0*.txt\0fbx ����\0*.fbx";
		HWND hWnd = NULL;

		memset(&OFN, 0, sizeof(OPENFILENAME));
		OFN.lStructSize = sizeof(OPENFILENAME);
		//OFN.hwndOwner = hWnd;
		OFN.hwndOwner = hWnd;
		OFN.lpstrFilter = filter;
		OFN.lpstrFile = lpstrFile;
		OFN.nMaxFile = 100;
		OFN.lpstrInitialDir = ".";

		if (GetOpenFileName(&OFN) != 0) {
			wsprintf(filePathName, "%s ������ �����Ͻðڽ��ϱ�?", OFN.lpstrFile);
			MessageBox(hWnd, filePathName, "���� ����", MB_OK);
			auto temp = OFN.lpstrFile;
			std::string str_result(temp);//LPSTR -> std::string

			return str_result;//���� ��� ���
		}
		return "";
	}
	static void _ExecuteFileA(std::string path = _GetOpenFileDialgA()) {
		HWND hConsole = GetConsoleWindow();
		ShowWindow(hConsole, SW_HIDE);

		SHELLEXECUTEINFO sei;
		ZeroMemory(&sei, sizeof(SHELLEXECUTEINFO));

		sei.cbSize = sizeof(SHELLEXECUTEINFO);
		//sei.lpFile =  L"C:\\ManyoneChurch\\msi\\SQLEXPRWT_x64_KOR.exe";
		//sei.lpFile = path.c_str();
		sei.lpParameters = "";
		sei.nShow = SW_SHOW;
		sei.fMask = SEE_MASK_NOCLOSEPROCESS;
		sei.lpVerb = "open";
		DWORD result = ShellExecuteEx(&sei);
		if (sei.hProcess != NULL)
			WaitForSingleObject(sei.hProcess, INFINITE); //���⼭ �ش� ���μ����� ����ɶ����� ����ϰԵ�

		//ShellExecute(NULL, L"open", "C:\\ManyoneChurch\\msi\\sql.bat", NULL, NULL, SW_SHOW);
		ShellExecute(NULL, "open", path.c_str(), NULL, NULL, SW_SHOW);

	}
#endif
};