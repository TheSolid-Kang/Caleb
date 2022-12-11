#include "CPageMgr.h"
#include "CPage.h"
#include "Enums.h"
#include "CSelectPage.h"
#include "CInsertPage.h"
#include "CRecordPage.h"
#include "CSettingPage.h"


CPageMgr::CPageMgr()
	: m_ptr_page(nullptr)
{

}

CPageMgr::~CPageMgr()
{
	if (nullptr != m_ptr_page)
		delete m_ptr_page;
}

int CPageMgr::SetPage(int _page)
{
	int result = 0;
	if (nullptr != m_ptr_page)
		delete m_ptr_page;

	switch (_page)
	{
	case static_cast<int>(PAGES::P_SELECT):
		m_ptr_page = new CSelectPage();
		break;
	case static_cast<int>(PAGES::P_INSERT):
		m_ptr_page = new CInsertPage();
		break;
	case static_cast<int>(PAGES::P_RECORD):
		m_ptr_page = new CRecordPage();
		break;
	case static_cast<int>(PAGES::P_UPDATE):
		m_ptr_page = new CSelectPage();
		break;
	case static_cast<int>(PAGES::P_DELETE):
		m_ptr_page = new CSelectPage();
		break;
	case static_cast<int>(PAGES::P_SETTING):
		m_ptr_page = new CSettingPage();
		break;

	case static_cast<int>(EXIT):
		m_ptr_page = new CPage();
		result = EXIT;
		break;
	}
	return result;
}

int  CPageMgr::Execute(int _event)
{
	return (*m_ptr_page).Execute(_event);
}