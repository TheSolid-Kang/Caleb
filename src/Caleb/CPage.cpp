#include "CPage.h"
#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>
#include <stdlib.h>
/*#if _DEBUG
#define new new(_NORMAL_BLOCK, __FILE__, __LINE__)
#define malloc(s) _malloc_dbg(s, _NORMAL_BLOCK, __FILE__, __LINE__)
#endif
*/
#ifdef _DEBUG
#define DBG_NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ )
// Replace _NORMAL_BLOCK with _CLIENT_BLOCK if you want the
// allocations to be of _CLIENT_BLOCK type
#else
#define DBG_NEW new
#endif
CPage::CPage()
	: m_r_page_mgr(CPageMgr::GetInstance())
	, m_uniq_map_func(std::make_unique<std::map<size_t, std::function<std::shared_ptr<void>(const void*)>>>())
	, m_uniq_map_selected_func(std::make_unique<std::map<size_t, std::function<std::shared_ptr<void>(const void*)>>>())
{
	(*this).initialize();
}

CPage::~CPage()
{

}

void CPage::initialize(void)
{
	(*this).init_func();
	(*this).init_selected_func();
}

void CPage::render(void)
{

}

void CPage::render(const void* _p_void)
{
}

void CPage::render(TString& _str)
{
	std::tcout << _str << std::endl;
}

int CPage::update(int _event = 0)
{
	return 99;
}

void CPage::release(void)
{
}

int CPage::Execute(int _event = 0)

{

	system("pause");
	system("cls");

	initialize();
	init_func();
	init_selected_func();

	render();

	int Update = update(_event);
	_CrtDumpMemoryLeaks();
	_CrtSetBreakAlloc(967);

	return Update;
}

void CPage::init_func(void)
{
	(*m_uniq_map_func).emplace(std::make_pair(static_cast<size_t>(COMMON_FUNC::PRINT_TITLE),
		[&](const void* _p_void) -> std::shared_ptr<void> {
			StringBuilder str_buil;
			std::for_each(m_list_title.cbegin(), m_list_title.cend(), [&str_buil](const TString& _ch_page) { str_buil.append_endl(_ch_page); });

			TString title = str_buil.str();
			auto shar_title = std::make_shared<TString>(title);
			return shar_title;
		}));

}


void CPage::init_selected_func(void)
{
	(*m_uniq_map_selected_func).emplace(std::make_pair(static_cast<size_t>(COMMON_SELECTED_FUNC::PRINT_TITLE),
		[&](const void* _p_void) -> std::shared_ptr<void> {
			auto shar_title = (*m_uniq_map_func)[static_cast<size_t>(COMMON_FUNC::PRINT_TITLE)](nullptr);
			auto& r_title = *(TString*)shar_title.get();//get 중요하다.
			CPage::render(r_title);
			return nullptr;
		}));
}
