#pragma once
#include "CPage.h"
class CTestPage : public CPage
{
public:
	CTestPage();
	~CTestPage();

private:
	enum class FUNC : size_t {
		ROOT = static_cast<size_t>(COMMON_FUNC::END)
		, ONE = 1, TWO, THREE, FOUR, FIVE
		, END
	};
	enum class SELECTED_FUNC : size_t {
		ROOT = static_cast<size_t>(COMMON_SELECTED_FUNC::END)
		, ONE = 1, TWO, THREE, FOUR, FIVE
		, END
	};


private:
	void initialize(void) override;
	void init_func(void) override;
	void init_selected_func(void) override;
	void render(void) override;
	int update(int _event) override;
	void release(void) override;


};

