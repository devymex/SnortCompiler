#include <iostream>
#include <fstream>
#include <tchar.h>
#include "../grouping/grouping.h"
#include "../compilernew/compilernew.h"

int main()
{
	//std::ifstream f1("..\\..\\output\\result.cdt", std::ios::binary);
	//std::ifstream f2("..\\..\\output\\result1.cdt", std::ios::binary);
	//BYTE *p1 = new BYTE[10000000];
	//BYTE *p2 = new BYTE[10000000];
	//f1.read((char*)p1, 7953451);
	//f2.read((char*)p2, 7953451);
	//for (size_t i = 0; i < 7953451; ++i)
	//{
	//	if (p1[i] != p2[i])
	//	{
	//		std::cout << i << std::endl;
	//	}
	//}
	CResNew result;
	//compilenew(_T("..\\..\\input\\allrules.rule"), result);
	//result.WriteToFile(_T("..\\..\\output\\result.cdt"));
	result.ReadFromFile(_T("..\\..\\output\\result.cdt"));
	CGROUPRes groupRes;

	CTimer ctime;
	grouping(result, groupRes);
	std::cout << "分组时间： " << ctime.Reset() << std::endl;

	groupRes.WriteToFile(_T("..\\..\\output\\GroupResut_2.cdt"));

	//groupRes.ReadFromFile(_T("..\\..\\output\\GroupResut.cdt"));
	//groupRes.WriteToFile(_T("..\\..\\output\\GroupResut1.cdt"));

	system("pause");
	return 0;
}