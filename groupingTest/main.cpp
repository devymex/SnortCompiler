#include <iostream>
#include <fstream>
#include <algorithm>
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
	compilenew(_T("..\\..\\input\\testrules.rule"), result);
	//result.WriteToFile(_T("..\\..\\output\\result.cdt"));
	//result.ReadFromFile(_T("..\\..\\output\\result.cdt"));
	//for (size_t i = 0; i < result.GetSidDfaIds().Size(); ++i)
	//{
	//	if (result.GetSidDfaIds()[i].m_dfaIds.Size() > 0 && result.GetSidDfaIds()[i].m_dfaIds[0] == 1244)
	//	{
	//		std::cout << result.GetSidDfaIds()[i].m_nSid << std::endl;
	//		break;
	//	}
	//}
	//CGROUPRes groupRes;

	//CTimer ctime;
	//grouping(result, groupRes);
	//std::cout << "分组时间： " << ctime.Reset() << std::endl;

	//groupRes.WriteToFile(_T("..\\..\\output\\GroupResut.cdt"));

	//groupRes.ReadFromFile(_T("..\\..\\output\\GroupResut.cdt"));
	//groupRes.WriteToFile(_T("..\\..\\output\\GroupResut1.cdt"));

	//std::ifstream fin("..\\..\\output\\test.txt");
	//std::vector<size_t> vecIds;
	//size_t tmp;
	//while (fin >> tmp)
	//{
	//	vecIds.push_back(tmp);
	//}

	std::cout << result.GetRegexTbl()[0][0].C_Str() << std::endl;
	std::cout << result.GetRegexTbl()[1][0].C_Str() << std::endl;

	std::vector<CDfaNew> vecDfas(2);
	CDfaNew MergeDfa;
	vecDfas[0] = result.GetDfaTable()[0];
	//vecDfas[0] = MergeDfa;
	//for (size_t i = 1; i < vecIds.size(); ++i)
	//{
	//	vecDfas[1] = result.GetDfaTable()[vecIds[i]];
	//	if (!NOrMerge(vecDfas, MergeDfa))
	//	{
	//		std::cout << "ERROR" << std::endl;
	//	}
	//	else
	//	{
	//		vecDfas[0] = MergeDfa;
	//	}
	//}
	vecDfas[1] = result.GetDfaTable()[1];
	std::cout << vecDfas[0].Size() << std::endl;
	std::cout << vecDfas[1].Size() << std::endl;
	outPutDfa(vecDfas[0], "..//..//output//dfa1.txt");
	outPutDfa(vecDfas[1], "..//..//output//dfa2.txt");
	NOrMerge(vecDfas, MergeDfa);
	std::cout << MergeDfa.Size() << std::endl;
	outPutDfa(MergeDfa, "..//..//output//dfa.txt");
	//outPutDfa(result.GetDfaTable()[vecIds[vecIds.size() - 1]], "..//..//output//dfa2_2.txt");

	system("pause");
	return 0;
}