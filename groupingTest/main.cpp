#include <iostream>
#include <fstream>
#include <tchar.h>
#include "../common/common.h"
#include "../dfanew/dfanew.h"
#include "../grouping/grouping.h"
#include "../compilernew/compilernew.h"
#include "../mergedfanew/MergeDfanew.h"

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
	////compilenew(_T("..\\..\\input\\allrules.rule"), result);
	////result.WriteToFile(_T("..\\..\\output\\result.cdt"));
	result.ReadFromFile(_T("..\\..\\output\\result.cdt"));
	CGROUPRes groupRes;

	//CTimer ctime;
	//grouping(result, groupRes);
	//std::cout << "分组时间： " << ctime.Reset() << std::endl;

	//groupRes.WriteToFile(_T("..\\..\\output\\GroupResut.cdt"));

	groupRes.ReadFromFile(_T("..\\..\\output\\GroupResut.cdt"));
	//groupRes.WriteToFile(_T("..\\..\\output\\GroupResut1.cdt"));

	std::ifstream fin("..\\..\\output\\mergetest.txt");
	std::vector<size_t> vecIds;
	size_t tmp;
	while (fin >> tmp)
	{
		vecIds.push_back(tmp);
	}

	std::vector<CDfanew> vecDfas(2);
	CDfanew MergeDfa;
	vecDfas[0] = result.GetDfaTable()[vecIds[0]];
	for (size_t i = 1; i < vecIds.size(); ++i)
	{
		vecDfas[1] = result.GetDfaTable()[vecIds[i]];
		if (!NOrMerge(vecDfas, MergeDfa))
		{
			std::cout << "ERROR" << std::endl;
		}
		vecDfas[0] = MergeDfa;
	}
	std::cout << MergeDfa.Size() << std::endl;

	system("pause");
	return 0;
}