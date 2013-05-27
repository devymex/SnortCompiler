#include "stdafx.h"
#include "hashmapping.h"
#include "../grouping/grouping.h"
#include "../compilernew/compilernew.h"
#include "../dfanew/dfanew.h"
#include "../mergedfanew/MergeDfanew.h"

void Merge(CGROUPRes &groupRes)
{
	std::set<SIGNATURE> Sigs;
	std::map<SIGNATURE, std::vector<size_t>> SigToIdsMap;
	for (size_t i = 0; i < groupRes.GetGroups().Size(); ++i)
	{
		if (groupRes.GetGroups()[i].vecSigs.size() == 1)
		{
			Sigs.insert(groupRes.GetGroups()[i].vecSigs[0]);
			SigToIdsMap[groupRes.GetGroups()[i].vecSigs[0]].push_back(i);
		}
	}
	std::vector<size_t> vecDeleteIds;
	for (size_t i = 0; i < groupRes.GetGroups().Size(); ++i)
	{
		if (groupRes.GetGroups()[i].vecSigs.size() > 1)
		{
			bool flag = true;
			for (size_t j = 0; j < groupRes.GetGroups()[i].vecSigs.size(); ++j)
			{
				if (std::find(Sigs.begin(), Sigs.end(), groupRes.GetGroups()[i].vecSigs[j]) == Sigs.end())
				{
					flag = false;
					break;
				}
			}
			if (flag)
			{
				for (size_t j = 0; j < groupRes.GetGroups()[i].vecSigs.size(); ++j)
				{
					flag = false;
					for (size_t k = 0; k < SigToIdsMap[groupRes.GetGroups()[i].vecSigs[j]].size(); ++k)
					{
						if (groupRes.GetGroups()[SigToIdsMap[groupRes.GetGroups()[i].vecSigs[j]][k]].vecSigs[0] == groupRes.GetGroups()[i].vecSigs[j])
						{
							std::vector<CDfanew> vecDfas;
							vecDfas.push_back(groupRes.GetDfaTable()[groupRes.GetGroups()[i].dfaId]);
							vecDfas.push_back(groupRes.GetDfaTable()[groupRes.GetGroups()[SigToIdsMap[groupRes.GetGroups()[i].vecSigs[j]][k]].dfaId]);
							CDfanew MergeDfa;
							if (NOrMerge(vecDfas, MergeDfa))
							{
								flag = true;
								groupRes.GetDfaTable().PushBack(MergeDfa);
								groupRes.GetGroups()[SigToIdsMap[groupRes.GetGroups()[i].vecSigs[j]][k]].dfaId = groupRes.GetDfaTable().Size() - 1;
								vecDeleteIds.push_back(i);
								break;
							}
						}
					}
					if (flag)
					{
						break;
					}
				}
			}
		}
	}
	CGROUPS newGroups = groupRes.GetGroups();
	groupRes.GetGroups().Clear();
	for (size_t i = 0; i < newGroups.Size(); ++i)
	{
		if (std::find(vecDeleteIds.begin(), vecDeleteIds.end(), i) != vecDeleteIds.end())
		{
			continue;
		}
		groupRes.GetGroups().PushBack(newGroups[i]);
	}
}

size_t hash(const SIGNATURE &oneSig)
{
	return oneSig % 16001;
}

HASHMAPPINGSC void Mapping()
{
	CGROUPRes groupRes;
	groupRes.ReadFromFile(_T("..\\..\\output\\GroupResut.cdt"));
	groupRes.GetGroups().Sort();
	Merge(groupRes);

	std::ofstream fout("..\\..\\output\\test.txt");
	for (size_t i = 0; i < groupRes.GetGroups().Size(); ++i)
	{
		if (groupRes.GetGroups()[i].vecSigs.size() == 2)
		{
			fout << groupRes.GetGroups()[i].vecSigs[0] << "\t" << groupRes.GetGroups()[i].vecSigs[1] << std::endl;
		}
	}
	fout.clear();
	fout.close();
	//std::vector<GROUPHASH> groups;
	//for (size_t i = 0; i < groupRes.GetGroups().Size(); ++i)
	//{
	//	groups.push_back(GROUPHASH());
	//	GROUPHASH &oneGroup = groups.back();
	//	oneGroup.dfaId = groupRes.GetGroups()[i].dfaId;
	//	oneGroup.vecSigs = groupRes.GetGroups()[i].vecSigs;
	//}

	//CDfaTblNew &dfaTbl = groupRes.GetDfaTable();
	//
	//std::map<size_t, std::vector<size_t>> hashMap;
	//for (size_t i = 0; i < groups.size(); ++i)
	//{
	//	std::cout << i << std::endl;
	//	if (groups[i].vecSigs.size() > 2)
	//	{
	//		break;
	//	}
	//	bool flag = false;
	//	size_t min = groups.size();
	//	size_t idx;
	//	for (size_t j = 0; j < groups[i].vecSigs.size(); ++j)
	//	{
	//		SIGNATURE &oneSig = groups[i].vecSigs[j];
	//		if (hashMap[hash(oneSig)].empty())
	//		{
	//			hashMap[hash(oneSig)].push_back(i);
	//			groups[i].currSig = oneSig;
	//			flag = true;
	//			break;
	//		}
	//		else
	//		{
	//			for (size_t k = 0; k < hashMap[hash(oneSig)].size(); ++k)
	//			{
	//				if (groups[hashMap[hash(oneSig)][k]].currSig == oneSig)
	//				{
	//					std::vector<CDfanew> vecDfas;
	//					vecDfas.push_back(dfaTbl[groups[i].dfaId]);
	//					vecDfas.push_back(dfaTbl[groups[hashMap[hash(oneSig)][k]].dfaId]);
	//					CDfanew MergeDfa;
	//					if (NOrMerge(vecDfas, MergeDfa))
	//					{
	//						dfaTbl.PushBack(MergeDfa);
	//						UpdateComSigs(groups[hashMap[hash(oneSig)][k]].vecSigs, groups[i].vecSigs);
	//						groups[hashMap[hash(oneSig)][k]].dfaId = dfaTbl.Size() - 1;
	//						flag = true;
	//						break;
	//					}
	//				}
	//			}
	//			if (flag)
	//			{
	//				break;
	//			}
	//			else
	//			{
	//				if (min > hashMap[hash(oneSig)].size())
	//				{
	//					min = hashMap[hash(oneSig)].size();
	//					idx = j;
	//				}
	//			}
	//		}
	//	}
	//	if (!flag)
	//	{
	//		hashMap[hash(groups[i].vecSigs[idx])].push_back(i);
	//		groups[i].currSig = groups[i].vecSigs[idx];
	//	}
	//}
	//std::cout << hashMap.size() << std::endl;
}
