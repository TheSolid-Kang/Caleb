#pragma once
#include "Headers.h"
#include <boost/noncopyable.hpp>
#include <sstream>

class CCalebMgr : public boost::noncopyable
{
private:
	CCalebMgr();
	CCalebMgr(const CCalebMgr&&) = delete;
	CCalebMgr&& operator = (const CCalebMgr&&) = delete;
	~CCalebMgr();

private:
	std::map<const std::string, std::string> m_map_caleb;

private:
	void initialize(void);
	void init_key(void);
	bool InitCaleb(const std::string& _diary_file_path) noexcept;

public:
	static CCalebMgr& GetInstance(void) noexcept { static CCalebMgr instance; return instance; }
	std::vector<std::string> GetVecSection(void) noexcept;


};