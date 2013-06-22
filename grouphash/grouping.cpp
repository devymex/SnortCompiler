/**
**	@file		grouping.cpp
**
**	@author		Lab 435, Xidian University
**
**	@brief		Support functions for grouping dfa
**
**	This implements grouping dfa algorithm, the algorithm has four steps
**	in general. At first, it groups dfa which has only one signature. Then, 
**	push the remain dfa into the group. Thirdly, it groups dfa with the same 
**	signatures. Finally, it merges two groups.
**
*/

#include "stdafx.h"
#include <hwprj\groups.h>
#include <hwprj\groupres.h>
#include <hwprj\grouping.h>

/* Extract signatures from res to vecDfaInfo and add all index to vecWaitForGroup

Arguments:
  res					the compile result
  vecDfaInfo		signatures correspond to each dfa
  vecWaitForGroup	the index of dfa waiting for grouping

Returns:				nothing

*/

void ExtractSigsVec(const CCompileResults &res, std::vector<CUnsignedArray> &SigsVec, std::vector<ulong> &vecWaitForGroup)
{
	ulong nSize = res.GetRegexTbl().Size();
	SigsVec.resize(nSize);
	for (ulong i = 0; i < nSize; ++i)
	{
		vecWaitForGroup.push_back(i);
		for (ulong j = 0; j < res.GetRegexTbl()[i].GetSigs().Size(); ++j)
		{
			SigsVec[i].PushBack(res.GetRegexTbl()[i].GetSigs()[j]);
		}
	}
}

/* group dfa which has only one signature by its only signature

Arguments:
  vecDfaInfo		signatures correspond to each dfa
  vecWaitForGroup	the index of dfa waiting for grouping
  groups				the group result

Returns:				nothing

*/

void GroupOnlyOneSig(const std::vector<CUnsignedArray> &SigsVec, std::vector<ulong> &vecWaitForGroup, CGroups &groups)
{
	std::map<SIGNATURE, CUnsignedArray> sigToIdsMap;
	for (std::vector<ulong>::iterator i = vecWaitForGroup.begin(); i != vecWaitForGroup.end();)
	{
		if (SigsVec[*i].Size() == 1)
		{
			sigToIdsMap[SigsVec[*i][0]].PushBack(*i);

			//update the index of dfa waiting for grouping
			i = vecWaitForGroup.erase(i);
		}
		else
		{
			++i;
		}
	}

	groups.Resize(sigToIdsMap.size());
	ulong idx = 0;
	for (std::map<SIGNATURE, CUnsignedArray>::iterator i = sigToIdsMap.begin(); i != sigToIdsMap.end(); ++i, ++idx)
	{
		groups[idx].ComSigs.PushBack(i->first);
		groups[idx].DfaIds = i->second;
		groups[idx].mergeDfaId = ulong(-1);
	}
}

/* try to merge dfa in one group, merge if success, otherwise seperate the group into two

Arguments:
  res					the compile result, the merged dfa will be pushed into the res's dfa table
  groups				the group result

Returns:				nothing

*/

struct DFASIZECOMP
{
	const CCompileResults *m_pRes;
	DFASIZECOMP(const CCompileResults *pRes) : m_pRes(pRes) {}
	bool operator()(ulong idx1, ulong idx2)
	{
		return m_pRes->GetDfaTable()[idx1].Size() < m_pRes->GetDfaTable()[idx2].Size();
	}
};

void DfaSizeSort(CCompileResults &res, CGroups &groups)
{
	for (ulong i = 0; i < groups.Size(); ++i)
	{
		ulong *pBeg = groups[i].DfaIds.Data();
		std::sort(pBeg, pBeg + groups[i].DfaIds.Size(), DFASIZECOMP(&res));
	}
}

