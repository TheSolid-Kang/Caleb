#include "CTestPage.h"
#include "CFileMgr.hpp"

CTestPage::CTestPage() 
	: m_uniq_map_func(std::make_unique<std::map<size_t, std::function<std::shared_ptr<void>(const void*)>>>())
	, m_uniq_map_testfunc(std::make_unique<std::map<size_t, std::function<std::shared_ptr<void>(const void*)>>>())
{
}

CTestPage::~CTestPage()
{
}

void CTestPage::initialize(void)
{
	auto arr_note = build_array(_T("1. 선택한 폴더 내 모든 파일 출력")
		, _T("2. ")
		, _T("3. ")
		, _T("")
		, _T("99. EXIT"));
	m_list_note.insert(m_list_note.end(), arr_note.begin(), arr_note.end());
	init_func();
	init_testfunc();
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
		, [&](const void* _p_void) -> std::shared_ptr<void> {return nullptr; }));
	(*m_uniq_map_func).emplace(std::make_pair(static_cast<size_t>(FUNC::THREE)
		, [&](const void* _p_void) -> std::shared_ptr<void> {return nullptr; }));
	(*m_uniq_map_func).emplace(std::make_pair(static_cast<size_t>(FUNC::FOUR)
		, [&](const void* _p_void) -> std::shared_ptr<void> {return nullptr; }));
	(*m_uniq_map_func).emplace(std::make_pair(static_cast<size_t>(FUNC::FIVE)
		, [&](const void* _p_void) -> std::shared_ptr<void> {return nullptr; }));
}

void CTestPage::init_testfunc(void)
{
	(*m_uniq_map_testfunc).emplace(std::make_pair(static_cast<size_t>(TEST_FUNC::ONE)
		, [&](const void* _p_void) -> std::shared_ptr<void> {
			(*m_uniq_map_func)[static_cast<size_t>(FUNC::ONE)](nullptr);
			return nullptr; }));
	(*m_uniq_map_testfunc).emplace(std::make_pair(static_cast<size_t>(TEST_FUNC::TWO)
		, [&](const void* _p_void) -> std::shared_ptr<void> {return nullptr; }));
	(*m_uniq_map_testfunc).emplace(std::make_pair(static_cast<size_t>(TEST_FUNC::THREE)
		, [&](const void* _p_void) -> std::shared_ptr<void> {return nullptr; }));
	(*m_uniq_map_testfunc).emplace(std::make_pair(static_cast<size_t>(TEST_FUNC::FOUR)
		, [&](const void* _p_void) -> std::shared_ptr<void> {return nullptr; }));
	(*m_uniq_map_testfunc).emplace(std::make_pair(static_cast<size_t>(TEST_FUNC::FIVE)
		, [&](const void* _p_void) -> std::shared_ptr<void> {return nullptr; }));

}

void CTestPage::render(void)
{
	std::for_each(m_list_note.cbegin(), m_list_note.cend(), [](auto _note) {std::tcout << _note << std::endl; });
}

int CTestPage::update(int _event)
{
	int result;

	switch (CIO::ask_and_return_integer())
	{
	case static_cast<size_t>(TEST_FUNC::ONE):
		(*m_uniq_map_testfunc)[static_cast<size_t>(TEST_FUNC::ONE)](nullptr);
		break;
	case static_cast<size_t>(TEST_FUNC::TWO):
		(*m_uniq_map_testfunc)[static_cast<size_t>(TEST_FUNC::TWO)](nullptr);
		break;
	case static_cast<size_t>(TEST_FUNC::THREE):
		(*m_uniq_map_testfunc)[static_cast<size_t>(TEST_FUNC::THREE)](nullptr);
		break;
	case static_cast<size_t>(TEST_FUNC::FOUR):
		(*m_uniq_map_testfunc)[static_cast<size_t>(TEST_FUNC::FOUR)](nullptr);
		break;
	case static_cast<size_t>(TEST_FUNC::FIVE):
		(*m_uniq_map_testfunc)[static_cast<size_t>(TEST_FUNC::FIVE)](nullptr);
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
