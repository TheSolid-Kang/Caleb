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
	std::map<const std::string, std::string> m_map_diary;

private:
	void initialize(void);
	void init_key(void);
	bool InitDiary(const std::string& _diary_file_path) noexcept;

public:
	static CDiaryMgr& GetInstance(void) noexcept { static CDiaryMgr instance; return instance; }

public:
	std::string GetDiarySelectedSection(const std::string& _diary_file_path, const std::string& _section) ;
	std::map<const std::string, std::string>& GetMapDiary(void) noexcept { return m_map_diary; }
	std::vector<std::string> GetVecSection(void) noexcept;
	std::string& GetDiary(const std::string& _diary_file_path) ;
	std::map<std::string, int> GetMapWordCount(const std::string& _diary_file_path, const std::string& _section);
};