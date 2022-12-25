#pragma once
#include "CPage.h"
class CSelectPage : public CPage
{
public:
	CSelectPage();
	~CSelectPage();

private:
	std::list<MyString> m_list_page;

private:
	// CPage��(��) ���� ��ӵ�
	void initialize(void) override;
	void render(void) override;
	int update(int _event) override;
	void release(void) override;
};