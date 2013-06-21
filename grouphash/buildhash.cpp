/**
**	@file		buildhash.cpp
**
**	@author		Lab 435, Xidian University
**
**	@brief		Support functions for mapping groups into hash table
**
**	This implements mapping groups into hash table algorithm, the algorithm has 
**	four steps in general. At first, it maps groups which has only one signature. 
**	Then, it maps groups which has two or more signatures, for one group, select
**	a signature which corresponds to a hash slot with least groups. Thirdly, 
**	simple adjust for conflict hash slot. Finally, recursive adjust for conflict 
**	hash slot. After that, try to combine two groups in a hash slot or two hash 
**	slots in some situations.
**
*/

#include "stdafx.h"
#include "hash.h"
#include <hwprj\buildhash.h>
#include <hwprj\siddfaids.h>

/* hash function, correspond to a hash slot for a given signature

Arguments:
  oneSig				signature

Returns:				hash slot

*/

struct SIGHASH
{
	ulong nBucketCnt;
	ulong operator()(const SIGNATURE &oneSig)
	{
		const ulong _FNV_offset_basis = 2166136261U;
		const ulong _FNV_prime = 16777619U;

		ulong _Val = _FNV_offset_basis;
		_Val ^= oneSig;
		_Val *= _FNV_prime;

		return (_Val % nBucketCnt);
	}
}hash;

//one point in the adjust path
struct STATION
{
	ulong dfaId;
	SIGNATURE sig;
};

/* find the adjust path

Arguments:
  vecGroups			the result of group
  result				the hash table state
  currSig			check the hash slot the currSig corresponds to
  vecPath			record the adjust path
  depth				 the find depth

Returns:				true if find a adjust path to reduce conflict
						false otherwise

*/

bool myFind(std::vector<GROUPHASH> &vecGroups, RESULTMAP &result, SIGNATURE &currSig, std::deque<STATION> &seqPath, ulong &depth)
{
	++depth;

	//the hash slot has no group
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
		for (std::vector<ulong>::iterator i = result[hash(currSig)].begin(); i != result[hash(currSig)].end(); ++i)
		{
			std::deque<STATION>::iterator iter;
			for (iter = seqPath.begin(); iter != seqPath.end(); ++iter)
			{
				if (iter->dfaId == *i)
				{
					break;
				}
			}
			if (iter != seqPath.end())
			{
				continue;
			}
			for (std::vector<SIGNATURE>::iterator j = vecGroups[*i].vecSigs.begin(); j != vecGroups[*i].vecSigs.end(); ++j)
			{
				if (*j == vecGroups[*i].currSig)
				{
					continue;
				}
				seqPath.push_front(STATION());
				seqPath.front().dfaId = *i;
				seqPath.front().sig = *j;
				if (myFind(vecGroups, result, *j, seqPath, depth))
				{
					return true;
				}
				else
				{
					seqPath.pop_front();
					--depth;
				}
			}
		}
		return false;
	}
}

/* recursive adjust for conflict hash slot.

Arguments:
  vecGroups			    the result of group
  result				the hash table state

Returns:				nothing

*/

