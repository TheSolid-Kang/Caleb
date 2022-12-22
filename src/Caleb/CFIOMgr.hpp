#pragma once
#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <filesystem>
#include <Windows.h>
#include <atlconv.h>

enum class FILE_TYPE : int {
	FT_TXT
	, FT_PNG
	, FT_JPG
	, FT_JPEG
	, FT_CSV
	, FT_MD
	, FT_END
};
class CFIOMgr
{
public:
	//1. Directory 내 파일 생성
	//2. Directory 내 파일 목록 출력
	//3. 파일 쓰기 
	//4. Directory 내 파일 읽기
	//5. Directory 내 파일 수정
private:
	static std::string GetFileType(int _file_type)
	{
		std::string str_type = "";
		switch (_file_type)
		{
		case static_cast<int>(FILE_TYPE::FT_TXT):
			str_type = ".TXT";
			break;
		case static_cast<int>(FILE_TYPE::FT_PNG):
			str_type = ".PNG";
			break;
		case static_cast<int>(FILE_TYPE::FT_JPG):
			str_type = ".JPG";
			break;
		case static_cast<int>(FILE_TYPE::FT_JPEG):
			str_type = ".JPEG";
			break;
		case static_cast<int>(FILE_TYPE::FT_CSV):
			str_type = ".CSV";
			break;
		case static_cast<int>(FILE_TYPE::FT_MD):
			str_type = ".MD";
			break;
		default:
			str_type = ".TXT";
			break;
		}

		return str_type;
	}

public:
#if UNICODE //unicode == ANSI == W
	//0. 실행파일 경로 구하기
	static std::string GetEXEFilePath() {
		std::wstring wstr_exe_path = _GetEXEFilePathW();
		std::string exe_path(wstr_exe_path.begin(), wstr_exe_path.end());
		return exe_path;
	}
	static std::wstring GetEXEFilePathW() {
		std::wstring wstr_exe_path = _GetEXEFilePathW();
		return wstr_exe_path;
	}
	
	//1-2. Directory 내 하위 폴더 차례대로 생성
	static void CreateDirectorys(std::string _path) {
		_CreateDirectorysW(_path);
	}
	//4. Directory 내 파일 읽기
	static std::vector<std::string> GetVecFileLines(std::string _path) {
		std::vector<std::wstring> vec_wline = _GetVecFileLinesW(_path);
		std::vector<std::string> vec_line;
		std::for_each(vec_wline.begin(), vec_wline.end(), [&vec_line](std::wstring& _wstr_line) {
			//1번
			//std::string str_line;
			//str_line.assign(_wstr_line.begin(), _wstr_line.end());
			//vec_line.emplace_back(str_line);

			//2번
			//vec_line.emplace_back(std::string(_wstr_line.begin(), _wstr_line.end()));

			//3번
			USES_CONVERSION;
			vec_line.emplace_back(W2A(_wstr_line.c_str()));
			});
		vec_line.shrink_to_fit();
		return vec_line;
		//return _GetVecFileLinesA(_path);
	}
	static std::vector<std::wstring> GetVecFileLinesW(std::string _path) {
		return _GetVecFileLinesW(_path);
	}

#else
	//0. 실행파일 경로 구하기
	static std::string GetEXEFilePath() {
		std::string str_exe_path = _GetEXEFilePathA();
		return str_exe_path;
	}
	//1-2. Directory 내 하위 폴더 차례대로 생성
	static void CreateDirectorys(std::string _path) {
		_CreateDirectorysA(_path);
	}
	//4. Directory 내 파일 읽기
	static std::vector<std::string> GetVecFileLines(std::string _path) {;
		return _GetVecFileLinesA(_path);
	}

#endif
	//1. Directory 내 파일 생성
	static void CreateNewFile(std::string _path, std::string _file_name, int _file_type = static_cast<int>(FILE_TYPE::FT_TXT)) {
		if (std::string::npos == _file_name.find("."))//npos == find 결과가 없는 경우
			_file_name += GetFileType(_file_type);
		std::string file_path = _path + _file_name;
		std::ofstream fout = std::ofstream(file_path);//파일 열기_만약 파일이 없으면 만듦.
		std::cout << "fout.is_open() == " << fout.is_open() << std::endl;
		if (true == fout.is_open())
			fout.close();
	}

