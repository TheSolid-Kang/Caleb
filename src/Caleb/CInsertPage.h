#pragma once
#include "CPage.h"
class CInsertPage : public CPage
{
public:
	CInsertPage();
	~CInsertPage();
private:
	enum class FUNC : size_t {
		ROOT = static_cast<size_t>(COMMON_FUNC::END)
		, CREATE_DIARY, CREATE_FILE_NAME
		, END
	};
	enum class SELECTED_FUNC : size_t {
		ROOT = static_cast<size_t>(COMMON_SELECTED_FUNC::END)
		, END
	};


private:
	// IPage을(를) 통해 상속됨
	void initialize(void) override;
	void init_func(void) override;
	void init_selected_func(void) override;
	void render(void) override;
	int update(int _event) override;
	void release(void) override;

private:
	void CreateDiary(int _iSelectedEvent = 1);
	TString CreateName(int _iSelectedEvent );
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