void RecursiveAdjust(std::vector<GROUPHASH> &vecGroups, RESULTMAP &result)
{
	bool flag = true;
	while (flag)
	{
		flag = false;
		for (RESULTMAP::iterator i = result.begin(); i != result.end(); ++i)
		{
			//try to adjust the conflict hash slot
			if (i->second.size() > 1)
			{
				for (std::vector<ulong>::iterator j = i->second.begin(); j != i->second.end();)
				{
					std::deque<STATION> seqPath;
					ulong depth;
					bool change = false;
					for (std::vector<SIGNATURE>::iterator k = vecGroups[*j].vecSigs.begin(); k != vecGroups[*j].vecSigs.end(); ++k)
					{
						if (*k == vecGroups[*j].currSig)
						{
							continue;
						}
						seqPath.clear();
						depth = 0;
						seqPath.push_front(STATION());
						seqPath.front().dfaId = *j;
						seqPath.front().sig = *k;
						if (myFind(vecGroups, result, *k, seqPath, depth))
						{
							flag = true;
							change = true;
							j = i->second.erase(j);
							while (!seqPath.empty())
							{
								std::vector<ulong> &Ids = result[hash(vecGroups[seqPath.back().dfaId].currSig)];
								std::vector<ulong>::iterator iter = std::find(Ids.begin(), Ids.end(), seqPath.back().dfaId);
								if (iter != Ids.end())
								{
									Ids.erase(iter);
								}
								vecGroups[seqPath.back().dfaId].currSig = seqPath.back().sig;
								result[hash(vecGroups[seqPath.back().dfaId].currSig)].push_back(seqPath.back().dfaId);
								seqPath.pop_back();
							}
							break;
						}
					}
					if (change)
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
}

/* simple adjust for conflict hash slot.

Arguments:
  vecGroups			    the result of group
  result				the hash table state

Returns:				nothing

*/
//This function is abandoned, because it is contained by the function RecursiveAdjust.

void Adjust(std::vector<GROUPHASH> &vecGroups, RESULTMAP &result)
{
	for (RESULTMAP::iterator i = result.begin(); i != result.end(); ++i)
	{
		//try to adjust the conflict hash slot
		if (i->second.size() > 1)
		{
			for (std::vector<ulong>::iterator j = i->second.begin(); j != i->second.end();)
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

/* map groups which has two or more signatures, for one group, select
a signature which corresponds to a hash slot with least groups

Arguments:
  vecGroups			the result of group
  dmap				the correspondence between dfa and its signatures
  vecIds				the index of group waiting for mapping
  result				the hash table state

Returns:				nothing

*/

void Optimize(std::vector<GROUPHASH> &vecGroups, const IDMAP &dmap, std::vector<ulong> &vecIds, RESULTMAP &result)
{
	for (IDMAP::const_iterator i = dmap.begin(); i != dmap.end(); ++i)
	{
		if (std::find(vecIds.begin(), vecIds.end(), i->first) == vecIds.end())
		{
			ulong min = dmap.size();
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

/* map groups into hash slot

Arguments:
  vecGroups			the result of group
  gmap				the correspondence between signature and dfas
  dmap				the correspondence between dfa and its signatures
  result				the hash table state

Returns:				nothing

*/

void Mapping(std::vector<GROUPHASH> &vecGroups, const SIGNATUREMAP &gmap, const IDMAP &dmap, RESULTMAP &result)
{
	std::vector<ulong> vecIds;
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

	//Adjust(vecGroups, result);

	RecursiveAdjust(vecGroups, result);
}

/* extract common signatures from two groups

Arguments:
  g1					 the first group
  g2					 the second group
  vecComSigs		common signatures of the two groups

Returns:				nothing

*/

void CommonSigs(const GROUPHASH &g1, const GROUPHASH &g2, std::vector<SIGNATURE> &vecComSigs)
{
	std::map<SIGNATURE, ulong> sigToNumMap;
	for (ulong i = 0; i < g1.vecSigs.size(); ++i)
	{
		++sigToNumMap[g1.vecSigs[i]];
	}
	for (ulong i = 0; i < g2.vecSigs.size(); ++i)
	{
		++sigToNumMap[g2.vecSigs[i]];
	}

	for (std::map<SIGNATURE, ulong>::iterator i = sigToNumMap.begin(); i != sigToNumMap.end(); ++i)
	{
		if (i->second == 2)
		{
			vecComSigs.push_back(i->first);
		}
	}
}

/* update g1's signatures to the common signatures of g1 and g2

Arguments:
  g1					 the first group
  g2					 the second group

Returns:				nothing

*/

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

/* judge the group g1 and g2 can combine or not

Arguments:
  g1					 the first group
  g2					 the second group
  result				the hash table state
  Sig					the signature selected to be used for hash

Returns:				true if there has a signature which can represent group g1 and g2
						and the signature corresponds to an empty hash slot or g1 or g2's 
					hash slot
					false otherwise

*/

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

/* try to combine two groups in a hash slot or two hash slots in some situations.

Arguments:
  groupRes			 the merged dfa will be pushed into the groupRes's dfa table
  vecGroups			the result of group
  result				the hash table state

Returns:				nothing

*/

void Combine(CGroupRes &groupRes, std::vector<GROUPHASH> &vecGroups, RESULTMAP &result)
{
	//combine groups in one hash slot
	for (RESULTMAP::iterator i = result.begin(); i != result.end(); ++i)
	{
		if (i->second.size() > 1)
		{
			for (std::vector<ulong>::iterator j = i->second.begin(); j != i->second.end(); ++j)
			{
				//std::vector<CDfa> vecDfas(2);
				CDfaArray vecDfas;
				vecDfas.Resize(2);
				vecDfas[0] = groupRes.GetDfaTable()[vecGroups[*j].mergeDfaId];
				for (std::vector<ulong>::iterator k = j + 1; k != i->second.end(); )
				{
					if (vecGroups[*k].currSig != vecGroups[*j].currSig)
					{
						++k;
						continue;
					}
					CDfa MergeDfa;
					vecDfas[1] = groupRes.GetDfaTable()[vecGroups[*k].mergeDfaId];
					if (MergeMultipleDfas(vecDfas, MergeDfa))
					{
						for (ulong idx = 0; idx < vecGroups[*k].vecDfaIds.size(); ++idx)
						{
							vecGroups[*j].vecDfaIds.push_back(vecGroups[*k].vecDfaIds[idx]);
						}
						UpdateSigs(vecGroups[*j], vecGroups[*k]);
						groupRes.GetDfaTable().PushBack(MergeDfa);
						vecGroups[*j].mergeDfaId = groupRes.GetDfaTable().Size() - 1;
						vecGroups[*k].vecSigs.clear();
						vecGroups[*k].vecDfaIds.clear();
						vecGroups[*k].mergeDfaId = (ulong)-1;
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

	//combine groups in two hash slots and both hash slots have only one group
	std::vector<ulong> vecKeys;
	for (RESULTMAP::iterator i = result.begin(); i != result.end(); ++i)
	{
		vecKeys.push_back(i->first);
	}

	for (ulong i = 0; i < vecKeys.size(); ++i)
	{
		g_log << "Combine" << g_log.nl;
		g_log << "NO: " << i << g_log.nl;
		g_log << "Total: " << vecKeys.size() << g_log.nl;
		if (result[vecKeys[i]].size() == 1)
		{
			for (ulong j = 0; j < vecKeys.size(); ++j)
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
						//std::vector<CDfa> vecDfas(2);
						CDfaArray vecDfas;
						vecDfas.Resize(2);
						vecDfas[0] = groupRes.GetDfaTable()[g1.mergeDfaId];
						vecDfas[1] = groupRes.GetDfaTable()[g2.mergeDfaId];
						CDfa MergeDfa;
						if (MergeMultipleDfas(vecDfas, MergeDfa))
						{
							for (ulong idx = 0; idx < g2.vecDfaIds.size(); ++idx)
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
							g2.mergeDfaId = (ulong)-1;
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

/* clear up the group result and hash table

Arguments:
  vecGroups			the result of group
  result				the hash table state
  groupRes			 the relationship between sid and dfa id, dfa table and result of grouping
  HashResMap		the hash table after clear up

Returns:				nothing

*/

void ClearUpHashRes(std::vector<GROUPHASH> &vecGroups, RESULTMAP &result, CGroupRes &groupRes, HASHRES &HashResMap)
{
	ulong count = 0;
	std::vector<CDfa> vecDfas;
	groupRes.GetGroups().Clear();
	for (std::vector<GROUPHASH>::iterator i = vecGroups.begin(); i != vecGroups.end(); ++i)
	{
		if (i->mergeDfaId != ulong(-1))
		{
			groupRes.GetGroups().PushBack(ONEGROUP());
			ONEGROUP &group = groupRes.GetGroups().Back();
			for (ulong j = 0; j < i->vecDfaIds.size(); ++j)
			{
				group.DfaIds.PushBack(i->vecDfaIds[j]);
			}
			for (ulong j = 0; j < i->vecSigs.size(); ++j)
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
	for (std::vector<CDfa>::iterator i = vecDfas.begin(); i != vecDfas.end(); ++i)
	{
		groupRes.GetDfaTable().PushBack(*i);
	}

	for (ulong i = 0; i < groupRes.GetDfaTable().Size(); ++i)
	{
		groupRes.GetDfaTable()[i].Minimize();
	}

	for (RESULTMAP::iterator i = result.begin(); i != result.end(); ++i)
	{
		for (std::vector<ulong>::iterator j = i->second.begin(); j != i->second.end(); ++j)
		{
			if (vecGroups[*j].mergeDfaId != ulong(-1))
			{
				HASHNODE node;
				node.m_sig = vecGroups[*j].currSig;
				node.m_nDfaId = vecGroups[*j].mergeDfaId;
				HashResMap[i->first].push_back(node);
			}
		}
	}
}

/* mapping groups into hash table and combine groups in some situation to reduce 
number of groups

Arguments:
  groupRes			 the relationship between sid and dfa id, dfa table and result of grouping
  HashResMap		the hash table

Returns:				nothing

*/

HASHMAPHDR void HashMapping(CGroupRes &groupRes, HASHRES &HashResMap)
{
	std::vector<GROUPHASH> vecGroups;
	vecGroups.resize(groupRes.GetGroups().Size());
	for (ulong i = 0; i < groupRes.GetGroups().Size(); ++i)
	{
		for (ulong j = 0; j < groupRes.GetGroups()[i].ComSigs.Size(); ++j)
		{
			vecGroups[i].vecSigs.push_back(groupRes.GetGroups()[i].ComSigs[j]);
		}
		vecGroups[i].mergeDfaId = groupRes.GetGroups()[i].mergeDfaId;
		for (ulong j = 0; j < groupRes.GetGroups()[i].DfaIds.Size(); ++j)
		{
			vecGroups[i].vecDfaIds.push_back(groupRes.GetGroups()[i].DfaIds[j]);
		}
	}

	SIGNATUREMAP gmap;
	for (ulong i = 0; i < vecGroups.size(); ++i)
	{
		for (ulong j = 0; j < vecGroups[i].vecSigs.size(); ++j)
		{
			gmap[vecGroups[i].vecSigs[j]].push_back(i);
		}
	}
	IDMAP dmap;
	for (ulong i = 0; i < vecGroups.size(); ++i)
	{
		for (ulong j = 0; j < vecGroups[i].vecSigs.size(); ++j)
		{
			dmap[i].push_back(vecGroups[i].vecSigs[j]);
		}
	}

	ulong nRuleCnt = 0;
	for (ulong i = 0; i < groupRes.GetSidDfaIds().Size(); ++i)
	{
		if (groupRes.GetSidDfaIds()[i].m_nResult == COMPILEDINFO::RES_SUCCESS)
		{
			++nRuleCnt;
		}
	}
	hash.nBucketCnt = 4 * nRuleCnt;

	RESULTMAP result;
	Mapping(vecGroups, gmap, dmap, result);

	Combine(groupRes, vecGroups, result);

	ClearUpHashRes(vecGroups, result, groupRes, HashResMap);
}
