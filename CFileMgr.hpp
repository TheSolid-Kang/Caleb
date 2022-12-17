#pragma once
#include <Windows.h>
#include <commdlg.h> //파일 다이얼로그용.
#include <string>

#include <tchar.h>//ExecuteFile () 
#include <stdio.h>//ExecuteFile () 

class CFileMgr {
public:
#if UNICODE
	static std::wstring GetOpenFileDialg() {
		return GetOpenFileDialgW();
	}
	static void ExecuteFile() {
		ExecuteFileW();
	}
#else
	static std::string GetOpenFileDialg() {
		return GetOpenFileDialgA();
	}
	static void ExecuteFile() {
		ExecuteFileA();
	}
#endif

#if UNICODE
	static std::wstring GetOpenFileDialgW() {
		OPENFILENAME OFN;
		TCHAR filePathName[100] = L"";
		TCHAR lpstrFile[100] = L"";
		static TCHAR filter[] = L"모든 파일\0*.*\0텍스트 파일\0*.txt\0fbx 파일\0*.fbx";
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
			wsprintf(filePathName, L"%s 파일을 선택하시겠습니까?", OFN.lpstrFile);
			MessageBox(hWnd, filePathName, L"열기 선택", MB_OK);

			std::wstring wstr_result(OFN.lpstrFile); //LPWSTR -> std::wstring

			return wstr_result;//파일 경로 담김
		}
		return L"";
	}	
	static void ExecuteFileW(std::wstring path = GetOpenFileDialgW()) {
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
			WaitForSingleObject(sei.hProcess, INFINITE); //여기서 해당 프로세스가 종료될때까지 대기하게됨

		//ShellExecute(NULL, L"open", "C:\\ManyoneChurch\\msi\\sql.bat", NULL, NULL, SW_SHOW);
		ShellExecute(NULL, L"open", path.c_str(), NULL, NULL, SW_SHOW);

	}
#else
	static std::string GetOpenFileDialgA() {
		OPENFILENAME OFN;
		TCHAR filePathName[100] = "";
		TCHAR lpstrFile[100] = "";
		static TCHAR filter[] = "모든 파일\0*.*\0텍스트 파일\0*.txt\0fbx 파일\0*.fbx";
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
			wsprintf(filePathName, "%s 파일을 선택하시겠습니까?", OFN.lpstrFile);
			MessageBox(hWnd, filePathName, "열기 선택", MB_OK);
			auto temp = OFN.lpstrFile;
			std::string str_result(temp);//std::wstring -> std::string

			return str_result;//파일 경로 담김
		}
		return "";
	}
	static void ExecuteFileA(std::string path = GetOpenFileDialgA()) {
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
			WaitForSingleObject(sei.hProcess, INFINITE); //여기서 해당 프로세스가 종료될때까지 대기하게됨

		//ShellExecute(NULL, L"open", "C:\\ManyoneChurch\\msi\\sql.bat", NULL, NULL, SW_SHOW);
		ShellExecute(NULL, "open", path.c_str(), NULL, NULL, SW_SHOW);

	}
#endif
};