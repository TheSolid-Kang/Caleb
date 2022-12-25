#pragma once
#include <fstream>
#include <iostream>
#include <string>
#include <tchar.h>
#include <vector>
#include <filesystem>
#include <Windows.h>
#include <atlconv.h>
#if UNICODE 
using TString = std::wstring;
using tifstream = std::wifstream;
using tofstream = std::wofstream;
#define tcout  wcout
#define tcin  wcin
#else
using TString = std::string;
using tifstream = std::ifstream;
using tofstream = std::ofstream;
#define tcout  cout
#define tcin  wcin
#endif;



#define DEF_CAP 2048
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
	//1. Directory 내 파일 생성
//2. Directory 내 파일 목록 출력
//3. 파일 쓰기 
//4. Directory 내 파일 읽기
//5. Directory 내 파일 수정
public:
	static TString _GetFileType(int _file_type)
	{
		TString str_type = _T("");
		switch (_file_type)
		{
		case static_cast<int>(FILE_TYPE::FT_TXT):
			str_type = _T(".TXT");
			break;
		case static_cast<int>(FILE_TYPE::FT_PNG):
			str_type = _T(".PNG");
			break;
		case static_cast<int>(FILE_TYPE::FT_JPG):
			str_type = _T(".JPG");
			break;
		case static_cast<int>(FILE_TYPE::FT_JPEG):
			str_type = _T(".JPEG");
			break;
		case static_cast<int>(FILE_TYPE::FT_CSV):
			str_type = _T(".CSV");
			break;
		case static_cast<int>(FILE_TYPE::FT_MD):
			str_type = _T(".MD");
			break;
		default:
			str_type = _T(".TXT");
			break;
		}

		return str_type;
	}


	//0. 실행파일 경로 구하기
	static TString _GetEXEFilePath() {
		//1. �������� ��� ���ϱ� 
		TCHAR path[MAX_PATH] = { 0, };
		GetModuleFileName(NULL, path, MAX_PATH);
		TString exe_path = path;
		exe_path = exe_path.substr(0, exe_path.find_last_of(_T("\\/")));
		return exe_path;
	}

	//1-2. Directory 내 하위 폴더 차례대로 생성
	static void _CreateDirectorys(const TString& _path) {
		TString path(_path.begin(), _path.end());
		TCHAR arr_dir_name[256];
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

	//1. Directory �� ���� ����
	static void _CreateNewFile(TString _path, TString _file_name, int _file_type = static_cast<int>(FILE_TYPE::FT_TXT)) {
		if (std::string::npos == _file_name.find(_T(".")))//npos == find 결과가 없는 경우
			_file_name += _GetFileType(_file_type);
		TString file_path = _path + _file_name;
		tofstream fout = tofstream(file_path);//파일 열기_만약 파일이 없으면 만듦.

		std::tcout << _T("fout.is_open() == ") << fout.is_open() << std::endl;
		if (true == fout.is_open())
			fout.close();
	}

	//2. Directory 내 파일 목록 출력
	static std::vector<TString> _GetFilesInDirectory(TString& _path) {
		std::vector<TString> vec_files;
		vec_files.reserve(1024);
		auto iter = std::filesystem::directory_iterator(_path);
		while (true != iter._At_end()) {
#if UNICODE 
			vec_files.emplace_back((*iter++).path().wstring());//std::filesystem::path -> std::string 으로 변환
#else
			vec_files.emplace_back((*iter++).path().string());//std::filesystem::path -> std::string 으로 변환
#endif;
		}
		vec_files.shrink_to_fit();
		return vec_files;
	}
	//3. 파일 쓰기 
	static void _WriteText(const TString& _path, const TString& _text) {
		tofstream fout = tofstream(_path, std::ios::binary);//파일 열기_만약 파일이 없으면 만듦.

		if (true == fout.is_open()) {
			fout << _text;
			fout.close();
		}
	}
	//4. Directory 내 파일 읽기
	static std::vector<TString> _GetVecFileLines(const TString& _path) {
		TString line;
		std::vector<TString> vec_line;
		vec_line.reserve(1024);

		tifstream fin(_path);//, std::ios::binary

		if (true == fin.is_open()) {
			while (std::getline(fin, line))
				vec_line.emplace_back(line);
			fin.close(); // 열었던 파일을 닫는다. 
		}
		else {
			std::tcout << _T("파일 열기 실패") << std::endl;
		}
		vec_line.shrink_to_fit();
		return vec_line;
	}
	//5. Directory 내 파일 수정

};