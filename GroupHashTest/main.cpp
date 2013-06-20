#include <iostream>
#include <fstream>
#include <algorithm>
#include <sstream>
#include <tchar.h>

#include <hwprj\ctimer.h>
#include <hwprj\unsary.h>
#include <hwprj\signatures.h>
#include <hwprj\compiler.h>
#include <hwprj\compres.h>
#include <hwprj\groupres.h>
#include <hwprj\grouping.h>
#include <hwprj\buildhash.h>
#include <hwprj\trace.h>

int main()
{
	//CCompileResults result;
	//result.ReadFromFile("..\\result.cdt");

	//CGroupRes groupRes;
	//groupRes.ReadFromFile("..\\FinalResult.cdt");

	//std::ofstream fout("..\\Ids.txt");
	//for (ulong i = 0; i < groupRes.GetGroups().Size(); ++i)
	//{
	//	for (ulong j = 0; j < groupRes.GetGroups()[i].DfaIds.Size(); ++j)
	//	{
	//		fout << groupRes.GetGroups()[i].DfaIds[j] << " ";
	//	}
	//	fout << std::endl;
	//}
	//fout.clear();
	//fout.close();

	//std::ofstream foutMerge("..\\MergeId.txt");
	//for (ulong i = 0; i < groupRes.GetGroups().Size(); ++i)
	//{
	//	foutMerge << groupRes.GetDfaTable()[groupRes.GetGroups()[i].mergeDfaId].GetFinalStates().CountDfaIds() << std::endl;
	//}
	//foutMerge.clear();
	//foutMerge.close();

	CTimer ctime;
	CCompileResults result;
	try
	{
		CompileRuleFile("..\\allrules.rule", result);
	}
	catch (CTrace &e)
	{
		std::cout << e.File() << " - " << e.Line() << ": " << e.What() << std::endl;
		system("pause");
	}
	result.WriteToFile("..\\result.cdt");

	//result.ReadFromFile("..\\result.cdt");

	CGroupRes groupRes;
	Grouping(result, groupRes);
	groupRes.WriteToFile("..\\GroupResult.cdt");
	//groupRes.ReadFromFile("..\\GroupResult.cdt");
	HASHRES HashResMap;
	HashMapping(groupRes, HashResMap);
	groupRes.WriteToFile("..\\FinalResult.cdt");
	std::cout << "Total time: " << ctime.Reset() << std::endl;
	std::cout << groupRes.GetGroups().Size() << std::endl;
	std::cout << HashResMap.size() << std::endl;

	//CCompileResults result;
	//result.ReadFromFile("..\\result.cdt");
	//std::ifstream fin("..\\Ids.txt");
	//std::ifstream finMerge("..\\MergeId.txt");
	//std::vector<size_t> mergeDfaSize;
	//size_t id;
	//while (finMerge >> id)
	//{
	//	mergeDfaSize.push_back(id);
	//}

	//size_t count = 0;
	//std::string str;
	//while (std::getline(fin, str))
	//{
	//	//std::cout << count << std::endl;
	//	std::stringstream ss(str);
	//	std::vector<size_t> vecIds;
	//	while (ss >> id)
	//	{
	//		vecIds.push_back(id);
	//	}

	//	CDfaArray vecDfas;
	//	vecDfas.Resize(2);
	//	vecDfas[0] = result.GetDfaTable()[vecIds[0]];
	//	for (ulong i = 1; i < vecIds.size(); ++i)
	//	{
	//		vecDfas[1] = result.GetDfaTable()[vecIds[i]];
	//		CDfa MergeDfa;
	//		if (!MergeMultipleDfas(vecDfas, MergeDfa))
	//		{
	//			std::cout << "error" << std::endl;
	//		}
	//		vecDfas[0] = MergeDfa;
	//	}
	//	if (vecDfas[0].GetFinalStates().CountDfaIds() != mergeDfaSize[count++])
	//	{
	//		std::cout << "error" << std::endl;
	//	}
	//}

	//std::ifstream fin("..\\allrules.rule");
	//std::vector<std::string> vecRules;
	//std::string str;
	//while (std::getline(fin, str))
	//{
	//	vecRules.push_back(str);
	//}
	//fin.clear();
	//fin.close();

	//CGroupRes groupRes;
	//groupRes.ReadFromFile("..\\FinalResult.cdt");
	//std::ofstream fout("..\\CanCompile.rule");
	//for (ulong i = 0; i < groupRes.GetSidDfaIds().Size(); ++i)
	//{
	//	if (groupRes.GetSidDfaIds()[i].m_nResult == COMPILEDINFO::RES_SUCCESS)
	//	{
	//		str = "sid:";
	//		std::string sid;
	//		std::stringstream ss;
	//		ss << groupRes.GetSidDfaIds()[i].m_nSid;
	//		ss >> sid;
	//		str += sid;
	//		str += ";";
	//		for (ulong j = 0; j < vecRules.size(); ++j)
	//		{
	//			if (vecRules[j].find(str, 0) != std::string::npos)
	//			{
	//				fout << vecRules[j] << std::endl;
	//			}
	//		}
	//	}
	//}
	//fout.clear();
	//fout.close();

	system("pause");
	return 0;
}