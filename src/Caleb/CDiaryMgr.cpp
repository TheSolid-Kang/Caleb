#include "CDiaryMgr.h"
#include "CFIOMgr.hpp"
#include "CINIMgr.hpp"
#include "KMP.h"
#include "StringBuilder.h"
#include "CMyEtc.hpp"


CDiaryMgr::CDiaryMgr()
{
	initialize();
}
CDiaryMgr::~CDiaryMgr()
{
	
}

void CDiaryMgr::initialize(void)
{
	init_key();
}

void CDiaryMgr::init_key(void)
{
	//���� ��Ī �� ��������
	std::string diary_dir_path = CINIMgr::GetPrivateProfileString_INI("PATH", "DIARY_PATH");
	auto vec_file_name = CFIOMgr::GetFilesInDirectory(diary_dir_path);
	std::for_each(vec_file_name.rbegin(), vec_file_name.rend(), [&](auto& _file_name) {m_map_diary[_file_name]; });
}

bool CDiaryMgr::InitDiary(const std::string& _diary_file_path) noexcept
{
	std::string diary_dir_path = CINIMgr::GetPrivateProfileString_INI("PATH", "DIARY_PATH");
	if (std::string::npos  == _diary_file_path.find(diary_dir_path))
		const_cast<std::string&>(_diary_file_path).append("\\" + _diary_file_path);

	auto vec_line = CFIOMgr::GetVecFileLines(_diary_file_path);
	StringBuilder str_buil;
	std::for_each(vec_line.cbegin(), vec_line.cend(), [&str_buil](const std::string& _line) {str_buil.append_endl(_line); });

	m_map_diary[_diary_file_path] = str_buil.str();

	return true;
}

std::string CDiaryMgr::GetDiarySelectedSection(const std::string& _diary_file_path, const std::string& _section)
{
	//0. 
	std::string& diary = GetDiary(_diary_file_path);
	if (diary == "No files.")
		return "������ �����ϴ�.";

	//1. Section �� �ʱ�ȭ
	std::string section_begin = "<--" + _section + "-->";
	std::string section_last = "<--End " + _section + "-->";

	//2. ���ڿ� �� index �ʱ�ȭ 
	auto index_first = diary.find(section_begin);
	auto index_last = diary.rfind(section_last);
	if (index_first == std::string::npos || index_last == std::string::npos)
	{
		//throw new std::exception("record�� �������� �ʴ� section�Դϴ�.");// find ���� �� �� ���ڿ��� �ִٸ� �Լ� ����
		//�ش� ���Ͽ� SECTION ����
		auto vec_section = GetVecSection();
		auto iter = std::find(vec_section.cbegin(), vec_section.cend(), _section);
		if (iter == vec_section.cend())//ini�� �ش� section�� ���ٸ� return;
			return "";

		//ini�� �ش� sectino�� �ִٸ� ������ ������ �ٿ� �ش� section�� ���� �� return.
		m_map_diary[_diary_file_path].append(section_begin);
		m_map_diary[_diary_file_path].append("\n");
		m_map_diary[_diary_file_path].append(section_last);
		m_map_diary[_diary_file_path].append("\n");
		m_map_diary[_diary_file_path].append("\n");
		m_map_diary[_diary_file_path].append("\n");
		CFIOMgr::WriteText(_diary_file_path, m_map_diary[_diary_file_path]);

		return "";
	}
	index_first += section_begin.length();
	index_last -= index_first;

	//3. ���ڿ��� index ��ŭ �ڸ� �� ��ȯ
	return diary.substr(index_first, index_last);

}

std::vector<std::string> CDiaryMgr::GetVecSection(void) noexcept
{
	std::string str_ARR_DIARY_SECTION = CINIMgr::GetPrivateProfileString_INI("SECTION", "ARR_DIARY_SECTION");
	return CMyEtc::Split(str_ARR_DIARY_SECTION, '|');
}

std::string& CDiaryMgr::GetDiary(const std::string& _diary_file_path)
{
	std::string result = "";
	std::string diary_dir_path = CINIMgr::GetPrivateProfileString_INI("PATH", "DIARY_PATH");
	if (std::string::npos == _diary_file_path.find(diary_dir_path))
		const_cast<std::string&>(_diary_file_path).append("\\" + _diary_file_path);

	//1. ��� �� _diary_name�� ��ġ�ϴ� ������ �ִ��� Ȯ��
	auto vec_file_path = CFIOMgr::GetFilesInDirectory(diary_dir_path);
	std::vector<std::string>::const_iterator citer = std::find(vec_file_path.cbegin(), vec_file_path.cend(), _diary_file_path);
	if (citer == vec_file_path.cend())
	{
		result = "No files.";
		return result;
	}
	//2. �ش� key�� value�� �ʱ�ȭ �Ǿ����� Ȯ��
	if (m_map_diary[_diary_file_path]._Equal(""))
		InitDiary(_diary_file_path);

	//3. diary_name�� ��ġ�ϴ� ���Ͽ��� ������ ��������
	return m_map_diary[_diary_file_path];
}

std::map<std::string, int> CDiaryMgr::GetMapWordCount(const std::string& _diary_file_path, const std::string& _section)
{
	std::string section_record = GetDiarySelectedSection(_diary_file_path, _section);
	//praise ���ǿ��� ��� �ܾ� �˻�
#if UNICODE //unicode == ANSI == W
	std::wstring wstr_arr_keyword = CINIMgr::GetPrivateProfileStringW_INI("SEARCH", "ARR_KEYWORD");
	USES_CONVERSION;
	auto vec_keyword = CMyEtc::Split(W2A(wstr_arr_keyword.c_str()), '|');
#else
	std::string str_arr_keyword = CINIMgr::GetPrivateProfileString_INI("SEARCH", "ARR_KEYWORD");
	auto vec_keyword = CMyEtc::Split(str_arr_keyword, '|');
#endif

	std::map<std::string, int> map_wordcount;
	std::for_each(vec_keyword.cbegin(), vec_keyword.cend()
		, [&](const std::string& _keyword) {
			map_wordcount[_keyword] = (int)KMP::GetSearchedAddress(section_record, _keyword)->size();
		});


	return map_wordcount;
}

