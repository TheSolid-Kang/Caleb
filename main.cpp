#include "Headers.h"
#include "CPageMgr.h"
int main() {
	CPageMgr::GetInstance().SetPage();
	while (EXIT != CPageMgr::GetInstance().Execute());
	return NULL;
}