void Merge(CCompileResults &res, CGroups &groups)
{
	for (ulong i = 0; i < groups.Size(); ++i)
	{
		g_log << "Merge " << g_log.nl;
		g_log << "NO: " << i << g_log.nl;
		g_log << "Total: " << groups.Size() << g_log.nl << g_log.nl;
		groups[i].mergeDfaId = groups[i].DfaIds[0];

		//not need merge for group with only one dfa
		if (groups[i].DfaIds.Size() == 1)
		{
			continue;
		}
		CDfaArray vecDfas;
		vecDfas.Resize(2);
		CDfa MergeDfa;
		MergeDfa.SetId(res.GetDfaTable().Size());
		vecDfas[0] = res.GetDfaTable()[groups[i].DfaIds[0]];
		
		//flag if all dfas in the group can merge together
		bool mergeFlag = true;
		for (ulong j = 1; j < groups[i].DfaIds.Size(); ++j)
		{
			vecDfas[1] = res.GetDfaTable()[groups[i].DfaIds[j]];
			if (!MergeMultipleDfas(vecDfas, MergeDfa))
			{
				mergeFlag = false;

				//if first merge is success, it indicates that a new merged dfa need push into res's dfa table
				if (j != 1)
				{
					res.GetDfaTable().PushBack(vecDfas[0]);
					groups[i].mergeDfaId = res.GetDfaTable().Size() - 1;
				}
				groups.PushBack(ONEGROUP());
				for (ulong k = j; k < groups[i].DfaIds.Size(); ++k)
				{
					groups.Back().DfaIds.PushBack(groups[i].DfaIds[k]);
				}
				groups.Back().mergeDfaId = ulong(-1);
				groups.Back().ComSigs = groups[i].ComSigs;
				CUnsignedArray tmp(groups[i].DfaIds);
				groups[i].DfaIds.Clear();
				for (ulong k = 0; k < j; ++k)
				{
					groups[i].DfaIds.PushBack(tmp[k]);
				}
				break;
			}
			else
			{
				vecDfas[0] = MergeDfa;
			}
		}

		//all dfas in the group can merge together
		if (mergeFlag)
		{
			res.GetDfaTable().PushBack(MergeDfa);
			groups[i].mergeDfaId = res.GetDfaTable().Size() - 1;
		}
	}
}

/* put dfa waiting for grouping into a group, if the dfa has the group's signature and 
can merge with the group's dfa merged before

Arguments:
  vecDfaInfo		signatures correspond to each dfa
  res					the compile result, the merged dfa will be pushed into the res's dfa table
  groups				the group result
  vecWaitForGroup	the index of dfa waiting for grouping

Returns:				nothing

*/

void PutInBySig(const std::vector<CUnsignedArray> &SigsVec, CCompileResults &res, CGroups &groups, std::vector<ulong> &vecWaitForGroup)
{
	std::map<SIGNATURE, std::vector<ulong>> sigToGroupsMap;
	ulong idx = 0;
	for (ulong i = 0; i < groups.Size(); ++i, ++idx)
	{
		sigToGroupsMap[groups[i].ComSigs[0]].push_back(idx);
	}

	idx = 0;
	for (std::map<SIGNATURE, std::vector<ulong>>::iterator i = sigToGroupsMap.begin(); i != sigToGroupsMap.end(); ++i, ++idx)
	{
		g_log << "PutInBySig " << g_log.nl;
		g_log << "NO: " << idx << g_log.nl;
		g_log << "Total: " << sigToGroupsMap.size() << g_log.nl << g_log.nl;
		for (std::vector<ulong>::iterator j = i->second.begin(); j != i->second.end(); ++j)
		{
			CDfaArray vecDfas;
			vecDfas.Resize(2);
			vecDfas[0] = res.GetDfaTable()[groups[*j].mergeDfaId];
			for (std::vector<ulong>::iterator k = vecWaitForGroup.begin(); k != vecWaitForGroup.end(); )
			{

				//the dfa doesn't have the group's signature
				if (SigsVec[*k].Find(i->first) == ulong(-1))
				{
					++k;
					continue;
				}
				vecDfas[1] = res.GetDfaTable()[*k];
				CDfa MergeDfa;
				if (MergeMultipleDfas(vecDfas, MergeDfa))
				{
					res.GetDfaTable().PushBack(MergeDfa);
					groups[*j].DfaIds.PushBack(*k);
					groups[*j].mergeDfaId = res.GetDfaTable().Size() - 1;
					vecDfas[0] = MergeDfa;

					//update the index of dfa waiting for grouping
					k = vecWaitForGroup.erase(k);
				}
				else
				{
					++k;
				}
			}
		}
	}
}

