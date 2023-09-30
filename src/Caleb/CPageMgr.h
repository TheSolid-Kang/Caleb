#pragma once
#include "Headers.h"
#include <boost/noncopyable.hpp>

class CPage;
class CPageMgr : public boost::noncopyable
{
private:
	CPageMgr();
	CPageMgr(const CPageMgr&) = delete;
	CPageMgr& operator = (const CPageMgr&) = delete;
	~CPageMgr();

public:
	static CPageMgr& GetInstance() noexcept { static CPageMgr instance; return instance; }
private:
	CPage* m_ptr_page;

public:
	int SetPage(int _page = 0);// 0 == static_cast<int>(PAGES::P_SELECT)

public:
	int Execute(int _event = 0);




};