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

bool CCalebMgr::InitCaleb(const std::string& _diary_file_path) noexcept
{

	return false;
}

std::vector<std::string> CCalebMgr::GetVecSection(void) noexcept
{
	std::string str_ARR_DIARY_SECTION = CINIMgr::GetPrivateProfileString_INI("SECTION", "ARR_CALEB_SECTION");
	return CMyEtc::Split(str_ARR_DIARY_SECTION, '|');
}