/* group dfa which has the same signatures by its signatures

Arguments:
  vecDfaInfo		signatures correspond to each dfa
  newGroups			the new group result
  vecWaitForGroup	the index of dfa waiting for grouping

Returns:				nothing

*/

void BuildGroupBySig(const std::vector<CUnsignedArray> &SigsVec, CGroups &newGroups, std::vector<ulong> &vecWaitForGroup)
{
	std::map<CUnsignedArray, CUnsignedArray> sigsToIdsMap;
	for (ulong i = 0; i < vecWaitForGroup.size(); ++i)
	{
		sigsToIdsMap[SigsVec[vecWaitForGroup[i]]].PushBack(vecWaitForGroup[i]);
	}
	vecWaitForGroup.clear();

	newGroups.Resize(sigsToIdsMap.size());
	ulong idx = 0;
	for (std::map<CUnsignedArray, CUnsignedArray>::iterator i = sigsToIdsMap.begin(); i != sigsToIdsMap.end(); ++i, ++idx)
	{
		for (ulong j = 0; j < i->first.Size(); ++j)
		{
			newGroups[idx].ComSigs.PushBack(i->first[j]);
		}
		newGroups[idx].DfaIds = i->second;
		newGroups[idx].mergeDfaId = ulong(-1);
	}
}

/* extract signatures from group with only one signature

Arguments:
  groups				the group result, each group has only one signature
  vecUsed			the signatures which are used by group with only one signature

Returns:				nothing

*/

void ExtractUsedSigs(const CGroups &groups, std::vector<SIGNATURE> &vecUsed)
{
	for (ulong i = 0; i < groups.Size(); ++i)
	{
		vecUsed.push_back(groups[i].ComSigs[0]);
	}
	std::sort(vecUsed.begin(), vecUsed.end());
	vecUsed.erase(std::unique(vecUsed.begin(), vecUsed.end()), vecUsed.end());
}

/* extract common signatures from two groups

Arguments:
  g1					 the first group
  g2					 the second group
  vecUsed			the signatures which are used by group with only one signature
  vecComSigs		common signatures of the two groups

Returns:				nothing

*/

void ExtractComSigs(const ONEGROUP &g1, const ONEGROUP &g2, const std::vector<SIGNATURE> &vecUsed, std::vector<SIGNATURE> &vecComSigs)
{
	std::map<SIGNATURE, ulong> sigToNumMap;
	for (ulong i = 0; i < g1.ComSigs.Size(); ++i)
	{
		++sigToNumMap[g1.ComSigs[i]];
	}
	for (ulong i = 0; i < g2.ComSigs.Size(); ++i)
	{
		++sigToNumMap[g2.ComSigs[i]];
	}

	for (std::map<SIGNATURE, ulong>::iterator i = sigToNumMap.begin(); i != sigToNumMap.end(); ++i)
	{

		//the two group both have the signature and the signature is not contained in vecUsed
		if (i->second == 2 && std::find(vecUsed.begin(), vecUsed.end(), i->first) == vecUsed.end())
		{
			vecComSigs.push_back(i->first);
		}
	}
}

/* the used upper limit of the signatures

Arguments:
  vecSigs			signatures

Returns:				the used upper limit of the signatures

*/

ulong AvailableNum(const std::vector<SIGNATURE> &vecSigs)
{
	if (vecSigs.size() <= 2)
	{
		return 0;
	}
	else
	{
		return vecSigs.size() - 3;
	}
}

/* try to merge two groups, merge if the two group have common signatures , the used number 
of the common signatures isn't exceed its upper limit and the two groups merged dfa can merge

Arguments:
  res					the compile result, the merged dfa will be pushed into the res's dfa table
  vecUsed			the signatures which can not be used
  newGroups			the new group result

Returns:				nothing

*/

