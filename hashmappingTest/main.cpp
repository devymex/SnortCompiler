#include <iostream>
#include <fstream>
#include <tchar.h>
#include "../common/common.h"
#include "../dfanew/dfanew.h"
#include "../compilernew/compilernew.h"
#include "../hashmapping/hashmapping.h"
#include "../grouping/grouping.h"

void main()
{
	//CGROUPRes groupRes;
	//HASHRES HashResMap;
	//groupRes.ReadFromFile(_T("..\\..\\output\\GroupResut.cdt"));
	//HashMapping(groupRes, HashResMap);
	//groupRes.WriteToFile(_T("..\\..\\output\\FinalResut.cdt"));

	//groupRes.ReadFromFile(_T("..\\..\\output\\FinalResut.cdt"));
	//for (size_t i = 0; i < groupRes.GetGroups().Size(); ++i)
	//{
	//	ONEGROUP &group = groupRes.GetGroups()[i];
	//	HashResMap[hash(group.currSig)].push_back(HASHNODE(group.currSig, group.mergeDfaId));
	//}
	
	//std::ofstream fout("..//..//output//test.txt");
	//for (size_t i = 0; i < groupRes.GetGroups().Size(); ++i)
	//{
	//	for (size_t j = 0; j < groupRes.GetGroups()[i].DfaIds.Size(); ++j)
	//	{
	//		fout << groupRes.GetGroups()[i].DfaIds[j] << " ";
	//	}
	//	fout << "\t" << groupRes.GetDfaTable()[groupRes.GetGroups()[i].mergeDfaId].Size() << "\t" << groupRes.GetGroups()[i].currSig << std::endl;
	//}
	//fout.clear();
	//fout.close();

	CTimer ctime;
	CResNew result;
	compilenew(_T("..\\..\\input\\allrules.rule"), result);
	result.WriteToFile(_T("..\\..\\output\\result.cdt"));
	//result.ReadFromFile(_T("..\\..\\output\\result.cdt"));
	CGROUPRes groupRes;
	grouping(result, groupRes);
	groupRes.WriteToFile(_T("..\\..\\output\\GroupResut.cdt"));
	HASHRES HashResMap;
	HashMapping(groupRes, HashResMap);
	groupRes.WriteToFile(_T("..\\..\\output\\FinalResut.cdt"));
	std::cout << "×ÜÊ±¼ä£º " << ctime.Reset() << std::endl;
	std::cout << groupRes.GetGroups().Size() << std::endl;
	std::cout << HashResMap.size() << std::endl;

	//CGROUPRes oldres;
	//CGROUPRes newres;
	//oldres.ReadFromFile(_T("..\\..\\output\\GroupResut1.cdt"));
	//newres.ReadFromFile(_T("..\\..\\output\\GroupResut.cdt"));
	//for (size_t i = 0; i < oldres.GetDfaTable().Size(); ++i)
	//{
	//	if (oldres.GetDfaTable()[i].GetFinalState().GetAllDfaIdCount() != newres.GetDfaTable()[i].GetFinalState().GetAllDfaIdCount())
	//	{
	//		CDfaNew olddfa = oldres.GetDfaTable()[i];
	//		CDfaNew newdfa = newres.GetDfaTable()[i];
	//		for (size_t j = 0; j < oldres.GetDfaTable()[i].GetFinalState().GetAllDfaIdCount(); ++j)
	//		{
	//			STATEID nStaId = oldres.GetDfaTable()[i].GetFinalState()[j];
	//			CVectorUnsigned ids;
	//			oldres.GetDfaTable()[i].GetFinalState().GetDfaIds(nStaId, ids);
	//			for (size_t k = 0; k < ids.Size(); ++k)
	//			{
	//				std::cout << (size_t)nStaId << " " << ids[k] << std::endl;
	//			}
	//		}
	//		std::cout << std::endl;
	//		for (size_t j = 0; j < newres.GetDfaTable()[i].GetFinalState().GetAllDfaIdCount(); ++j)
	//		{
	//			STATEID nStaId = newres.GetDfaTable()[i].GetFinalState()[j];
	//			CVectorUnsigned ids;
	//			newres.GetDfaTable()[i].GetFinalState().GetDfaIds(nStaId, ids);
	//			for (size_t k = 0; k < ids.Size(); ++k)
	//			{
	//				std::cout << (size_t)nStaId << " " << ids[k] << std::endl;
	//			}
	//		}
	//		std::cout << "error" << std::endl;
	//		break;
	//	}
	//}

	system("pause");
}
