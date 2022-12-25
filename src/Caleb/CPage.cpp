#include "CPage.h"

// �޸� ���� �ֿܼ� ǥ���ϱ� ���� �Լ�
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
	//CFIOMgr���� std::locale::global(std::locale(".UTF-8"));
	//ó�� �����Ƿ� 
	//�ַܼ� �ѱ� ������ korean���� ���� �ؾ���.
	//std::locale::global(std::locale("Korean"));

	render();

	int Update = update(_event);

	return Update;
}