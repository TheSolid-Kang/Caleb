#pragma once
#include <commdlg.h> //파일 다이얼로그용.
#include <fstream>
#include <string>
#include <filesystem>
#include <windows.h>
#include <atlconv.h>

class CFileMgr {
public:
	static std::string GetOpenFileDialg() {
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
			std::string str_result(wstr_result.begin(), wstr_result.end());//std::wstring -> std::string

			return str_result;//파일 경로 담김

			int a = 0;
		}
		return "";
	}
};