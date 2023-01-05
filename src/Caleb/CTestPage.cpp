#include "CTestPage.h"
#include "CFileMgr.hpp"
#include "KMP.h"
#include "CMyEtc.hpp"

CTestPage::CTestPage() 
{
}

CTestPage::~CTestPage()
{
}

void CTestPage::initialize(void)
{
	auto arr_note = build_array(_T("1. 선택한 폴더 내 모든 파일명 출력")
		, _T("2. 원하는 섹션 보기")
		, _T("3. 전체 Caleb 읽기")
		, _T("")
		, _T("")
		, _T("")
		, _T("99. EXIT"));
	m_list_title.insert(m_list_title.end(), arr_note.begin(), arr_note.end());
}


void CTestPage::init_func(void)
{
	(*m_uniq_map_func).emplace(std::make_pair(static_cast<size_t>(FUNC::ONE)
		, [&](const void* _p_void) -> std::shared_ptr<void> {
			auto dir_path = CFileMgr::GetOpenFolderDialg(); //폴더 선택 및 출력경로 반환
			auto vec_file = CFIOMgr::GetRecursiveFilesInDirectory(dir_path); //폴더 내 모든 파일명 vector 컨테이너에 담기
			std::for_each(vec_file.cbegin(), vec_file.cend(), [](const TString& _file_path) {std::tcout << _file_path << std::endl; });//출력
			return nullptr; }));
	(*m_uniq_map_func).emplace(std::make_pair(static_cast<size_t>(FUNC::TWO)
		, [&](const void* _p_void) -> std::shared_ptr<void> {
			//1. 파일명 목록 컨테이너 생성
			auto dir_path = CINIMgr::GetPrivateProfileString_INI(_T("PATH"), _T("DIARY_PATH"));
		  auto vec_file_name = CFIOMgr::GetFilesInDirectory(dir_path);

			//2. 검색된 파일 하나씩 열어보며 diary의 TheWords에 내용이 있는지 확인
			//  2-1. 내용이 있다면 map에 저장
			std::map<TString, TString> map_note;
			TString section = _T("TheWords");
			//section = _T("Record");
			//section = _T("Mission");
			//section = _T("Fellowship");
			//section = _T("Praise");
			std::for_each(vec_file_name.cbegin(), vec_file_name.cend()
				, [&map_note, section](const TString& _file_path) {
				auto note = CDiaryMgr::GetInstance().GetDiarySelectedSection(_file_path, section);

				if (true == note._Equal(_T("\n")))
					return;
				else if (true == note._Equal(_T("\n\n")))
					return;
				else if (true == note._Equal(_T("\n\n\n")))
					return;

				map_note.emplace(std::make_pair(_file_path, note));
				});

			for (std::pair<TString, TString> pair_note : map_note)
				//std::tcout << pair_note.first << _T("\n") << pair_note.second;
				std::tcout << pair_note.first << _T("\n");

			return nullptr; }));
	(*m_uniq_map_func).emplace(std::make_pair(static_cast<size_t>(FUNC::THREE)
		, [&](const void* _p_void) -> std::shared_ptr<void> {

			//0. 변수 초기화
			//TString caleb_root_path = CFileMgr::GetOpenFolderDialg(); //잘 됨.
			TString caleb_root_path = _T("C:\\Caleb\\CalebRecord");
			//  vector 컨테이너에 CalebRecord에 담긴 파일 path 입력
			auto vec_caleb_path = CFIOMgr::GetFilesInDirectory(caleb_root_path);
			std::map<TString, std::map<TString, int>> map_cnt;

			//1. 검색어 설정
			//auto search_key = CIO::ask_and_return_string(); //잘 안 됨.
			std::list<TString> list_search_key;
			

			list_search_key.emplace_back(_T("은아"));
			list_search_key.emplace_back(_T("결혼"));
			list_search_key.emplace_back(_T("사랑"));
			list_search_key.emplace_back(_T("절망"));

			TString wstr_arr_keyword = CINIMgr::GetPrivateProfileString_INI(_T("SEARCH"), _T("ARR_KEYWORD"));
			auto vec_keyword = CMyEtc::Split(wstr_arr_keyword, _T('|'));
			for (const TString& _search_key : vec_keyword)
				list_search_key.emplace_back(_search_key);

			//2. Caleb 내에서 검색 횟수 확인
			//  1) 컨테이너에서 caleb_path 선택
			for (const TString& _caleb_path : vec_caleb_path)
			{
				//  2) caleb.txt 파일에 담긴 모든 내용 입력
				TString caleb = CFIOMgr::GetFileLines(_caleb_path);
				for (TString _search_key : list_search_key)
				{
					//  3) 한 Caleb에서 검색어 별 언급횟수 확인 및 입력
					auto uniq_vec_address = KMP::GetSearchedAddress(caleb, _search_key);
					if ((*uniq_vec_address).size() != 0)
						map_cnt[_caleb_path][_search_key] += (*uniq_vec_address).size();
				}
			}
			//3. Console창에 각각의 Caleb에서 검색어 별 언급횟수 출력
			for (std::pair<TString, std::map<TString, int>> _pair_cnt : map_cnt)
			{
				std::tcout << _T("=====") << _pair_cnt.first << _T("=====") << std::endl;
				for (std::pair<TString, int> _pair : _pair_cnt.second)
					std::tcout << _T("    ") << _pair.first << _T(" == ") << _pair.second << std::endl;
			}

			return nullptr; }));
	(*m_uniq_map_func).emplace(std::make_pair(static_cast<size_t>(FUNC::FOUR)
		, [&](const void* _p_void) -> std::shared_ptr<void> {return nullptr; }));
	(*m_uniq_map_func).emplace(std::make_pair(static_cast<size_t>(FUNC::FIVE)
		, [&](const void* _p_void) -> std::shared_ptr<void> {return nullptr; }));
}

