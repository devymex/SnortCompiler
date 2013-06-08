/**
**  @file        hashmapping.cpp
**
**  @author      Lab 435, Xidian University
**
**  @brief       Support functions for mapping groups into hash table
**
**  This implements mapping groups into hash table algorithm, the algorithm has 
**  four steps in general. 
**
*/

#include "stdafx.h"
#include "hashmapping.h"
#include "../dfanew/dfanew.h"
#include "../common/common.h"
#include "../compilernew/compilernew.h"
#include "../grouping/grouping.h"
#include "../mergedfanew/MergeDfanew.h"

HASHMAPPINGSC size_t hash(const SIGNATURE &oneSig)
{
	return oneSig % 16001;
}

struct PATH
{
	size_t dfaId;
	SIGNATURE sig;
};

bool myFind(std::vector<GROUPHASH> &vecGroups, RESULTMAP &result, SIGNATURE &currSig, std::vector<PATH> &vecPath, size_t &depth)
{
	++depth;
	if (result[hash(currSig)].size() == 0)
	{
		return true;
	}
	else if (depth > 5)
	{
		return false;
	}
	else
	{
		for (std::vector<size_t>::iterator i = result[hash(currSig)].begin(); i != result[hash(currSig)].end(); ++i)
		{
			for (std::vector<SIGNATURE>::iterator j = vecGroups[*i].vecSigs.begin(); j != vecGroups[*i].vecSigs.end(); ++j)
			{
				if (*j == vecGroups[*i].currSig)
				{
					continue;
				}
				if (myFind(vecGroups, result, *j, vecPath, depth))
				{
					vecPath.push_back(PATH());
					vecPath.back().dfaId = *i;
					vecPath.back().sig = *j;
					return true;
				}
			}
		}
		return false;
	}
}

void RecursiveAdjust(std::vector<GROUPHASH> &vecGroups, const IDMAP &dmap, RESULTMAP &result)
{
	bool flag = true;
	while (flag)
	{
		flag = false;
		for (RESULTMAP::iterator i = result.begin(); i != result.end(); ++i)
		{
			if (i->second.size() > 1)
			{
				for (std::vector<size_t>::iterator j = i->second.begin(); j != i->second.end();)
				{
					std::vector<PATH> vecPath;
					size_t depth;
					for (std::vector<SIGNATURE>::iterator k = vecGroups[*j].vecSigs.begin(); k != vecGroups[*j].vecSigs.end(); ++k)
					{
						if (*k == vecGroups[*j].currSig)
						{
							continue;
						}
						vecPath.clear();
						depth = 0;
						if (myFind(vecGroups, result, *k, vecPath, depth))
						{
							flag = true;
							vecGroups[*j].currSig = *k;
							std::vector<size_t>::iterator iter = std::find(result[hash(*k)].begin(), result[hash(*k)].end(), *j);
							if (iter != result[hash(*k)].end())
							{
								result[hash(*k)].erase(iter);
							}
							result[hash(*k)].push_back(*j);
							j = i->second.erase(j);
							while (!vecPath.empty())
							{
								std::vector<size_t> &Ids = result[hash(vecGroups[vecPath.back().dfaId].currSig)];
								iter = std::find(Ids.begin(), Ids.end(), vecPath.back().dfaId);
								if (iter != Ids.end())
								{
									Ids.erase(iter);
								}
								vecGroups[vecPath.back().dfaId].currSig = vecPath.back().sig;
								result[hash(vecGroups[vecPath.back().dfaId].currSig)].push_back(vecPath.back().dfaId);
								vecPath.pop_back();
							}
							break;
						}
					}
					if (i->second.size() == 1)
					{
						break;
					}
					else
					{
						++j;
					}
				}
			}
		}
	}
}

void Adjust(std::vector<GROUPHASH> &vecGroups, const IDMAP &dmap, RESULTMAP &result)
{
	for (RESULTMAP::iterator i = result.begin(); i != result.end(); ++i)
	{
		if (i->second.size() > 1)
		{
			for (std::vector<size_t>::iterator j = i->second.begin(); j != i->second.end();)
			{
				bool flag = false;
				for (std::vector<SIGNATURE>::iterator k = vecGroups[*j].vecSigs.begin(); k != vecGroups[*j].vecSigs.end(); ++k)
				{
					if (result[hash(*k)].empty())
					{
						result[hash(*k)].push_back(*j);
						vecGroups[*j].currSig = *k;
						j = i->second.erase(j);
						flag = true;
						break;
					}
				}
				if (flag)
				{
					if (i->second.size() == 1)
					{
						break;
					}
				}
				else
				{
					++j;
				}
			}
		}
	}
}

