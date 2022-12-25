#include "CCalebMgr.h"
#include "CMyEtc.hpp"
#include "CINIMgr.hpp"
#include "CFIOMgr.hpp"

CCalebMgr::CCalebMgr()
{
	initialize();
}

CCalebMgr::~CCalebMgr()
{

}

void CCalebMgr::initialize(void)
{
}

void CCalebMgr::init_key(void)
{
	
}

bool CCalebMgr::InitCaleb(const TString& _diary_file_path) noexcept
{

	return false;
}

std::vector<TString> CCalebMgr::GetVecSection(void) noexcept
{
	TString str_ARR_DIARY_SECTION = CINIMgr::_GetPrivateProfileString_INI(_T("SECTION"), _T("ARR_CALEB_SECTION"));
	return CMyEtc::Split(str_ARR_DIARY_SECTION, _T('|'));
}
