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
  m_vec_file_name = CFIOMgr::GetFilesInDirectory(CINIMgr::GetPrivateProfileStringA_INI("PATH", "DIARY_PATH"));
  std::reverse(m_vec_file_name.begin(), m_vec_file_name.end());

  auto vec_section = CDiaryMgr::GetInstance().GetVecSection();
  std::for_each(m_vec_file_name.cbegin(), m_vec_file_name.cend(), [&vec_section](auto _file_path) {
    for (auto _section : vec_section)
      CDiaryMgr::GetInstance().GetDiarySelectedSection(_file_path, _section);
    });
}

void CRecordPage::render(void)
{
  int i = 0;

  auto map_diary = CDiaryMgr::GetInstance().GetMapDiary();
  std::cout << typeid(*this).name() << std::endl;
  std::for_each(map_diary.rbegin(), map_diary.rend(), [&i](std::pair<const std::string, std::string>& _pair) {
    size_t index = _pair.first.find_last_of("\\") + 1;
    std::cout << std::to_string(++i) + ". " + _pair.first.substr(index, _pair.first.size() - index) << std::endl;
    });
  std::cout << m_vec_file_name.size() + 1 << ". EXIT" << std::endl;
}

int CRecordPage::update(int _event)
{
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
  auto diary = CDiaryMgr::GetInstance().GetDiary(selected_file_path);
  std::cout << diary << std::endl;

  auto vec_section = CDiaryMgr::GetInstance().GetVecSection();
  vec_section = { "Record" };//221204 임시 코드 
  for (auto _section : vec_section)
  {
    auto map_wordcount = CDiaryMgr::GetInstance().GetMapWordCount(selected_file_path, _section);
    std::cout << "=================search===================" << std::endl;
    std::cout << "section == " << _section << std::endl;
    std::locale::global(std::locale("Korean"));
    for (std::pair<std::string, int> _pair : map_wordcount)
    {
      while (_pair.first.length() < 10) //칸 수 맞추기
        _pair.first.append(" ");
      std::cout << "key: " << _pair.first << " count: " << _pair.second << std::endl;
    }
    std::cout << "==========================================" << std::endl;
  }
}
