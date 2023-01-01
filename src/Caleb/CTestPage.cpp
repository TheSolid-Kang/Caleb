#include "CTestPage.h"
#include "CFileMgr.hpp"

CTestPage::CTestPage() 
{
}

CTestPage::~CTestPage()
{
}

void CTestPage::initialize(void)
{
	auto arr_note = build_array(_T("1. 선택한 폴더 내 모든 파일 출력")
		, _T("2. 가계부 선택 및 내용 parsing")
		, _T("3. ")
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
				std::tcout << pair_note.first << _T("\n") << pair_note.second;

			return nullptr; }));
	(*m_uniq_map_func).emplace(std::make_pair(static_cast<size_t>(FUNC::THREE)
		, [&](const void* _p_void) -> std::shared_ptr<void> {return nullptr; }));
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
		, [&](const void* _p_void) -> std::shared_ptr<void> {return nullptr; }));
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
