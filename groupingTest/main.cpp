#include <iostream>
#include <tchar.h>
#include "../grouping/grouping.h"
#include "../compilernew/compilernew.h"

int main()
{
	CResNew result;
	//compilenew(_T("..\\..\\input\\allrules.rule"), result);
	//result.WriteToFile(_T("..\\..\\output\\result.cdt"));
	result.ReadFromFile(_T("..\\..\\output\\result.cdt"));
	CGROUPRes groupRes;

	CTimer ctime;
	grouping(result, groupRes);
	std::cout << "分组时间： " << ctime.Reset() << std::endl;

	groupRes.WriteToFile(_T("..\\..\\output\\GroupResut.cdt"));

	//groupRes.ReadFromFile(_T("..\\..\\output\\GroupResut.cdt"));
	//groupRes.WriteToFile(_T("..\\..\\output\\GroupResut1.cdt"));

	system("pause");
	return 0;
}