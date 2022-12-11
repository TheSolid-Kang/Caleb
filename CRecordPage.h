#pragma once
#include "CPage.h"
class CRecordPage : public CPage
{
public:
	CRecordPage();
	~CRecordPage();

private:
	std::vector<std::string> m_vec_file_name;

private:
	// CPage��(��) ���� ��ӵ�
	void initialize(void) override;
	void render(void) override;
	int update(int _event) override;
	void release(void) override;

private:
	void PrintSelectedDairy(int _selected_diary);
};