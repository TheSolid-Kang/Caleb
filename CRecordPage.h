#pragma once
#include "CPage.h"
class CRecordPage : public CPage
{
public:
	CRecordPage();
	~CRecordPage();

private:
	std::vector<std::string> m_vec_file_name;
	std::string m_strDiarySection;

private:
	// CPage을(를) 통해 상속됨
	void initialize(void) override;
	void init_note(void) noexcept;
	void render(void) override;
	int update(int _event) override;
	void release(void) override;

private:
	void PrintSelectedDairy(int _selected_diary);


};