#include "Headers.h"
#include "CPageMgr.h"
#include <Windows.h>
int main() {
	/*
#ifdef _WIN32
	SetConsoleOutputCP(CP_UTF8); // == std::locale::global(std::locale(".UTF-8"));
#endif
	*/
	CPageMgr::GetInstance().SetPage();
	while (EXIT != CPageMgr::GetInstance().Execute());
	return NULL;
}