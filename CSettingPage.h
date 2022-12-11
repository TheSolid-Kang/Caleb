#pragma once
#include "CPage.h"

class CSettingPage : public CPage
{
public:
	CSettingPage();
	~CSettingPage();

private:
	// CPage��(��) ���� ��ӵ�
	void initialize(void) override;
	void render(void) override;
	int update(int _event) override;
	void release(void) override;

private:
	void SelectSetting(int _selected_setting = 1); //1 == default
	void DefaultSet(void); 


};

