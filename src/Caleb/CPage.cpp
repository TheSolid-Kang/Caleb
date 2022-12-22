#include "CPage.h"

// 메모리 릭을 콘솔에 표시하기 위한 함수
#if _DEBUG
#define new new(_NORMAL_BLOCK, __FILE__, __LINE__)
#define malloc(s) _malloc_dbg(s, _NORMAL_BLOCK, __FILE__, __LINE__)
#endif

CPage::CPage()
	: m_r_page_mgr(CPageMgr::GetInstance())
{

}

CPage::~CPage()
{

}

void CPage::initialize(void)
{

}

void CPage::render(void)
{

}

int CPage::update(int _event = 0)
{
	return 99;
}

void CPage::release(void)
{
}

int CPage::Execute(int _event = 0)
{
	_CrtDumpMemoryLeaks();

	system("pause");
	system("cls");

	initialize();
	//CFIOMgr에서 std::locale::global(std::locale(".UTF-8"));
	//처리 했으므로 
	//콘솔로 한글 보려면 korean으로 변경 해야함.
	//std::locale::global(std::locale("Korean"));

	std::locale::global(std::locale("Korean"));
	render();
	std::locale::global(std::locale(".UTF-8"));

	int Update = update(_event);

	return Update;
}