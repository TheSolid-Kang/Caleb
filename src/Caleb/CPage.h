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
	virtual void init_func(void) = 0;
	virtual void init_selected_func(void) = 0;
	virtual void render(void) = 0;
	virtual void render(const void* _p_void) = 0;
	virtual void render(TString& _str) = 0;
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
	std::list<TString> m_list_title;

	enum class COMMON_FUNC : size_t { PRINT_TITLE, END };
	enum class COMMON_SELECTED_FUNC : size_t { PRINT_TITLE, END };

	std::unique_ptr<std::map<size_t, std::function<std::shared_ptr<void>(const void*)>>> m_uniq_map_func;
	std::unique_ptr<std::map<size_t, std::function<std::shared_ptr<void>(const void*)>>> m_uniq_map_selected_func;

protected:
	// IPage을(를) 통해 상속됨
	virtual void initialize(void) override;
	virtual void init_func(void) override;
	virtual void init_selected_func(void) override;
	virtual void render(void) override;
	virtual void render(const void* _p_void) override;
	void render(TString& _str) override;
	void render(TString&& _str) { render(_str); };
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