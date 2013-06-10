#include <iostream>
#include <fstream>
#include <tchar.h>
#include <hwprj\common.h>
#include <hwprj\hashmapping.h>

void main()
{
	//CGROUPRes groupRes;
	//HASHRES HashResMap;
	//groupRes.ReadFromFile(_T("..\\..\\output\\GroupResut.cdt"));
	//HashMapping(groupRes, HashResMap);
	//groupRes.WriteToFile(_T("..\\..\\output\\FinalResut.cdt"));

	//groupRes.ReadFromFile(_T("..\\..\\output\\FinalResut.cdt"));
	//for (ULONG i = 0; i < groupRes.GetGroups().Size(); ++i)
	//{
	//	ONEGROUP &group = groupRes.GetGroups()[i];
	//	HashResMap[hash(group.currSig)].push_back(HASHNODE(group.currSig, group.mergeDfaId));
	//}
	
	//std::ofstream fout("..//..//output//test.txt");
	//for (ULONG i = 0; i < groupRes.GetGroups().Size(); ++i)
	//{
	//	for (ULONG j = 0; j < groupRes.GetGroups()[i].DfaIds.Size(); ++j)
	//	{
	//		fout << groupRes.GetGroups()[i].DfaIds[j] << " ";
	//	}
	//	fout << "\t" << groupRes.GetDfaTable()[groupRes.GetGroups()[i].mergeDfaId].Size() << "\t" << groupRes.GetGroups()[i].currSig << std::endl;
	//}
	//fout.clear();
	//fout.close();

	CTimer ctime;
	CCompileResults result;
	//compilenew(_T("..\\..\\input\\allrules.rule"), result);
	//result.WriteToFile(_T("..\\..\\output\\result.cdt"));
	result.ReadFromFile(_T("..\\..\\output\\result.cdt"));
	CGROUPRes groupRes;
	Grouping(result, groupRes);
	groupRes.WriteToFile(_T("..\\..\\output\\GroupResut.cdt"));
	HASHRES HashResMap;
	HashMapping(groupRes, HashResMap);
	groupRes.WriteToFile(_T("..\\..\\output\\FinalResut.cdt"));
	std::cout << "×ÜÊ±¼ä£º " << ctime.Reset() << std::endl;
	std::cout << groupRes.GetGroups().Size() << std::endl;
	std::cout << HashResMap.size() << std::endl;

	system("pause");
}
