#include "CSettingPage.h"

CSettingPage::CSettingPage()
{

}

CSettingPage::~CSettingPage()
{

}

void CSettingPage::initialize(void)
{
	auto arr_note = build_array(typeid(*this).name()
		, "1. default setting"
		, ""
		, ""
		, "99.EXIT");
	m_list_note.insert(m_list_note.end(), arr_note.begin(), arr_note.end());
}

void CSettingPage::render(void)
{
	std::for_each(m_list_note.cbegin(), m_list_note.cend(), [](auto _note) {std::cout << _note << std::endl; });
}

int CSettingPage::update(int _event)
{
	SelectSetting(CIO::ask_and_return_integer());
	return m_r_page_mgr.SetPage(static_cast<int>(PAGES::P_SELECT));
}

void CSettingPage::release(void)
{

}

void CSettingPage::SelectSetting(int _selected_setting)
{
	switch (_selected_setting)
	{
	case 1: // 1 == default setting
		DefaultSet();
		break;
	}
}

void CSettingPage::DefaultSet(void)
{
	//1. SET Config.INI
	std::string exe_path = CFIOMgr::GetEXEFilePath();
	CINIMgr::WritePrivateProfileStringA_INI("PATH", "DIARY_FORM_PATH", exe_path + "\\Form\\DiaryForm.TXT");
	CINIMgr::WritePrivateProfileStringA_INI("PATH", "DIARY_PATH", "C:\\Caleb\\CalebRecord");
	CINIMgr::WritePrivateProfileStringA_INI("PATH", "CALEB_FORM_PATH", exe_path + "\\Form\\CalebForm.TXT");
	CINIMgr::WritePrivateProfileStringA_INI("PATH", "CALEB_PATH", "C:\\Caleb\\Caleb");

	CINIMgr::WritePrivateProfileStringA_INI("SECTION", "ARR_DIARY_SECTION", "Mission|Record|TheWords|Fellowship|Praise");
	CINIMgr::WritePrivateProfileStringA_INI("SECTION", "ARR_CALEB_SECTION", "Caleb|Reason|Mission|Record");
	if(CINIMgr::GetPrivateProfileStringA_INI("SEARCH","ARR_KEYWORD") == "0" )
		CINIMgr::WritePrivateProfileStringA_INI("SEARCH", "ARR_KEYWORD", "�ƹ���|�°�|���");

	//2. diary form ���� ����
	auto vec_section = CDiaryMgr::GetInstance().GetVecSection();
	StringBuilder str_buil;
	std::for_each(vec_section.cbegin(), vec_section.cend()
		, [&str_buil](const std::string& _str_section) {
			str_buil.append_endl("<--" + _str_section + "-->");
			str_buil.append_endl("");
			str_buil.append_endl("<--End " + _str_section + "-->");
			str_buil.append_endl("");
			str_buil.append_endl("");
			str_buil.append_endl("");
		});
	CFIOMgr::CreateDirectorysA(exe_path + "\\Form\\");
	std::string diary_form_path = CINIMgr::GetPrivateProfileStringA_INI("PATH", "DIARY_FORM_PATH");
	CFIOMgr::WriteText(diary_form_path, str_buil.str());

	//3. caleb form ���� ����
	vec_section = CCalebMgr::GetInstance().GetVecSection();
	str_buil.clear();
	std::for_each(vec_section.cbegin(), vec_section.cend()
		, [&str_buil](const std::string& _str_section) {
			str_buil.append_endl("<--" + _str_section + "-->");
			str_buil.append_endl("");
			str_buil.append_endl("<--End " + _str_section + "-->");
			str_buil.append_endl("");
			str_buil.append_endl("");
			str_buil.append_endl("");
		});
	CFIOMgr::CreateDirectorysA(exe_path + "\\Form\\");
	std::string caleb_form_path = CINIMgr::GetPrivateProfileStringA_INI("PATH", "CALEB_FORM_PATH");
	CFIOMgr::WriteText(caleb_form_path, str_buil.str());

	//4. diary ���� ���� ��� ����
	CFIOMgr::CreateDirectorysA(CINIMgr::GetPrivateProfileStringA_INI("PATH", "DIARY_PATH"));

	//5. caleb ���� ���� ��� ����
	CFIOMgr::CreateDirectorysA(CINIMgr::GetPrivateProfileStringA_INI("PATH", "CALEB_PATH"));



}