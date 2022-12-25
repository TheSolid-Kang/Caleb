#pragma once
#include "Headers.h"
#include <boost/noncopyable.hpp>
#include <sstream>

class CDiaryMgr : public boost::noncopyable
{
private:
	CDiaryMgr();
	CDiaryMgr(const CDiaryMgr&) = delete;
	CDiaryMgr& operator = (const CDiaryMgr&) = delete;
	~CDiaryMgr();

private:
	std::map<const MyString, MyString> m_map_diary;

private:
	void initialize(void);
	void init_key(void);
	bool InitDiary(const MyString& _diary_file_path) noexcept;

public:
	static CDiaryMgr& GetInstance(void) noexcept { static CDiaryMgr instance; return instance; }

public:
	MyString GetDiarySelectedSection(const MyString& _diary_file_path, const MyString& _section) ;
	std::map<const MyString, MyString>& GetMapDiary(void) noexcept { return m_map_diary; }
	std::vector<MyString> GetVecSection(void) noexcept;
	MyString& GetDiary(const MyString& _diary_file_path) ;
	std::map<MyString, int> GetMapWordCount(const MyString& _diary_file_path, const MyString& _section);
};