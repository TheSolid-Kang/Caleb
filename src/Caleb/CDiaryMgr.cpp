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
	TString diary_dir_path = CINIMgr::_GetPrivateProfileString_INI(_T("PATH"), _T("DIARY_PATH"));
	std::vector<TString> vec_file_name = CFIOMgr::_GetFilesInDirectory(diary_dir_path);
	std::for_each(vec_file_name.rbegin(), vec_file_name.rend(), [&](auto& _file_name) {m_map_diary[_file_name]; });
}

bool CDiaryMgr::InitDiary(const TString& _diary_file_path) noexcept
{
	TString diary_dir_path = CINIMgr::_GetPrivateProfileString_INI(_T("PATH"), _T("DIARY_PATH"));
	if (TString::npos  == _diary_file_path.find(diary_dir_path))
		const_cast<TString&>(_diary_file_path).append(_T("\\") + _diary_file_path);

	auto vec_line = CFIOMgr::_GetVecFileLines(_diary_file_path);
	StringBuilder str_buil;
	std::for_each(vec_line.cbegin(), vec_line.cend(), [&str_buil](const TString& _line) {str_buil.append_endl(_line); });

	m_map_diary[_diary_file_path] = str_buil.str();

	return true;
}

TString CDiaryMgr::GetDiarySelectedSection(const TString& _diary_file_path, const TString& _section)
{
	//0. 
	TString& diary = GetDiary(_diary_file_path);
	if (diary == _T("No files."))
		return _T("������ �����ϴ�.");

	//1. Section �� �ʱ�ȭ
	TString section_begin = _T("<--") + _section + _T("-->");
	TString section_last = _T("<--End ") + _section + _T("-->");

	//2. ���ڿ� �� index �ʱ�ȭ 
	auto index_first = diary.find(section_begin);
	auto index_last = diary.rfind(section_last);
	if (index_first == TString::npos || index_last == TString::npos)
	{
		//throw new std::exception("record�� �������� �ʴ� section�Դϴ�.");// find ���� �� �� ���ڿ��� �ִٸ� �Լ� ����
		//�ش� ���Ͽ� SECTION ����
		auto vec_section = GetVecSection();
		auto iter = std::find(vec_section.cbegin(), vec_section.cend(), _section);
		if (iter == vec_section.cend())//ini�� �ش� section�� ���ٸ� return;
			return _T("");

		//ini�� �ش� sectino�� �ִٸ� ������ ������ �ٿ� �ش� section�� ���� �� return.
		m_map_diary[_diary_file_path].append(section_begin);
		m_map_diary[_diary_file_path].append(_T("\n"));
		m_map_diary[_diary_file_path].append(section_last);
		m_map_diary[_diary_file_path].append(_T("\n"));
		m_map_diary[_diary_file_path].append(_T("\n"));
		m_map_diary[_diary_file_path].append(_T("\n"));
		CFIOMgr::_WriteText(_diary_file_path, m_map_diary[_diary_file_path]);

		return _T("");
	}
	index_first += section_begin.length();
	index_last -= index_first;

	//3. ���ڿ��� index ��ŭ �ڸ� �� ��ȯ
	return diary.substr(index_first, index_last);

}

std::vector<TString> CDiaryMgr::GetVecSection(void) noexcept
{
	TString str_ARR_DIARY_SECTION = CINIMgr::_GetPrivateProfileString_INI(_T("SECTION"), _T("ARR_DIARY_SECTION"));
	return CMyEtc::Split(str_ARR_DIARY_SECTION, _T('|'));
}

TString& CDiaryMgr::GetDiary(const TString& _diary_file_path)
{
	TString result = _T("");
	TString diary_dir_path = CINIMgr::_GetPrivateProfileString_INI(_T("PATH"), _T("DIARY_PATH"));
	if (TString::npos == _diary_file_path.find(diary_dir_path))
		const_cast<TString&>(_diary_file_path).append(_T("\\") + _diary_file_path);

	//1. ��� �� _diary_name�� ��ġ�ϴ� ������ �ִ��� Ȯ��
	auto vec_file_path = CFIOMgr::_GetFilesInDirectory(diary_dir_path);
	std::vector<TString>::const_iterator citer = std::find(vec_file_path.cbegin(), vec_file_path.cend(), _diary_file_path);
	if (citer == vec_file_path.cend())
	{
		result = _T("No files.");
		return result;
	}
	//2. �ش� key�� value�� �ʱ�ȭ �Ǿ����� Ȯ��
	if (m_map_diary[_diary_file_path]._Equal(_T("")))
		InitDiary(_diary_file_path);

	//3. diary_name�� ��ġ�ϴ� ���Ͽ��� ������ ��������
	return m_map_diary[_diary_file_path];
}

std::map<TString, int> CDiaryMgr::GetMapWordCount(const TString& _diary_file_path, const TString& _section)
{
	TString section_record = GetDiarySelectedSection(_diary_file_path, _section);
	//praise ���ǿ��� ��� �ܾ� �˻�
	TString wstr_arr_keyword = CINIMgr::_GetPrivateProfileString_INI(_T("SEARCH"), _T("ARR_KEYWORD"));
	auto vec_keyword = CMyEtc::Split(wstr_arr_keyword, _T('|'));

	std::map<TString, int> map_wordcount;
	std::for_each(vec_keyword.cbegin(), vec_keyword.cend()
		, [&](const TString& _keyword) {
			map_wordcount[_keyword] = (int)KMP::GetSearchedAddress(section_record, _keyword)->size();
		});


	return map_wordcount;
}

