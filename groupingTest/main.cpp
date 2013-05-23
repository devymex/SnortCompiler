#include <iostream>
#include <tchar.h>
#include "../grouping/grouping.h"
#include "../compilernew/compilernew.h"

int main()
{
	CResNew result;
	compilenew(_T("..\\..\\input\\allrules.rule"), result);
	CGROUPRes groupRes;
	grouping(result, groupRes);
	groupRes.WriteToFile(_T("..\\..\\output\\GroupResut.cdt"));

	//groupRes.ReadFromFile(_T("..\\..\\output\\GroupResut.cdt"));
	//groupRes.WriteToFile(_T("..\\..\\output\\GroupResut1.cdt"));

	system("pause");
	return 0;
}