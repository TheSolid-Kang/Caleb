#include "CSelectPage.h"

CSelectPage::CSelectPage()
{
}

CSelectPage::~CSelectPage()
{
}

void CSelectPage::initialize(void)
{
  auto arr_page = build_array(_T("1. Insert Page")
    , _T("2. Record Page")
    , _T("3. Update Page")
    , _T("4. 미완_DELETE Page")
    , _T("5. Setting Page")
    , _T("99.EXIT"));
  m_list_page.insert(m_list_page.end(), arr_page.begin(), arr_page.end());
}

void CSelectPage::render(void)
{
  std::for_each(m_list_page.cbegin(), m_list_page.cend(), [](auto _ch_page) {std::tcout << _ch_page << std::endl; });
}

int CSelectPage::update(int _event = 0)
{
  int result;
  int selected_page = CIO::ask_and_return_integer();

  if (selected_page == EXIT)
    result = EXIT;
  else if ((selected_page >= static_cast<int>(PAGES::P_INSERT) && selected_page < static_cast<int>(PAGES::P_END)))
    result = m_r_page_mgr.SetPage(selected_page);
  else
    result = m_r_page_mgr.SetPage(static_cast<int>(PAGES::P_SELECT));

  return result;
}

void CSelectPage::release(void)
{
}