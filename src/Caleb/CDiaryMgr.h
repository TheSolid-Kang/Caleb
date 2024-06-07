#pragma once
#include "Headers.h"
//#include <boost/noncopyable.hpp>
#include <sstream>

class CDiaryMgr //: public boost::noncopyable
{
private:
	CDiaryMgr();
	CDiaryMgr(const CDiaryMgr&) = delete;
	CDiaryMgr& operator = (const CDiaryMgr&) = delete;
	~CDiaryMgr();

private:
	std::map<const TString, TString> m_map_diary;

private:
	void initialize(void);
	void init_key(void);
	bool InitDiary(const TString& _diary_file_path) noexcept;

public:
	static CDiaryMgr& GetInstance(void) noexcept { static CDiaryMgr instance; return instance; }

public:
	TString GetDiarySelectedSection(const TString& _diary_file_path, const TString& _section) ;
	std::map<const TString, TString>& GetMapDiary(void) noexcept { return m_map_diary; }
	std::vector<TString> GetVecSection(void) noexcept;
	TString& GetDiary(const TString& _diary_file_path) ;
	std::map<TString, int> GetMapWordCount(const TString& _diary_file_path, const TString& _section);
};