void MergeGroup(CCompileResults &res, std::vector<SIGNATURE> &vecUsed, CGroups &newGroups)
{
	std::map<std::vector<SIGNATURE>, ulong> SigsToNumMap;
	for (ulong i = 0; i < newGroups.Size(); ++i)
	{
		std::vector<SIGNATURE> vecSigs;
		for (ulong j = 0; j < newGroups[i].ComSigs.Size(); ++j)
		{
			if (std::find(vecUsed.begin(), vecUsed.end(), newGroups[i].ComSigs[j]) == vecUsed.end())
			{
				vecSigs.push_back(newGroups[i].ComSigs[j]);
			}
		}
		if (vecSigs.size() == 1)
		{
			vecUsed.push_back(vecSigs[0]);
		}
		else
		{
			++SigsToNumMap[vecSigs];
		}
	}
	for (ulong i = 0; i < newGroups.Size(); ++i)
	{
		g_log << "MergeGroup " << g_log.nl;
		g_log << "NO: " << i << g_log.nl;
		g_log << "Total: " << newGroups.Size() << g_log.nl << g_log.nl;
		std::vector<SIGNATURE> vecSigs;
		for (ulong j = 0; j < newGroups[i].ComSigs.Size(); ++j)
		{
			if (std::find(vecUsed.begin(), vecUsed.end(), newGroups[i].ComSigs[j]) == vecUsed.end())
			{
				vecSigs.push_back(newGroups[i].ComSigs[j]);
			}
		}
		//++SigsToNumMap[vecSigs];
		CDfaArray vecDfas;
		vecDfas.Resize(2);
		vecDfas[0] = res.GetDfaTable()[newGroups[i].mergeDfaId];
		for (; ;)
		{
			ulong idx = 0;
#undef min
			int nMax = std::numeric_limits<int>::min();
			for (ulong j = i + 1; j < newGroups.Size(); ++j)
			{
				std::vector<SIGNATURE> vecComSigs;
				ExtractComSigs(newGroups[i], newGroups[j], vecUsed, vecComSigs);
				if (SigsToNumMap[vecComSigs] >= AvailableNum(vecComSigs))
				{
					continue;
				}
				if (vecComSigs.size() > 1)
				{
					vecDfas[1] = res.GetDfaTable()[newGroups[j].mergeDfaId];
					CDfa MergeDfa;
					if (MergeMultipleDfas(vecDfas, MergeDfa))
					{
						//select the fittest group to merge
						int nReduceSize = vecDfas[0].Size() + vecDfas[1].Size() - MergeDfa.Size();
						if (nMax < nReduceSize)
						{
							nMax = nReduceSize;
							idx = j;
						}
					}
				}
			}

			//no group can merge with the current group
			if (idx == 0)
			{
				break;
			}
			else
			{
				std::vector<SIGNATURE> vecComSigs;
				ExtractComSigs(newGroups[i], newGroups[idx], vecUsed, vecComSigs);
				vecDfas[1] = res.GetDfaTable()[newGroups[idx].mergeDfaId];
				CDfa MergeDfa;
				if (MergeMultipleDfas(vecDfas, MergeDfa))
				{
					if (SigsToNumMap[vecSigs] > 0)
					{
						if (SigsToNumMap[vecSigs] == AvailableNum(vecSigs))
						{
							for (ulong k = 0; k < vecSigs.size(); ++k)
							{
								std::vector<SIGNATURE>::iterator iter = std::find(vecUsed.begin(), vecUsed.end(), vecSigs[k]);
								if (iter != vecUsed.end())
								{
									vecUsed.erase(iter);
								}
							}
						}
						--SigsToNumMap[vecSigs];
					}
					++SigsToNumMap[vecComSigs];
					if (SigsToNumMap[vecComSigs] == AvailableNum(vecComSigs))
					{
						vecUsed.insert(vecUsed.end(), vecComSigs.begin(), vecComSigs.end());
						std::sort(vecUsed.begin(), vecUsed.end());
						vecUsed.erase(std::unique(vecUsed.begin(), vecUsed.end()), vecUsed.end());
					}
					for (ulong k = 0; k < newGroups[idx].DfaIds.Size(); ++k)
					{
						newGroups[i].DfaIds.PushBack(newGroups[idx].DfaIds[k]);
					}
					newGroups[i].ComSigs.Clear();
					for (ulong k = 0; k < vecComSigs.size(); ++k)
					{
						newGroups[i].ComSigs.PushBack(vecComSigs[k]);
					}
					vecSigs = vecComSigs;
					res.GetDfaTable().PushBack(MergeDfa);
					newGroups[i].mergeDfaId = res.GetDfaTable().Size() - 1;
					newGroups.Erase(idx);
					vecDfas[0] = MergeDfa;
				}
				else
				{
					g_log << "ERROR" << g_log.nl;
					system("pause");
				}
			}
		}
	}
}

