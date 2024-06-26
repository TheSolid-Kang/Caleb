#include "CSettingPage.h"

CSettingPage::CSettingPage()
{

}

CSettingPage::~CSettingPage()
{

}

void CSettingPage::initialize(void)
{
	auto arr_note = build_array(_T("1. default setting")
		, _T("")
		, _T("")
		, _T("99.EXIT"));
	m_list_title.insert(m_list_title.end(), arr_note.begin(), arr_note.end());
}

void CSettingPage::render(void)
{
	std::for_each(m_list_title.cbegin(), m_list_title.cend(), [](auto _note) {std::tcout << _note << std::endl; });
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
	TString exe_path = CFIOMgr::GetEXEFilePath();
	CINIMgr::WritePrivateProfileString_INI(_T("PATH"), _T("DIARY_FORM_PATH"), exe_path + _T("\\Form\\DiaryForm.TXT"));
	CINIMgr::WritePrivateProfileString_INI(_T("PATH"), _T("DIARY_PATH"), _T("C:\\Caleb\\CalebRecord"));
	CINIMgr::WritePrivateProfileString_INI(_T("PATH"), _T("CALEB_FORM_PATH"), exe_path + _T("\\Form\\CalebForm.TXT"));
	CINIMgr::WritePrivateProfileString_INI(_T("PATH"), _T("CALEB_PATH"), _T("C:\\Caleb\\Caleb"));

	CINIMgr::WritePrivateProfileString_INI(_T("SECTION"), _T("ARR_DIARY_SECTION"), _T("Mission|Record|TheWords|Fellowship|Praise"));
	CINIMgr::WritePrivateProfileString_INI(_T("SECTION"), _T("ARR_CALEB_SECTION"), _T("Caleb|Reason|Mission|Record"));
	if(CINIMgr::GetPrivateProfileString_INI(_T("SEARCH"), _T("ARR_KEYWORD")) == _T("0"))
		CINIMgr::WritePrivateProfileString_INI(_T("SEARCH"), _T("ARR_KEYWORD"), _T(""));

	//2. diary form 
	auto vec_section = CDiaryMgr::GetInstance().GetVecSection();
	StringBuilder str_buil;
	std::for_each(vec_section.cbegin(), vec_section.cend()
		, [&str_buil](const TString& _str_section) {
			str_buil.Append_endl(_T("<--") + _str_section + _T("-->"));
			str_buil.Append_endl(_T(""));
			str_buil.Append_endl(_T("<--End ") + _str_section + _T("-->"));
			str_buil.Append_endl(_T(""));
			str_buil.Append_endl(_T(""));
			str_buil.Append_endl(_T(""));
		});
	CFIOMgr::CreateDirectorys(exe_path + _T("\\Form\\"));
	TString diary_form_path = CINIMgr::GetPrivateProfileString_INI(_T("PATH"), _T("DIARY_FORM_PATH"));
	CFIOMgr::WriteText(diary_form_path, str_buil.str());

	//3. caleb form 
	vec_section = CCalebMgr::GetInstance().GetVecSection();
	str_buil.clear();
	std::for_each(vec_section.cbegin(), vec_section.cend()
		, [&str_buil](const TString& _str_section) {
			str_buil.Append_endl(_T("<--") + _str_section + _T("-->"));
			str_buil.Append_endl(_T(""));
			str_buil.Append_endl(_T("<--End ") + _str_section + _T("-->"));
			str_buil.Append_endl(_T(""));
			str_buil.Append_endl(_T(""));
			str_buil.Append_endl(_T(""));
		});
	CFIOMgr::CreateDirectorys(exe_path + _T("\\Form\\"));
	TString caleb_form_path = CINIMgr::GetPrivateProfileString_INI(_T("PATH"), _T("CALEB_FORM_PATH"));
	CFIOMgr::WriteText(caleb_form_path, str_buil.str());

	//4. diary 
	CFIOMgr::CreateDirectorys(CINIMgr::GetPrivateProfileString_INI(_T("PATH"), _T("DIARY_PATH")));

	//5. caleb 
	CFIOMgr::CreateDirectorys(CINIMgr::GetPrivateProfileString_INI(_T("PATH"), _T("CALEB_PATH")));



}