	//2. Directory 내 파일 목록 출력
	static std::vector<std::string> GetFilesInDirectory(std::string _path) {
		std::vector<std::string> vec_files ;
		vec_files.reserve(1024);
		auto iter = std::filesystem::directory_iterator(_path);
		while (true != iter._At_end())
			vec_files.emplace_back((*iter++).path().string());//std::filesystem::path -> std::string 으로 변환
		vec_files.shrink_to_fit();
		return vec_files;
	}
	//3. 파일 쓰기 
	static void WriteText(std::string _path, std::string _text) {
		std::ofstream fout(_path, std::ios::binary);
		if (true == fout.is_open()) {
			fout << _text;
			fout.close();
		}
	}

	//5. Directory 내 파일 수정


private:
#if UNICODE //unicode == ANSI == W
	//1-2. Directory 내 하위 폴더 차례대로 생성
	static void _CreateDirectorysW(std::string _path) {
		std::wstring path(_path.begin(), _path.end());
		wchar_t arr_dir_name[256];
		wchar_t* ch_ptr_path = const_cast<wchar_t*>(path.c_str());
		wchar_t* ch_ptr_dirname = arr_dir_name;

		while (*ch_ptr_path) {
			if (('\\' == *ch_ptr_path) || ('/' == *ch_ptr_path)) {
				if (':' != *(ch_ptr_path - 1))
					::CreateDirectoryW(arr_dir_name, NULL);
			}
			*ch_ptr_dirname++ = *ch_ptr_path++;
			*ch_ptr_dirname = '\0';
		}
		::CreateDirectoryW(arr_dir_name, NULL);

	}
	static std::wstring _GetEXEFilePathW() {
		//1. 실행파일 경로 구하기 
		wchar_t path[MAX_PATH] = { 0, };
		GetModuleFileNameW(NULL, path, MAX_PATH);
		USES_CONVERSION;
		std::wstring exe_path = path;

		exe_path = exe_path.substr(0, exe_path.find_last_of(L"\\/"));

		return exe_path;
	}

#else
	static void _CreateDirectorysA(std::string _path) {
		char arr_dir_name[256];
		char* ch_ptr_path = const_cast<char*>(_path.c_str());
		char* ch_ptr_dirname = arr_dir_name;

		while (*ch_ptr_path) {
			if (('\\' == *ch_ptr_path) || ('/' == *ch_ptr_path)) {
				if (':' != *(ch_ptr_path - 1))
					CreateDirectoryA(arr_dir_name, NULL);
			}
			*ch_ptr_dirname++ = *ch_ptr_path++;
			*ch_ptr_dirname = '\0';
		}
		CreateDirectoryA(arr_dir_name, NULL);

	}
	static std::string _GetEXEFilePathA() {
		//1. 실행파일 경로 구하기 
		char path[MAX_PATH] = { 0, };
		GetModuleFileName(NULL, path, MAX_PATH);
		USES_CONVERSION;
		std::string exe_path = path;
		exe_path = exe_path.substr(0, exe_path.find_last_of("\\/"));

		return exe_path;
	}


#endif
	//4. Directory 내 파일 읽기
	static std::vector<std::wstring> _GetVecFileLinesW(std::string _path) {
		std::wstring line;
		std::vector<std::wstring> vec_line;
		vec_line.reserve(1024);
		std::wifstream fin(_path);//, std::ios::binary

		if (true == fin.is_open()) {
			while (std::getline(fin, line))
				vec_line.emplace_back(line);
			fin.close(); // 열었던 파일을 닫는다. 
		}
		else {
			std::cout << "파일 열기 실패" << std::endl;
		}
		vec_line.shrink_to_fit();
		return vec_line;
	}
	//4. Directory 내 파일 읽기
	static std::vector<std::string> _GetVecFileLinesA(std::string _path) {

		std::string line;
		std::vector<std::string> vec_line;
		vec_line.reserve(1024);
		std::ifstream fin(_path);//, std::ios::binary

		if (true == fin.is_open()) {
			while (std::getline(fin, line))
				vec_line.emplace_back(line);

			fin.close(); // 열었던 파일을 닫는다. 
		}
		else {
			std::cout << "파일 열기 실패" << std::endl;
		}
		vec_line.shrink_to_fit();
		return vec_line;
	}
};