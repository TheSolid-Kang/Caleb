#pragma once
#include <Windows.h>
#include <commdlg.h> //open file dialog
#include <string>
#include <tchar.h>

#include <tchar.h>//ExecuteFile () 
#include <stdio.h>//ExecuteFile () 

#include <shlobj_core.h> //open folder dialog

#if UNICODE 
using TString = std::wstring;
#else
using TString = std::string;
#endif;

#define MAX_PATH 1024

class CFileMgr {
public:
	static TString GetOpenFileDialg() {
		OPENFILENAME OFN;
		TCHAR filePathName[MAX_PATH] = _T("");
		TCHAR lpstrFile[MAX_PATH] = _T("");
		static TCHAR filter[] = _T("모든 파일\0*.*\0텍스트 파일\0*.txt\0fbx 파일\0*.fbx");
		HWND hWnd = NULL;

		memset(&OFN, 0, sizeof(OPENFILENAME));
		OFN.lStructSize = sizeof(OPENFILENAME);
		//OFN.hwndOwner = hWnd;
		OFN.hwndOwner = hWnd;
		OFN.lpstrFilter = filter;
		OFN.lpstrFile = lpstrFile;
		OFN.nMaxFile = 100;
		OFN.lpstrInitialDir = _T(".");

		if (GetOpenFileName(&OFN) != 0) {
			wsprintf(filePathName, _T("%s 파일을 선택하시겠습니까?"), OFN.lpstrFile);
			MessageBox(hWnd, filePathName, _T("열기 선택"), MB_OK);

			TString wstr_result(OFN.lpstrFile); //LPWSTR -> std::wstring

			return wstr_result;//파일 경로 담김
		}
		return _T("");
	}
	static void ExecuteFile(const TString& path = GetOpenFileDialg()) {
		HWND hConsole = GetConsoleWindow();
		ShowWindow(hConsole, SW_HIDE);

		SHELLEXECUTEINFO sei;
		ZeroMemory(&sei, sizeof(SHELLEXECUTEINFO));

		sei.cbSize = sizeof(SHELLEXECUTEINFO);
		//sei.lpFile =  L"C:\\ManyoneChurch\\msi\\SQLEXPRWT_x64_KOR.exe";
		sei.lpFile = path.c_str();
		sei.lpParameters = _T("");
		sei.nShow = SW_SHOW;
		sei.fMask = SEE_MASK_NOCLOSEPROCESS;
		sei.lpVerb = _T("open");
		DWORD result = ShellExecuteEx(&sei);
		if (sei.hProcess != NULL)
			WaitForSingleObject(sei.hProcess, INFINITE); //여기서 해당 프로세스가 종료될때까지 대기하게됨

		//ShellExecute(NULL, L"open", "C:\\ManyoneChurch\\msi\\sql.bat", NULL, NULL, SW_SHOW);
		//ShellExecute(NULL, _T("open"), path.c_str(), NULL, NULL, SW_SHOW);

	}

	static TString GetOpenFolderDialg() {

		auto BrowseCallbackProc = [](HWND hWnd, UINT uMsg, LPARAM lParam, LPARAM lpData) -> int CALLBACK{
			switch (uMsg)
			{
			case BFFM_INITIALIZED:      // 폴더 선택 대화상자를 초기화 할 때, 초기 경로 설정
			{
			::SendMessage(hWnd, BFFM_SETSELECTION, TRUE, (LPARAM)lpData);
			}
			break;

			// BROWSEINFO 구조체의 ulFlags 값에 BIF_STATUSTEXT 가 설정된 경우 호출
			// 단, BIF_NEWDIALOGSTYLE 가 설정되어 있을 경우 호출되지 않음
			case BFFM_SELCHANGED:       // 사용자가 폴더를 선택할 경우 대화상자에 선택된 경로 표시
			{
			TCHAR szPath[MAX_PATH] = { 0, };

			::SHGetPathFromIDList((LPCITEMIDLIST)lParam, szPath);
			::SendMessage(hWnd, BFFM_SETSTATUSTEXT, 0, (LPARAM)szPath);
			}
			break;

			// BROWSEINFO 구조체의 ulFlags 값에 BIF_VALIDATE 가 설정된 경우 호출
			// BIF_EDITBOX 와 같이 설정된 경우만 호출됨
			case BFFM_VALIDATEFAILED:   // 에디터 콘트롤에서 폴더 이름을 잘못 입력한 경우 호출
			{
			::MessageBox(hWnd, _T("해당 폴더를 찾을 수 없습니다."), _T("오류"),
			MB_ICONERROR | MB_OK);
			}
			break;
			}

			return 0; };

		BROWSEINFO browse_info;
		TCHAR szTemp[MAX_PATH] = _T("");//저장 위치
		TCHAR* pRootPath = const_cast<TCHAR*>(_T("D:\\"));//시작 위치
		HWND hWnd = NULL;
		ZeroMemory(&browse_info, sizeof(BROWSEINFO));

		browse_info.hwndOwner = hWnd;
		browse_info.lpszTitle = _T("폴더를 선택해주세요.");
		browse_info.ulFlags = BIF_NEWDIALOGSTYLE | BIF_EDITBOX 
			| BIF_RETURNONLYFSDIRS | BIF_STATUSTEXT | BIF_VALIDATE;
		browse_info.lpfn = BrowseCallbackProc;
		browse_info.lParam = (LPARAM)pRootPath;
		browse_info.pszDisplayName = szTemp;

		LPITEMIDLIST pItemIdList = SHBrowseForFolder(&browse_info);

		if (SHGetPathFromIDList(pItemIdList, szTemp)) {
			MessageBox(hWnd, szTemp, _T("open?"), MB_OK);
			return browse_info.pszDisplayName;//szTemp 
		}
		
		return _T("");
	}
};