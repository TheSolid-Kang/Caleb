#pragma once
#include "Headers.h"
#include "CPageMgr.h"
#include "CFIOMgr.hpp"
#include "CINIMgr.hpp"
#include "StringBuilder.h"
#include "Enums.h"
#include <boost/noncopyable.hpp>
#include "CDiaryMgr.h"
#include "CCalebMgr.h"

class IPage
{
protected:
	virtual void initialize(void) = 0;
	virtual void render(void) = 0;
	virtual int update(int _event = 0) = 0;
	virtual void release(void) = 0;
};

class CPage : public IPage, public boost::noncopyable
{
public:
	CPage();
	virtual ~CPage();
protected:
	CPageMgr& m_r_page_mgr;
	std::list<std::string> m_list_note;

protected:
	// IPage을(를) 통해 상속됨
	virtual void initialize(void) override;
	virtual void render(void) override;
	virtual int update(int _event) override;
	virtual void release(void) override;

public:
	int Execute(int _event);

protected:
	template<typename... Args>
	auto build_array(Args&&... _args)
		->std::array<std::common_type_t<Args...>, sizeof ...(_args)>
	{ return { std::forward < std::common_type_t<Args...>>((Args&&)_args)... }; }


};