void CTestPage::init_selected_func(void)
{
	(*m_uniq_map_selected_func).emplace(std::make_pair(static_cast<size_t>(SELECTED_FUNC::ONE)
		, [&](const void* _p_void) -> std::shared_ptr<void> {
			(*m_uniq_map_func)[static_cast<size_t>(FUNC::ONE)](nullptr);
	return nullptr; }));
	(*m_uniq_map_selected_func).emplace(std::make_pair(static_cast<size_t>(SELECTED_FUNC::TWO)
		, [&](const void* _p_void) -> std::shared_ptr<void> {
			(*m_uniq_map_func)[static_cast<size_t>(FUNC::TWO)](nullptr);
			return nullptr; }));
	(*m_uniq_map_selected_func).emplace(std::make_pair(static_cast<size_t>(SELECTED_FUNC::THREE)
		, [&](const void* _p_void) -> std::shared_ptr<void> {
			(*m_uniq_map_func)[static_cast<size_t>(FUNC::THREE)](nullptr);
			return nullptr; }));
	(*m_uniq_map_selected_func).emplace(std::make_pair(static_cast<size_t>(SELECTED_FUNC::FOUR)
		, [&](const void* _p_void) -> std::shared_ptr<void> {return nullptr; }));
	(*m_uniq_map_selected_func).emplace(std::make_pair(static_cast<size_t>(SELECTED_FUNC::FIVE)
		, [&](const void* _p_void) -> std::shared_ptr<void> {return nullptr; }));
}

void CTestPage::render(void)
{
	(*m_uniq_map_selected_func)[static_cast<size_t>(COMMON_SELECTED_FUNC::PRINT_TITLE)](nullptr);
}

int CTestPage::update(int _event)
{
	int result;

	switch (CIO::ask_and_return_integer())
	{
	case static_cast<size_t>(SELECTED_FUNC::ONE):
		(*m_uniq_map_selected_func)[static_cast<size_t>(SELECTED_FUNC::ONE)](nullptr);
		break;
	case static_cast<size_t>(SELECTED_FUNC::TWO):
		(*m_uniq_map_selected_func)[static_cast<size_t>(SELECTED_FUNC::TWO)](nullptr);
		break;
	case static_cast<size_t>(SELECTED_FUNC::THREE):
		(*m_uniq_map_selected_func)[static_cast<size_t>(SELECTED_FUNC::THREE)](nullptr);
		break;
	case static_cast<size_t>(SELECTED_FUNC::FOUR):
		(*m_uniq_map_selected_func)[static_cast<size_t>(SELECTED_FUNC::FOUR)](nullptr);
		break;
	case static_cast<size_t>(SELECTED_FUNC::FIVE):
		(*m_uniq_map_selected_func)[static_cast<size_t>(SELECTED_FUNC::FIVE)](nullptr);
		break;

	default:
		return m_r_page_mgr.SetPage(static_cast<int>(PAGES::P_SELECT));
		break;
	}
	result = m_r_page_mgr.SetPage(static_cast<int>(PAGES::P_TEST));
   return result;
}

void CTestPage::release(void)
{
}
