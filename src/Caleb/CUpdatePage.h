#pragma once
#include "CPage.h"
class CUpdatePage : public CPage
{
public:
	CUpdatePage();
	~CUpdatePage();

private:
	// CPage��(��) ���� ��ӵ�
	void initialize(void) override;
	void render(void) override;
	int update(int _event) override;
	void release(void) override;
};

