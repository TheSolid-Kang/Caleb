#include "CUpdatePage.h"

CUpdatePage::CUpdatePage()
{
}

CUpdatePage::~CUpdatePage()
{

}

void CUpdatePage::initialize(void)
{
	auto arr_note = build_array(_T("1. ")
		, _T("2. ")
		, _T("3. ")
		, _T("")
		, _T("99. EXIT"));
	m_list_title.insert(m_list_title.end(), arr_note.begin(), arr_note.end());
}

void CUpdatePage::init_func(void)
{
}

void CUpdatePage::init_selected_func(void)
{
}

void CUpdatePage::render(void)
{
	(*m_uniq_map_selected_func)[static_cast<size_t>(COMMON_SELECTED_FUNC::PRINT_TITLE)](nullptr);
}

int CUpdatePage::update(int _event)
{
	return 0;
}

void CUpdatePage::release(void)
{
}