void Optimize(std::vector<GROUPHASH> &vecGroups, const IDMAP &dmap, std::vector<size_t> &vecIds, RESULTMAP &result)
{
	for (IDMAP::const_iterator i = dmap.begin(); i != dmap.end(); ++i)
	{
		if (std::find(vecIds.begin(), vecIds.end(), i->first) == vecIds.end())
		{
			size_t min = dmap.size();
			SIGNATURE sig;
			for (std::vector<SIGNATURE>::const_iterator j = i->second.begin(); j != i->second.end(); ++j)
			{
				if (result[hash(*j)].empty())
				{
					sig = *j;
					break;
				}
				if (min > result[hash(*j)].size())
				{
					min = result[hash(*j)].size();
					sig = *j;
				}
			}
			result[hash(sig)].push_back(i->first);
			vecGroups[i->first].currSig = sig;
		}
	}
}

void Mapping(std::vector<GROUPHASH> &vecGroups, const SIGNATUREMAP &gmap, const IDMAP &dmap, RESULTMAP &result)
{
	std::vector<size_t> vecIds;
	for (IDMAP::const_iterator i = dmap.begin(); i != dmap.end(); ++i)
	{
		if (i->second.size() == 1)
		{
			result[hash(i->second[0])].push_back(i->first);
			vecGroups[i->first].currSig = i->second[0];
			vecIds.push_back(i->first);
		}
	}
	for (SIGNATUREMAP::const_iterator i = gmap.begin(); i != gmap.end(); ++i)
	{
		if (i->second.size() == 1 && std::find(vecIds.begin(), vecIds.end(), i->second[0]) == vecIds.end())
		{
			result[hash(i->first)].push_back(i->second[0]);
			vecGroups[i->second[0]].currSig = i->first;
			vecIds.push_back(i->second[0]);
		}
	}

	Optimize(vecGroups, dmap, vecIds, result);

	Adjust(vecGroups, dmap, result);

	RecursiveAdjust(vecGroups, dmap, result);

	//std::cout << vecGroups.size() << std::endl;
	//std::cout << (vecGroups.size() - result.size())/double(vecGroups.size()) << std::endl;
	//size_t count = 0;
	//for (RESULTMAP::iterator i = result.begin(); i != result.end(); ++i)
	//{
	//	count += i->second.size();
	//}
	//std::cout << count << std::endl;
}

void CommonSigs(const GROUPHASH &g1, const GROUPHASH &g2, std::vector<SIGNATURE> &vecComSigs)
{
	std::map<SIGNATURE, size_t> sigToNumMap;
	for (size_t i = 0; i < g1.vecSigs.size(); ++i)
	{
		++sigToNumMap[g1.vecSigs[i]];
	}
	for (size_t i = 0; i < g2.vecSigs.size(); ++i)
	{
		++sigToNumMap[g2.vecSigs[i]];
	}

	for (std::map<SIGNATURE, size_t>::iterator i = sigToNumMap.begin(); i != sigToNumMap.end(); ++i)
	{
		if (i->second == 2)
		{
			vecComSigs.push_back(i->first);
		}
	}
}

void UpdateSigs(GROUPHASH &g1, const GROUPHASH &g2)
{
	std::vector<SIGNATURE> vecComSigs;
	CommonSigs(g1, g2, vecComSigs);
	g1.vecSigs.clear();
	for (std::vector<SIGNATURE>::iterator i = vecComSigs.begin(); i != vecComSigs.end(); ++i)
	{
		g1.vecSigs.push_back(*i);
	}
}

bool CanCombine(const GROUPHASH &g1, const GROUPHASH &g2, RESULTMAP &result, SIGNATURE &Sig)
{
	std::vector<SIGNATURE> vecComSigs;
	CommonSigs(g1, g2, vecComSigs);
	for (std::vector<SIGNATURE>::iterator i = vecComSigs.begin(); i != vecComSigs.end(); ++i)
	{
		if (result[hash(*i)].size() == 0)
		{
			Sig = *i;
			return true;
		}
	}

	for (std::vector<SIGNATURE>::iterator i = vecComSigs.begin(); i != vecComSigs.end(); ++i)
	{
		if (hash(*i) == hash(g1.currSig) || hash(*i) == hash(g2.currSig))
		{
			Sig = *i;
			return true;
		}
	}
	return false;
}

