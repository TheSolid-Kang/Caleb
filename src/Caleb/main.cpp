#include "Headers.h"
#include "CPageMgr.h"
#include <Windows.h>

#include <locale.h>
int main() {
	setlocale(LC_ALL, ""); 
	/*
#ifdef _WIN32
	SetConsoleOutputCP(CP_UTF8); // == std::locale::global(std::locale(".UTF-8"));
#endif*/
	std::locale::global(std::locale(".UTF-8"));
	//std::locale::global(std::locale("Korean"));
	CPageMgr::GetInstance().SetPage();
	while (EXIT != CPageMgr::GetInstance().Execute());
	return NULL;
}