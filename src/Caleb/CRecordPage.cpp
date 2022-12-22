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
  m_list_note.insert(m_list_note.end(), vec_section.cbegin(), vec_section.cend());
  std::for_each(m_list_note.begin(), m_list_note.end(), [&i](std::string& _note) { _note = std::to_string(i++) + ". " + _note; });

  m_list_note.emplace_front(typeid(*this).name());
  m_list_note.emplace_back(std::to_string(m_list_note.size() + 1) + ". EXIT");
}

void CRecordPage::init_note(void) noexcept
{
  if (0 != m_list_note.size())
    m_list_note.clear();

  m_vec_file_name = CFIOMgr::GetFilesInDirectory(CINIMgr::GetPrivateProfileString_INI("PATH", "DIARY_PATH"));
  std::reverse(m_vec_file_name.begin(), m_vec_file_name.end());

  int i = 0;
  m_list_note.emplace_back(typeid(*this).name());
  std::for_each(m_vec_file_name.cbegin(), m_vec_file_name.cend(), [&](const std::string& _file_name) {
    size_t index = _file_name.find_last_of("\\") + 1;
    m_list_note.emplace_back(std::to_string(++i) + ". " + _file_name.substr(index, _file_name.size() - index));
    });
  m_list_note.emplace_back(std::to_string(m_vec_file_name.size() + 1) + ". EXIT");
}

void CRecordPage::render(void)
{
  std::for_each(m_list_note.cbegin(), m_list_note.cend(), [](const std::string& _title) {std::cout << _title << std::endl; });
}

int CRecordPage::update(int _event)
{
  int selectedDiarySection = CIO::ask_and_return_integer();
  auto vec_section = CDiaryMgr::GetInstance().GetVecSection();
  if(EXIT == selectedDiarySection)
    return m_r_page_mgr.SetPage(static_cast<int>(PAGES::P_SELECT));

  if(0 > selectedDiarySection || selectedDiarySection > vec_section.size())
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
  std::string selected_file_path = m_vec_file_name[_selected_diary - 1];
  std::cout << selected_file_path << std::endl;
  //auto diary = CDiaryMgr::GetInstance().GetDiary(selected_file_path);
  auto diary = CDiaryMgr::GetInstance().GetDiarySelectedSection(selected_file_path, m_strDiarySection);
  std::locale::global(std::locale("Korean"));
  std::cout << diary << std::endl;
  
  auto map_wordcount = CDiaryMgr::GetInstance().GetMapWordCount(selected_file_path, m_strDiarySection);
  std::cout << "=================search===================" << std::endl;
  std::cout << "section == " << m_strDiarySection << std::endl;
  for (std::pair<std::string, int> _pair : map_wordcount)
  {
    while (_pair.first.length() < 10) //Ä­ ¼ö ¸ÂÃß±â
      _pair.first.append(" ");
    std::cout << "key: " << _pair.first << " count: " << _pair.second << std::endl;
  }
  std::cout << "==========================================" << std::endl;
  std::locale::global(std::locale(".UTF-8"));
}