void Combine(CGROUPRes &groupRes, std::vector<GROUPHASH> &vecGroups, RESULTMAP &result)
{
	for (RESULTMAP::iterator i = result.begin(); i != result.end(); ++i)
	{
		if (i->second.size() > 1)
		{
			for (std::vector<size_t>::iterator j = i->second.begin(); j != i->second.end(); ++j)
			{
				std::vector<CDfaNew> vecDfas(2);
				vecDfas[0] = groupRes.GetDfaTable()[vecGroups[*j].mergeDfaId];
				for (std::vector<size_t>::iterator k = j + 1; k != i->second.end(); )
				{
					CDfaNew MergeDfa;
					vecDfas[1] = groupRes.GetDfaTable()[vecGroups[*k].mergeDfaId];
					if (MergeMultipleDfas(vecDfas, MergeDfa))
					{
						for (size_t idx = 0; idx < vecGroups[*k].vecDfaIds.size(); ++idx)
						{
							vecGroups[*j].vecDfaIds.push_back(vecGroups[*k].vecDfaIds[idx]);
						}
						UpdateSigs(vecGroups[*j], vecGroups[*k]);
						groupRes.GetDfaTable().PushBack(MergeDfa);
						vecGroups[*j].mergeDfaId = groupRes.GetDfaTable().Size() - 1;
						vecGroups[*k].vecSigs.clear();
						vecGroups[*k].vecDfaIds.clear();
						vecGroups[*k].mergeDfaId = (size_t)-1;
						//groupRes.GetGroups().Erase(*k);
						k = i->second.erase(k);
					}
					else
					{
						++k;
					}
				}
			}
		}
	}

	std::vector<size_t> vecKeys;
	for (RESULTMAP::iterator i = result.begin(); i != result.end(); ++i)
	{
		vecKeys.push_back(i->first);
	}

	for (size_t i = 0; i < vecKeys.size(); ++i)
	{
		std::cout << "Combine" << std::endl;
		std::cout << "NO: " << i << std::endl;
		std::cout << "Total: " << vecKeys.size() << std::endl;
		if (result[vecKeys[i]].size() == 1)
		{
			for (size_t j = 0; j < vecKeys.size(); ++j)
			{
				if (i == j)
				{
					continue;
				}
				if (result[vecKeys[j]].size() == 1)
				{
					SIGNATURE Sig;
					GROUPHASH &g1 = vecGroups[result[vecKeys[i]][0]];
					GROUPHASH &g2 = vecGroups[result[vecKeys[j]][0]];
					if (CanCombine(g1, g2, result, Sig))
					{
						std::vector<CDfaNew> vecDfas(2);
						vecDfas[0] = groupRes.GetDfaTable()[g1.mergeDfaId];
						vecDfas[1] = groupRes.GetDfaTable()[g2.mergeDfaId];
						CDfaNew MergeDfa;
						if (MergeMultipleDfas(vecDfas, MergeDfa))
						{
							for (size_t idx = 0; idx < g2.vecDfaIds.size(); ++idx)
							{
								g1.vecDfaIds.push_back(g2.vecDfaIds[idx]);
							}
							UpdateSigs(g1, g2);
							groupRes.GetDfaTable().PushBack(MergeDfa);
							g1.mergeDfaId = groupRes.GetDfaTable().Size() - 1;
							if (hash(Sig) == hash(g1.currSig))
							{
								result[vecKeys[j]].clear();
							}
							else if (hash(Sig) == hash(g2.currSig))
							{
								result[vecKeys[j]].clear();
								result[vecKeys[j]].push_back(result[vecKeys[i]][0]);
								result[vecKeys[i]].clear();
							}
							else
							{
								result[hash(Sig)].push_back(result[vecKeys[i]][0]);
								if (std::find(vecKeys.begin(), vecKeys.end(), hash(Sig)) == vecKeys.end())
								{
									vecKeys.push_back(hash(Sig));
								}
								result[vecKeys[i]].clear();
								result[vecKeys[j]].clear();
							}
							g1.currSig = Sig;
							g2.vecSigs.clear();
							g2.vecDfaIds.clear();
							g2.mergeDfaId = (size_t)-1;
						}
						if (result[vecKeys[i]].size() == 0)
						{
							break;
						}
						if (result[vecKeys[j]].size() == 0)
						{
							continue;
						}
					}
				}
			}
			if (result[vecKeys[i]].size() == 0)
			{
				continue;
			}
		}
	}
}

