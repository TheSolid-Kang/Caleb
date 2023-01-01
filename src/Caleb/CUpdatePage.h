#pragma once
#include "CPage.h"
class CUpdatePage : public CPage
{
public:
	CUpdatePage();
	~CUpdatePage();

private:
	enum class FUNC : size_t {
		ROOT = static_cast<size_t>(COMMON_FUNC::END)
		, END
	};
	enum class SELECTED_FUNC : size_t {
		ROOT = static_cast<size_t>(COMMON_SELECTED_FUNC::END)
		, END
	};


private:
	// IPage을(를) 통해 상속됨
	void initialize(void) override;
	void init_func(void) override;
	void init_selected_func(void) override;
	void render(void) override;
	int update(int _event) override;
	void release(void) override;
};

