#include "CUpdatePage.h"

CUpdatePage::CUpdatePage()
{
}

CUpdatePage::~CUpdatePage()
{

}

void CUpdatePage::initialize(void)
{
	auto arr_note = build_array(typeid(*this).name()
		, "1. ������ �ϱ� ����"
		, "2. ����� ���� �ϱ� ����(���� ��� ����: 221127.TXT)"
		, "3. "
		, ""
		, "99. EXIT");
	m_list_note.insert(m_list_note.end(), arr_note.begin(), arr_note.end());
}

void CUpdatePage::render(void)
{
	std::for_each(m_list_note.cbegin(), m_list_note.cend(), [](auto _note) {std::cout << _note << std::endl; });
}

int CUpdatePage::update(int _event)
{
	return 0;
}

void CUpdatePage::release(void)
{
}
