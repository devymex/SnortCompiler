#include <iostream>
#include <fstream>
#include <algorithm>
#include <sstream>
#include <tchar.h>

#include <hwprj\ctimer.h>
#include <hwprj\unsary.h>
#include <hwprj\compiler.h>
#include <hwprj\compres.h>
#include <hwprj\groupres.h>
#include <hwprj\grouping.h>
#include <hwprj\buildhash.h>
#include <hwprj\trace.h>

int main()
{
	CTimer ctime;
	CCompileResults result;
	CompileRuleFile("..\\allrules.rule", result);
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

	//std::ifstream fin("..\\Ids.txt");
	//std::vector<size_t> vecIds;
	//size_t id;
	//while (fin >> id)
	//{
	//	vecIds.push_back(id);
	//}

	//CCompileResults result;
	//result.ReadFromFile("..\\result.cdt");

	//CDfaArray vecDfas;
	//vecDfas.Resize(2);
	//vecDfas[0] = result.GetDfaTable()[vecIds[0]];
	//for (ulong i = 1; i < vecIds.size(); ++i)
	//{
	//	vecDfas[1] = result.GetDfaTable()[vecIds[i]];
	//	CDfa MergeDfa;
	//	MergeMultipleDfas(vecDfas, MergeDfa);
	//	vecDfas[0] = MergeDfa;
	//}
	//CFinalStates finalState = vecDfas[0].GetFinalState();

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