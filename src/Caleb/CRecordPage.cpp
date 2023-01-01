#include "CRecordPage.h"
#include "KMP.h"
#include "CDiaryMgr.h"

CRecordPage::CRecordPage()
{
}

CRecordPage::~CRecordPage()
{
}

void CRecordPage::initialize(void)
{
  auto vec_section = CDiaryMgr::GetInstance().GetVecSection();
  int i = 0;
  m_list_title.insert(m_list_title.end(), vec_section.cbegin(), vec_section.cend());
  std::for_each(m_list_title.begin(), m_list_title.end(), [&i](TString& _note) { _note = std::to_tstring(i++) + _T(". ") + _note; });

  //m_list_title.emplace_front(typeid(*this).name());
  m_list_title.emplace_back(_T("99. EXIT"));
}

void CRecordPage::init_func(void)
{
}

void CRecordPage::init_selected_func(void)
{
}

void CRecordPage::init_note(void) noexcept
{
  if (0 != m_list_title.size())
    m_list_title.clear();
  auto dir_path = CINIMgr::GetPrivateProfileString_INI(_T("PATH"), _T("DIARY_PATH"));
  m_vec_file_name = CFIOMgr::GetFilesInDirectory(dir_path);
  std::reverse(m_vec_file_name.begin(), m_vec_file_name.end());

  int i = 0;
  std::for_each(m_vec_file_name.cbegin(), m_vec_file_name.cend(), [&](const TString& _file_name) {
    size_t index = _file_name.find_last_of(_T("\\")) + 1;
    m_list_title.emplace_back(std::to_tstring(++i) + _T(". ") + _file_name.substr(index, _file_name.size() - index));
    });
  m_list_title.emplace_back(std::to_tstring(m_vec_file_name.size() + 1) + _T(". EXIT"));
}

void CRecordPage::render(void)
{
  (*m_uniq_map_selected_func)[static_cast<size_t>(COMMON_SELECTED_FUNC::PRINT_TITLE)](nullptr);
}

int CRecordPage::update(int _event)
{
  int selectedDiarySection = CIO::ask_and_return_integer();
  auto vec_section = CDiaryMgr::GetInstance().GetVecSection();
  if(EXIT == selectedDiarySection)
    return m_r_page_mgr.SetPage(static_cast<int>(PAGES::P_SELECT));
  else if(0 > selectedDiarySection || selectedDiarySection > vec_section.size())
    return m_r_page_mgr.SetPage(static_cast<int>(PAGES::P_RECORD));

  m_strDiarySection = vec_section[selectedDiarySection];

  system("cls");

  init_note();
  render();
  int selected_diary = CIO::ask_and_return_integer();
  if (selected_diary > 0 && selected_diary < m_vec_file_name.size())
    PrintSelectedDairy(selected_diary);

  return m_r_page_mgr.SetPage(static_cast<int>(PAGES::P_SELECT));
}

void CRecordPage::release(void)
{

}

void CRecordPage::PrintSelectedDairy(int _selected_diary)
{
  system("cls");
  TString selected_file_path = m_vec_file_name[_selected_diary - 1];
  std::tcout << selected_file_path << std::endl;

  auto diary = CDiaryMgr::GetInstance().GetDiarySelectedSection(selected_file_path, m_strDiarySection);
  std::tcout << diary << std::endl;
  
  auto map_wordcount = CDiaryMgr::GetInstance().GetMapWordCount(selected_file_path, m_strDiarySection);
  std::tcout << _T("=================search===================") << std::endl;
  std::tcout << _T("section == ") << m_strDiarySection << std::endl;
  for (std::pair<TString, int> _pair : map_wordcount)
  {
    while (_pair.first.length() < 10) //ĭ �� ���߱�
      _pair.first.append(_T(" "));
    std::tcout << _T("key: ") << _pair.first << _T(" count: ") << _pair.second << std::endl;
  }
  std::tcout << _T("==========================================") << std::endl;
}
