#pragma once
#include <commdlg.h> //���� ���̾�α׿�.
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
			std::string str_result(wstr_result.begin(), wstr_result.end());//std::wstring -> std::string

			return str_result;//���� ��� ���

			int a = 0;
		}
		return "";
	}
};