#pragma once
#include "CPage.h"
class CRecordPage : public CPage
{
public:
	CRecordPage();
	~CRecordPage();

private:
	std::vector<TString> m_vec_file_name;
	TString m_strDiarySection;

private:
	// CPage��(��) ���� ��ӵ�
	void initialize(void) override;
	void init_note(void) noexcept;
	void render(void) override;
	int update(int _event) override;
	void release(void) override;

private:
	void PrintSelectedDairy(int _selected_diary);


};