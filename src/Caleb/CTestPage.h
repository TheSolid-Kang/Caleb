#pragma once
#include "CPage.h"
class CTestPage : public CPage
{
public:
	CTestPage();
	~CTestPage();

private:
	enum class FUNC :int{ONE = 1, TWO, THREE, FOUR, FIVE};
	enum class TEST_FUNC :int{ONE = 1, TWO, THREE, FOUR, FIVE};
	std::unique_ptr<std::map<size_t, std::function<std::shared_ptr<void>(const void*)>>> m_uniq_map_func;
	std::unique_ptr<std::map<size_t, std::function<std::shared_ptr<void>(const void*)>>> m_uniq_map_testfunc;

private:
	void initialize(void) override;
	void init_func(void);
	void init_testfunc(void);
	void render(void) override;
	int update(int _event) override;
	void release(void) override;


};

