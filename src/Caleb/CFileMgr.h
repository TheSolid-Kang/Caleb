#pragma once
#include <Windows.h>
#include <commdlg.h> //open file dialog
#include <iostream>

#include <string>
#include <tchar.h>

#include <tchar.h>//ExecuteFile () 
#include <stdio.h>//ExecuteFile () 

#include <shlobj_core.h> //open folder dialog
#include <string>
#include <atlcore.h>


#include <fstream>
#include <filesystem>

#include "StringBuilder.h"
#include "StringEditor.h"
#include <mutex>
#include <thread>
#include "CLogger.h"

namespace fs = std::filesystem;
class CFileMgr {
#if UNICODE 
	using TString = std::wstring;
	using tifstream = std::wifstream;
	using tofstream = std::wofstream;
	using tistringstream = std::wistringstream;
	using tstring_view = std::wstring_view;
	using tstringstream = std::wstringstream;
#define tcout  wcout
#define tcin  wcin
#else
	using TString = std::string;
	using tifstream = std::ifstream;
	using tofstream = std::ofstream;
	using tistringstream = std::istringstream;
	using tstring_view = std::string_view;
	using tstringstream = std::stringstream;
#define tcout  cout
#define tcin  cin
#endif;
public:
	static TString GetOpenFileDialg() {
		OPENFILENAME OFN;
		TCHAR filePathName[MAX_PATH] = _T("");
		TCHAR lpstrFile[MAX_PATH] = _T("");
		TCHAR filter[] = _T("모든 파일\0*.*\0텍스트 파일\0*.txt\0fbx 파일\0*.fbx");
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
		return _T("C:\\");
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

	static TString GetOpenFolderDialg(TString _path = (TString)_T("D:\\")) {

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

			return 0;
		};

		BROWSEINFO browse_info;
		TCHAR szTemp[MAX_PATH] = _T("");//저장 위치
		TCHAR* pRootPath = const_cast<TCHAR*>(_path.c_str());//시작 위치
		//const_cast<TCHAR*>(_T("D:\\"));//시작 위치
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

	static TCHAR* GetFileType(TCHAR* _path)
	{
		static TCHAR buf[MAX_PATH] = _T("");
		bool ret = false;
		TCHAR* ptr = nullptr;
#if UNICODE
		ptr = wcsrchr(_path, _T('.'));
		if (ptr == nullptr)
			return nullptr;
		wcscpy(buf, ptr + 1);//wcscpy 함수 사용하기 위해 C++/전처리기: _CRT_SECURE_NO_WARNINGS 추가함.

#else
		ptr = strrchr(_path, _T('.'));
		if (ptr == nullptr)
			return nullptr;
		strcpy(buf, ptr + 1);
#endif




		return buf;
	}

	static std::vector<TString> getTxtFiles(const TString& dirPath) {
		std::vector<TString> txtFiles;
		TString searchPath = dirPath + _T("\\*.txt");
		WIN32_FIND_DATA fd;
		HANDLE hFind = ::FindFirstFile(searchPath.c_str(), &fd);
		if (hFind != INVALID_HANDLE_VALUE) {
			do {
				txtFiles.push_back(fd.cFileName);
			} while (::FindNextFile(hFind, &fd));
			::FindClose(hFind);
		}
		return txtFiles;
	}
	//0. 실행파일 경로 구하기
	static TString GetEXEFilePath() {
		//1. 실행파일 경로 구하기 
		TCHAR path[MAX_PATH] = { 0, };
		GetModuleFileName(NULL, path, MAX_PATH);
		TString exe_path = path;
		exe_path = exe_path.substr(0, exe_path.find_last_of(_T("\\/")));
		return exe_path;
	}
	//1-1. Directory 내 파일 생성
	static void CreateNewFile(TString _path) {
		TString file_path = _path;
		tofstream fout = tofstream(file_path);//파일 열기_만약 파일이 없으면 만듦.

		std::tcout << _T("fout.is_open() == ") << fout.is_open() << std::endl;
		if (true == fout.is_open())
			fout.close();
	}
	//1-2. Directory 내 하위 폴더 차례대로 생성
	static void CreateDirectorys(const TString& _path) {
		TString path(_path.begin(), _path.end());
		TCHAR arr_dir_name[256] = { 0, };
		TCHAR* ch_ptr_path = const_cast<TCHAR*>(path.c_str());
		TCHAR* ch_ptr_dirname = arr_dir_name;

		while (*ch_ptr_path) {
			if ((_T('\\') == *ch_ptr_path) || (_T('/') == *ch_ptr_path)) {
				if (_T(':') != *(ch_ptr_path - 1))
					CreateDirectory(arr_dir_name, NULL);
			}
			*ch_ptr_dirname++ = *ch_ptr_path++;
			*ch_ptr_dirname = _T('\0');
		}
		CreateDirectory(arr_dir_name, NULL);
	}


	//2-1. Directory 내 파일 목록 출력
	static std::vector<TString> GetFilesInDirectory(TString& _path) {
		std::vector<TString> vec_files;
		vec_files.reserve(1024);
		auto iter = fs::directory_iterator(_path);
		while (true != iter._At_end()) {
#if UNICODE 
			vec_files.emplace_back((*iter++).path().wstring());//fs::path -> std::string 으로 변환
#else
			vec_files.emplace_back((*iter++).path().string());//fs::path -> std::string 으로 변환
#endif;
		}
		vec_files.shrink_to_fit();
		return vec_files;
	}
	//2-2. Directory 내 파일 목록 출력
	static std::vector<TString> GetRecursiveFilesInDirectory(const TString& _path) {
		std::vector<TString> vec_files;
		vec_files.reserve(1024);
		auto iter = fs::directory_iterator(_path);
		TString str_file_path = _T("");
		while (true != iter._At_end()) {
			try {
				fs::path file_path = (*iter++).path();
#if UNICODE 
				str_file_path = file_path.wstring();//fs::path -> std::wstring 으로 변환
#else
				str_file_path = file_path.string();//fs::path -> std::string 으로 변환
#endif;
				if (fs::is_directory(file_path)) {
					auto vec_recursive_file = GetRecursiveFilesInDirectory(str_file_path);
					vec_files.insert(vec_files.end(), vec_recursive_file.begin(), vec_recursive_file.end());
				}
				else
				{
					auto size = file_path.extension().string().size();
					vec_files.emplace_back(str_file_path);
				}
			}
			catch (std::exception _e) {

#if UNICODE 
				TCHAR* pStr = nullptr;
				int size = MultiByteToWideChar(CP_ACP, 0, _e.what(), -1, NULL, NULL);
				pStr = new TCHAR[size];
				MultiByteToWideChar(CP_ACP, 0, _e.what(), static_cast<int>(strlen(_e.what()) + 1), pStr, size);
				std::tcout << pStr << _T("\n: ") << str_file_path << _T("\n: ") << std::endl;
#else
				std::tcout << _e.what() << _T("\n: ") << str_file_path << _T("\n: ") << std::endl;
#endif;
			}

		}
		vec_files.shrink_to_fit();
		return vec_files;
	}
	static std::vector<TString> GetRecursiveFilesInDirectory(const TString& _path, const int _extentionVolume) {
		std::vector<TString> vec_files;
		vec_files.reserve(1024);
		auto iter = fs::directory_iterator(_path);
		TString str_file_path = _T("");
		while (true != iter._At_end()) {
			try {
				fs::path file_path = (*iter++).path();
#if UNICODE
				str_file_path = file_path.wstring();//fs::path -> std::wstring 으로 변환
#else
				str_file_path = file_path.string();//fs::path -> std::string 으로 변환
#endif;
				if (fs::is_directory(file_path)) {
					auto vec_recursive_file = GetRecursiveFilesInDirectory(str_file_path, _extentionVolume);
					vec_files.insert(vec_files.end(), vec_recursive_file.begin(), vec_recursive_file.end());
				}
				else {
					auto size = file_path.extension().string().size();
					if (size > _extentionVolume) {
						vec_files.emplace_back(str_file_path);
					}
				}
			}
			catch (std::exception _e) {
#if UNICODE 
				TCHAR* pStr = nullptr;
				int size = MultiByteToWideChar(CP_ACP, 0, _e.what(), -1, NULL, NULL);
				pStr = new TCHAR[size];
				MultiByteToWideChar(CP_ACP, 0, _e.what(), static_cast<int>(strlen(_e.what()) + 1), pStr, size);
				std::tcout << pStr << _T("\n: ") << str_file_path << _T("\n: ") << std::endl;
#else
				std::tcout << _e.what() << _T("\n: ") << str_file_path << _T("\n: ") << std::endl;
#endif;
			}

		}
		vec_files.shrink_to_fit();
		return vec_files;
	}
	static std::vector<TString> GetRecursiveFilesInDirectory(const TString& _path, const int _extentionVolume, const std::map<TCHAR, std::vector<TString>>& _chmapExceptionList) {
		std::vector<TString> vec_files;
		vec_files.reserve(1024);
		auto iter = fs::directory_iterator(_path);
		TString str_file_path = _T("");
		while (true != iter._At_end()) {
			try {
				fs::path file_path = (*iter++).path();
#if UNICODE 
				str_file_path = file_path.wstring();//fs::path -> std::wstring 으로 변환
#else
				str_file_path = file_path.string();//fs::path -> std::string 으로 변환
#endif;
				if (fs::is_directory(file_path)) {
					auto vec_recursive_file = GetRecursiveFilesInDirectory(str_file_path, _extentionVolume, _chmapExceptionList);
					vec_files.insert(vec_files.end(), vec_recursive_file.begin(), vec_recursive_file.end());
				}
				else {
#if UNICODE 
					TString extension = file_path.extension();
#else
					TString extension = file_path.extension().string();
#endif

					extension = StringEditor::ToUpper(extension);
					if (extension.size() < 1) { //확장자의 사이즈가 1 미만이라면 continue;
						continue;
					}
					size_t size = extension.size();
					if (size > _extentionVolume) {
						TCHAR chKey = extension[1];
						if (_chmapExceptionList.find(chKey) == _chmapExceptionList.end()) {
							vec_files.emplace_back(str_file_path);
						}
						else {
							auto vecExceptionList = _chmapExceptionList.at(chKey);
							auto iter = std::find(vecExceptionList.begin(), vecExceptionList.end(), extension);
							if (iter == vecExceptionList.end()) { //예외 목록에 포함된 확장자가 아니라면 vec_files에 담는다.
								vec_files.emplace_back(str_file_path);
							}
						}
					}

				}
			}
			catch (std::exception _e) {
#if UNICODE 
				TCHAR* pStr = nullptr;
				int size = MultiByteToWideChar(CP_ACP, 0, _e.what(), -1, NULL, NULL);
				pStr = new TCHAR[size];
				MultiByteToWideChar(CP_ACP, 0, _e.what(), static_cast<int>(strlen(_e.what()) + 1), pStr, size);
				std::tcout << pStr << _T("\n: ") << str_file_path << _T("\n: ") << std::endl;
#else
				std::tcout << _e.what() << _T("\n: ") << str_file_path << _T("\n: ") << std::endl;
#endif;
			}

		}
		vec_files.shrink_to_fit();
		return vec_files;
	}
	static std::vector<TString> GetRecursiveFilesInDirectory(TString&& _path, const int _extentionVolume, std::map<TCHAR, std::vector<TString>>&& _chmapExceptionList) {
		std::vector<TString> vec_files;
		vec_files.reserve(1024);
		auto iter = fs::directory_iterator(_path);
		TString str_file_path = _T("");
		while (true != iter._At_end()) {
			try {
				fs::path file_path = (*iter++).path();
#if UNICODE 
				str_file_path = file_path.wstring();//fs::path -> std::wstring 으로 변환
#else
				str_file_path = file_path.string();//fs::path -> std::string 으로 변환
#endif;
				if (true == fs::is_directory(file_path)) {
					auto vec_recursive_file = GetRecursiveFilesInDirectory(std::move(str_file_path), _extentionVolume, std::move(_chmapExceptionList));
					vec_files.insert(vec_files.end(), vec_recursive_file.begin(), vec_recursive_file.end());
				}
				else {
#if UNICODE 
					TString extension = file_path.extension();
#else
					TString extension = file_path.extension().string();
#endif
					if (extension.size() < 1) { //확장자의 사이즈가 1 미만이라면 continue;
						continue;
					}
					size_t size = extension.size();
					if (size > _extentionVolume) {
						extension = StringEditor::ToUpper(extension);
						TCHAR chKey = extension[1];
						if (_chmapExceptionList.find(chKey) == _chmapExceptionList.end()) {
							vec_files.emplace_back(str_file_path);
						}
						else {
							auto vecExceptionList = _chmapExceptionList.at(chKey);
							auto iter = std::find(vecExceptionList.begin(), vecExceptionList.end(), extension);
							if (iter == vecExceptionList.end()) { //예외 목록에 포함된 확장자가 아니라면 vec_files에 담는다.
								vec_files.emplace_back(str_file_path);
							}
						}
					}
				}
			}
			catch (std::exception _e) {
#if UNICODE 
				TCHAR* pStr = nullptr;
				int size = MultiByteToWideChar(CP_ACP, 0, _e.what(), -1, NULL, NULL);
				pStr = new TCHAR[size];
				MultiByteToWideChar(CP_ACP, 0, _e.what(), static_cast<int>(strlen(_e.what()) + 1), pStr, size);
				std::tcout << pStr << _T("\n: ") << str_file_path << _T("\n: ") << std::endl;
#else
				std::tcout << _e.what() << _T("\n: ") << str_file_path << _T("\n: ") << std::endl;
#endif;
			}

		}
		vec_files.shrink_to_fit();
		return vec_files;
	}

	static std::vector<TString> GetRecursiveFilesInDirectory2(const TString& _path, const int _extentionVolume, const std::map<TCHAR, std::vector<TString>>& _chmapExceptionList) {
		std::thread::id this_id = std::this_thread::get_id();
		CLogger::GetInstance()->LogBegin(_T(" GetRecursiveFilesInDirectory2()"));
		std::vector<TString> vec_files;

		vec_files.reserve(DEFAULT_CAP);
		TString str_file_path = _T("");
		TString extension = _T("");
		auto iter_fs = fs::recursive_directory_iterator(_path, fs::directory_options::follow_directory_symlink | fs::directory_options::skip_permission_denied);
		fs::recursive_directory_iterator iter_temp = iter_fs;
		for (; iter_fs != fs::recursive_directory_iterator();) {
			try {
				fs::path file_path;
				iter_temp = iter_fs;
				file_path = (*iter_fs++).path();



				if (fs::is_directory(file_path)) {
					continue;
				}
				else {
#if UNICODE 
					str_file_path = file_path.wstring();//fs::path -> std::wstring 으로 변환
					extension = file_path.extension();
#else
					str_file_path = file_path.string();//fs::path -> std::string 으로 변환
					extension = file_path.extension().string();
#endif
					extension = StringEditor::ToUpper(extension);
					if (extension.size() < 1) { //확장자의 사이즈가 1 미만이라면 continue;
						continue;
					}
					if (extension.size() > _extentionVolume) {
						TCHAR chKey = extension[1];
						if (_chmapExceptionList.find(chKey) == _chmapExceptionList.end()) {
							vec_files.emplace_back(str_file_path);
						}
						else {
							auto vecExceptionList = _chmapExceptionList.at(chKey);
							auto iter = std::find(vecExceptionList.begin(), vecExceptionList.end(), extension);
							if (iter == vecExceptionList.end()) { //예외 목록에 포함된 확장자가 아니라면 vec_files에 담는다.
								vec_files.emplace_back(str_file_path);
							}
						}
					}
				}
			}
			catch (const fs::filesystem_error& _fileError) {
				iter_temp.pop();
				iter_fs = iter_temp;
			}
		}
		vec_files.shrink_to_fit();
		CLogger::GetInstance()->LogEnd(_T(" GetRecursiveFilesInDirectory2()"));



		return vec_files;
	}

	// 파일 확장자 가져오기
	static TString GetExtention(const TString& _path) {
		fs::path path = _path;
#if UNICODE 
		TString extension = path.extension().wstring();
#else
		TString extension = path.extension().string();
#endif
		return extension;
	}
	//확장자 목록 txt 파일에 작성
	static void WriteExtensionList(const TString& _rootPath, const TString& _filePath, int _extentionVolume) {

		fs::directory_iterator iter;
		try {
			iter = fs::directory_iterator(_rootPath);
		}
		catch (std::exception _e) {
#if UNICODE 
			TCHAR* pStr = nullptr;
			int size = MultiByteToWideChar(CP_ACP, 0, _e.what(), -1, NULL, NULL);
			pStr = new TCHAR[size];
			MultiByteToWideChar(CP_ACP, 0, _e.what(), static_cast<int>(strlen(_e.what()) + 1), pStr, size);
			std::tcout << pStr << _T("\n: ") << _T("\n ") << std::endl;
			std::tcout << _e.what() << _T("\n: ") << _rootPath << _T("\n ") << std::endl;
#else
			std::tcout << _e.what() << _T("\n: ") << _rootPath << _T("\n ") << std::endl;
#endif;
			return;
		}
		TString str_file_path = _T("");
		while (true != iter._At_end()) {
			try {
				fs::path file_path = (*iter++).path();
#if UNICODE 
				str_file_path = file_path.wstring();//fs::path -> std::wstring 으로 변환
#else
				str_file_path = file_path.string();//fs::path -> std::string 으로 변환
#endif;
				if (fs::is_directory(file_path)) { //디렉토리인가?
					WriteExtensionList(str_file_path, _filePath, _extentionVolume);
				}
				else { //디렉토리가 아닌 파일인 경우
					auto size = file_path.extension().string().size();
					if (size > _extentionVolume) {

						WriteData(_filePath, GetExtention(str_file_path));
					}
				}
			}
			catch (std::exception _e) {
#if UNICODE 
				TCHAR* pStr = nullptr;
				int size = MultiByteToWideChar(CP_ACP, 0, _e.what(), -1, NULL, NULL);
				pStr = new TCHAR[size];
				MultiByteToWideChar(CP_ACP, 0, _e.what(), static_cast<int>(strlen(_e.what()) + 1), pStr, size);
				std::tcout << pStr << _T("\n: ") << str_file_path << _T("\n ") << std::endl;
#else
				std::tcout << _e.what() << _T("\n: ") << str_file_path << _T("\n ") << std::endl;
#endif;
			}

		}
	}
	//파일에 쓰기
	static void WriteData(const TString& _filePath, const TString& _saveData) {
		tofstream fout; //추출한 문자열 저장할 텍스트 파일	
		fout.open(_filePath, std::ios_base::out | std::ios_base::app | std::ios_base::binary);
		fout << _saveData << std::endl;
		fout.close();
	}

	//파일 읽기
	static TString ReadData(const TString& _filePath) {
		TString str;
		//TCHAR ch[300];
		StringBuilder strBuil;
		tifstream fin;

		std::locale::global(std::locale(".UTF-8"));
		fin.open(_filePath, std::ios_base::binary | std::ios_base::in);
		while (std::getline(fin, str)) {
			strBuil.Append_endl(str);
		}
		fin.close();
		return strBuil.str();
	}
	static std::string ReadData_ANSI2UNI(const TString& _filePath) {
		std::string str;
		std::string txt;
		std::ifstream fin;
		fin.open(_filePath, std::ios_base::binary | std::ios_base::in);
		while (std::getline(fin, str)) {
			txt.append(str);
			txt.append("\n");
		}

		return txt;
	}
	static std::wstring ReadData_ANSI2UNI_2(const TString& _filePath) {
		std::string str;
		std::string txt;
		std::ifstream fin;
		fin.open(_filePath, std::ios_base::binary | std::ios_base::in);
		while (std::getline(fin, str)) {
			txt.append(str);
			txt.append("\n");
		}
		USES_CONVERSION;
		std::wstring wstr(A2W(txt.c_str()));
		return wstr;
	}

	static TString GetFileName(const TString& _filePath) {
		auto fileName = _filePath.substr(_filePath.find_last_of(_T("\\")) + 1, _filePath.size() - _filePath.find(_T("\\")));
		fileName = fileName.substr(0, fileName.find_last_of(_T(".")) );
		return fileName;
	}
	
};