void ClearUpHashRes(std::vector<GROUPHASH> &vecGroups, RESULTMAP &result, CGROUPRes &groupRes, HASHRES &HashResMap)
{
	size_t count = 0;
	std::vector<CDfaNew> vecDfas;
	groupRes.GetGroups().Clear();
	for (std::vector<GROUPHASH>::iterator i = vecGroups.begin(); i != vecGroups.end(); ++i)
	{
		if (i->mergeDfaId != size_t(-1))
		{
			groupRes.GetGroups().PushBack(ONEGROUP());
			ONEGROUP &group = groupRes.GetGroups().Back();
			for (size_t j = 0; j < i->vecDfaIds.size(); ++j)
			{
				group.DfaIds.PushBack(i->vecDfaIds[j]);
			}
			for (size_t j = 0; j < i->vecSigs.size(); ++j)
			{
				group.ComSigs.PushBack(i->vecSigs[j]);
			}
			group.mergeDfaId = count;
			group.currSig = i->currSig;
			vecDfas.push_back(groupRes.GetDfaTable()[i->mergeDfaId]);
			i->mergeDfaId = count;
			++count;
		}
	}
	
	groupRes.GetDfaTable().Clear();
	for (std::vector<CDfaNew>::iterator i = vecDfas.begin(); i != vecDfas.end(); ++i)
	{
		groupRes.GetDfaTable().PushBack(*i);
	}

	for (RESULTMAP::iterator i = result.begin(); i != result.end(); ++i)
	{
		for (std::vector<size_t>::iterator j = i->second.begin(); j != i->second.end(); ++j)
		{
			if (vecGroups[*j].mergeDfaId != size_t(-1))
			{
				HashResMap[i->first].push_back(HASHNODE(vecGroups[*j].currSig, vecGroups[*j].mergeDfaId));
			}
		}
	}
}

HASHMAPPINGSC void HashMapping(CGROUPRes &groupRes, HASHRES &HashResMap)
{
	std::vector<GROUPHASH> vecGroups;
	vecGroups.resize(groupRes.GetGroups().Size());
	for (size_t i = 0; i < groupRes.GetGroups().Size(); ++i)
	{
		for (size_t j = 0; j < groupRes.GetGroups()[i].ComSigs.Size(); ++j)
		{
			vecGroups[i].vecSigs.push_back(groupRes.GetGroups()[i].ComSigs[j]);
		}
		vecGroups[i].mergeDfaId = groupRes.GetGroups()[i].mergeDfaId;
		for (size_t j = 0; j < groupRes.GetGroups()[i].DfaIds.Size(); ++j)
		{
			vecGroups[i].vecDfaIds.push_back(groupRes.GetGroups()[i].DfaIds[j]);
		}
	}
	//std::sort(vecGroups.begin(), vecGroups.end(), COMP());

	SIGNATUREMAP gmap;
	for (size_t i = 0; i < vecGroups.size(); ++i)
	{
		for (size_t j = 0; j < vecGroups[i].vecSigs.size(); ++j)
		{
			gmap[vecGroups[i].vecSigs[j]].push_back(i);
		}
	}
	IDMAP dmap;
	for (size_t i = 0; i < vecGroups.size(); ++i)
	{
		for (size_t j = 0; j < vecGroups[i].vecSigs.size(); ++j)
		{
			dmap[i].push_back(vecGroups[i].vecSigs[j]);
		}
	}

	RESULTMAP result;
	Mapping(vecGroups, gmap, dmap, result);

	Combine(groupRes, vecGroups, result);

	//size_t count = 0;
	//size_t slots = 0;
	//for (RESULTMAP::iterator i = result.begin(); i != result.end(); ++i)
	//{
	//	count += i->second.size();
	//	if (i->second.size() > 0)
	//	{
	//		++slots;
	//	}
	//}
	//std::cout << count << std::endl;
	//std::cout << slots << std::endl;
	//std::cout << (count - slots)/double(count) << std::endl;

	ClearUpHashRes(vecGroups, result, groupRes, HashResMap);
}
