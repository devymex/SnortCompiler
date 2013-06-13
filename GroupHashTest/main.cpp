#include <iostream>
#include <fstream>
#include <algorithm>
#include <tchar.h>

#include <hwprj\ctimer.h>
#include <hwprj\compiler.h>
#include <hwprj\buildhash.h>

int main()
{
	//std::ifstream f1("..\\..\\output\\result.cdt", std::ios::binary);
	//std::ifstream f2("..\\..\\output\\result1.cdt", std::ios::binary);
	//byte *p1 = new byte[10000000];
	//byte *p2 = new byte[10000000];
	//f1.read((char*)p1, 7953451);
	//f2.read((char*)p2, 7953451);
	//for (ulong i = 0; i < 7953451; ++i)
	//{
	//	if (p1[i] != p2[i])
	//	{
	//		std::cout << i << std::endl;
	//	}
	//}

	//CCompileResults result;
	//CompileRuleSet(_T("..\\..\\input\\allrules.rule"), result);
	//result.WriteToFile(_T("..\\..\\output\\result.cdt"));
	////result.ReadFromFile(_T("..\\..\\output\\result.cdt"));

	////for (ulong i = 0; i < result.GetSidDfaIds().Size(); ++i)
	////{
	////	if (result.GetSidDfaIds()[i].m_dfaIds.Size() > 0 && result.GetSidDfaIds()[i].m_dfaIds[0] == 1244)
	////	{
	////		std::cout << result.GetSidDfaIds()[i].m_nSid << std::endl;
	////		break;
	////	}
	////}
	//CGROUPRes groupRes;

	//CTimer ctime;
	//Grouping(result, groupRes);
	//std::cout << "分组时间： " << ctime.Reset() << std::endl;

	//groupRes.WriteToFile(_T("..\\..\\output\\GroupResut.cdt"));

	//groupRes.ReadFromFile(_T("..\\..\\output\\GroupResut.cdt"));
	//groupRes.WriteToFile(_T("..\\..\\output\\GroupResut1.cdt"));

	//std::ifstream fin("..\\..\\output\\Ids.txt");
	//std::vector<ulong> vecIds;
	//ulong tmp;
	//while (fin >> tmp)
	//{
	//	vecIds.push_back(tmp);
	//}

	//SIGNATURE Sig = vecIds.back();
	//vecIds.pop_back();
	//for (ulong i = 0; i < vecIds.size(); ++i)
	//{
	//	bool flag = false;
	//	for (ulong j = 0; j < result.GetRegexTbl()[vecIds[i]].GetSigCnt(); ++j)
	//	{
	//		if (result.GetRegexTbl()[vecIds[i]].GetSig(j) == Sig)
	//		{
	//			flag = true;
	//			break;
	//		}
	//	}
	//	if (!flag)
	//	{
	//		std::cout << "No Sig" << std::endl;
	//	}
	//}

	//std::vector<CDfa> vecDfas(2);
	//CDfa MergeDfa;
	//vecDfas[0] = result.GetDfaTable()[vecIds[0]];
	//for (ulong i = 1; i < vecIds.size(); ++i)
	//{
	//	vecDfas[1] = result.GetDfaTable()[vecIds[i]];
	//	if (MergeMultipleDfas(vecDfas, MergeDfa))
	//	{
	//		vecDfas[0] = MergeDfa;
	//	}
	//	else
	//	{
	//		std::cout << "ERROR" << std::endl;
	//		break;
	//	}
	//}
	//std::cout << vecDfas[0].Size() << std::endl;

	CTimer ctime;
	CCompileResults result;
	//CompileRuleSet("..\\allrules.rule", result);
	//result.WriteToFile("..\\result.cdt");
	result.ReadFromFile("..\\result.cdt");
	CGROUPRes groupRes;
	Grouping(result, groupRes);
	groupRes.WriteToFile("..\\GroupResut.cdt");
	HASHRES HashResMap;
	HashMapping(groupRes, HashResMap);
	groupRes.WriteToFile("..\\FinalResut.cdt");
	std::cout << "Total time: " << ctime.Reset() << std::endl;
	std::cout << groupRes.GetGroups().Size() << std::endl;
	std::cout << HashResMap.size() << std::endl;

	system("pause");
	return 0;
}