#pragma once
#include "CPage.h"
class CSelectPage : public CPage
{
public:
	CSelectPage();
	~CSelectPage();

private:
	std::list<std::string> m_list_page;

private:
	// CPage을(를) 통해 상속됨
	void initialize(void) override;
	void render(void) override;
	int update(int _event) override;
	void release(void) override;
};