/* add new group result to group result

Arguments:
  groups				the group result
  newGroups			the new group result

Returns:				nothing

*/

void AddNewGroups(CGroups &newGroups, CGroups &groups)
{
	for (ulong i = 0; i < newGroups.Size(); ++i)
	{
		groups.PushBack(newGroups[i]);
	}
	newGroups.Clear();
}

/* clear up the group result

Arguments:
  res					the compile result and its dfa table contains the merged dfas
  groups				the group result
  groupRes			 the relationship between sid and dfa ids, dfa table and result of grouping

Returns:				nothing

*/

void ClearUpRes(CCompileResults &res, const CGroups &groups, CGroupRes &groupRes)
{
	groupRes.GetSidDfaIds() = res.GetSidDfaIds();
	std::vector<ulong> occurred(res.GetDfaTable().Size(), 0);
	for (ulong i = 0; i < groups.Size(); ++i)
	{
		occurred[groups[i].mergeDfaId] = 1;
	}
	std::map<ulong, ulong> oldToNewMap;
	ulong count = 0;
	ulong idx = 0;
	for (std::vector<ulong>::iterator i = occurred.begin(); i != occurred.end(); ++i, ++idx)
	{
		if (*i == 1)
		{
			groupRes.GetDfaTable().PushBack(res.GetDfaTable()[idx]);
			oldToNewMap[idx] = count;
			++count;
		}
		else
		{
			res.GetDfaTable()[idx].Clear();
		}
	}
	groupRes.GetGroups().Resize(groups.Size());
	for (ulong i = 0; i < groups.Size(); ++i)
	{
		groupRes.GetGroups()[i].DfaIds = groups[i].DfaIds;
		groupRes.GetGroups()[i].ComSigs = groups[i].ComSigs;
		groupRes.GetGroups()[i].mergeDfaId = oldToNewMap[groups[i].mergeDfaId];
	}

	std::vector<SIGNATURE> vecSpecialSigs;
	for (ulong i = 0; i < groupRes.GetGroups().Size(); ++i)
	{
		if (groupRes.GetGroups()[i].ComSigs.Size() == 1)
		{
			vecSpecialSigs.push_back(groupRes.GetGroups()[i].ComSigs[0]);
		}
	}
	for (ulong i = 0; i < groupRes.GetGroups().Size(); ++i)
	{
		if (groupRes.GetGroups()[i].ComSigs.Size() >= 2)
		{
			std::vector<SIGNATURE> vecSigs;
			for (ulong j = 0; j < groupRes.GetGroups()[i].ComSigs.Size(); ++j)
			{
				vecSigs.push_back(groupRes.GetGroups()[i].ComSigs[j]);
			}
			for (std::vector<SIGNATURE>::iterator j = vecSigs.begin(); j != vecSigs.end();)
			{
				if (std::find(vecSpecialSigs.begin(), vecSpecialSigs.end(), *j) != vecSpecialSigs.end())
				{
					j = vecSigs.erase(j);
				}
				else
				{
					++j;
				}
			}
			if (!vecSigs.empty())
			{
				groupRes.GetGroups()[i].ComSigs.Clear();
				for (std::vector<SIGNATURE>::iterator j = vecSigs.begin(); j != vecSigs.end(); ++j)
				{
					groupRes.GetGroups()[i].ComSigs.PushBack(*j);
				}
			}
		}
	}
}

