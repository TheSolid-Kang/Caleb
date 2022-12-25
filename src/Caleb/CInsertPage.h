#pragma once
#include "CPage.h"
class CInsertPage : public CPage
{
public:
	CInsertPage();
	~CInsertPage();
private:
	// CPage��(��) ���� ��ӵ�
	void initialize(void) override;
	void render(void) override;
	int update(int _event) override;
	void release(void) override;


private:
	void CreateDiary(int _iSelectedEvent = 1);
	MyString CreateName(int _iSelectedEvent );
};

/*
[DiaryForm.TXT]
<--�� ��-->
1.
2.
3.
4.
5.

<--record-->

============================================================

[config.ini]
[PATH]
DIARY_FORM_PATH=C:\Users\hoppi\Desktop\TheSolid_KANG\Project\C++_Project\Practice\x64\Debug\Form\DiaryForm.TXT
DIARY_PATH=C:\Caleb\CalebRecord\


*/