void outPutGroups(CGroupRes &groupRes, const char* fileName)
{
	std::ofstream fout(fileName);
	if(!fout)
	{
		std::cerr << "open file failure!" << g_log.nl;
		return;
	}
	fout << "groupNumber\t" << "MergeId\t" << "origDfaId" << g_log.nl; 
	for(ulong i = 0; i < groupRes.GetGroups().Size(); ++i)
	{
		fout << i << "\t";
		fout << groupRes.GetGroups()[i].mergeDfaId << "\t";
		for(ulong j = 0; j < groupRes.GetGroups()[i].DfaIds.Size(); ++j)
		{
			fout << groupRes.GetGroups()[i].DfaIds[j] << g_log.nl << "\t" << "\t";
		}
		fout << g_log.nl;
	}
	fout.close();
}

/* the grouping algorithm

Arguments:
  res					the compile result
  groupRes			 the relationship between sid and dfa ids, dfa table and result of grouping

Returns:				nothing

*/

GROUPINGHDR void Grouping(CCompileResults &res, CGroupRes &groupRes)
{
	CTimer t1, tAll;

	g_log << "Extract Dfa's information..." << g_log.nl;
	std::vector<CUnsignedArray> SigsVec;
	std::vector<ulong> vecWaitForGroup;
	ExtractSigsVec(res, SigsVec, vecWaitForGroup);
	g_log << "Completed in " << t1.Reset() << " Sec." << g_log.nl << g_log.nl;

	g_log << "Group dfa who has only one sig..." << g_log.nl;
	CGroups groups;
	GroupOnlyOneSig(SigsVec, vecWaitForGroup, groups);
	g_log << "Completed in " << t1.Reset() << " Sec." << g_log.nl << g_log.nl;

	//Merge dfa with only one signature...
	g_log << "Merge dfa with only one signature..." << g_log.nl;
	DfaSizeSort(res, groups);
	Merge(res, groups);
	g_log << "Completed in " << t1.Reset() << " Sec." << g_log.nl << g_log.nl;

	//Put dfa in group which have the same signature...
	g_log << "Put dfa in group which have the same signature..." << g_log.nl;
	PutInBySig(SigsVec, res, groups, vecWaitForGroup);
	g_log << "Completed in " << t1.Reset() << " Sec." << g_log.nl << g_log.nl;

	//Build group which has the same signature...
	g_log << "Build group which has the same signatures..." << g_log.nl;
	CGroups newGroups;
	BuildGroupBySig(SigsVec, newGroups, vecWaitForGroup);
	//DfaSizeSort(res, newGroups);
	Merge(res, newGroups);
	g_log << "Completed in " << t1.Reset() << " Sec." << g_log.nl << g_log.nl;

	//Extract the already used signatures...
	g_log << "Extract the already used signatures..." << g_log.nl;
	std::vector<SIGNATURE> vecUsed;
	ExtractUsedSigs(groups, vecUsed);
	g_log << "Completed in " << t1.Reset() << " Sec." << g_log.nl << g_log.nl;

	//Merge group which have the same signature...
	g_log << "Merge group which have the same signatures..." << g_log.nl;
	MergeGroup(res, vecUsed, newGroups);
	g_log << "Completed in " << t1.Reset() << " Sec." << g_log.nl << g_log.nl;
	
	//Add new groups...
	g_log << "Add new groups..." << g_log.nl;
	AddNewGroups(newGroups, groups);
	g_log << "Completed in " << t1.Reset() << " Sec." << g_log.nl << g_log.nl;

	//Clear up the result...
	g_log << "Clear up the result..." << g_log.nl;
	ClearUpRes(res, groups, groupRes);

	g_log << "Completed in " << t1.Reset() << " Sec." << g_log.nl << g_log.nl;

	g_log << groupRes.GetGroups().Size() << g_log.nl;

	g_log << "Total time: " << tAll.Reset() << " Sec." << g_log.nl;
}