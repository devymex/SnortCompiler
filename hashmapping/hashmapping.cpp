#include "stdafx.h"
#include "hashmapping.h"
#include "../grouping/grouping.h"

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

void Mapping(std::vector<GROUPHASH> vecGroups, const SIGNATUREMAP &gmap, const IDMAP &dmap, RESULTMAP &result)
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

	std::cout << vecGroups.size() << std::endl;
	std::cout << (vecGroups.size() - result.size())/double(vecGroups.size()) << std::endl;
	size_t count = 0;
	for (RESULTMAP::iterator i = result.begin(); i != result.end(); ++i)
	{
		count += i->second.size();
	}
	std::cout << count << std::endl;
}

void ClearUpHashRes(const std::vector<GROUPHASH> &vecGroups, const RESULTMAP &result, HASHRES &HashResMap)
{
	for (RESULTMAP::const_iterator i = result.begin(); i != result.end(); ++i)
	{
		for (std::vector<size_t>::const_iterator j = i->second.begin(); j != i->second.end(); ++j)
		{
			HashResMap[i->first].push_back(HASHNODE(vecGroups[*j].currSig, vecGroups[*j].mergeDfaId));
		}
	}
}

HASHMAPPINGSC void HashMapping(CGROUPRes &groupRes, HASHRES &HashResMap)
{
	groupRes.ReadFromFile(_T("..\\..\\output\\GroupResut.cdt"));

	std::vector<GROUPHASH> vecGroups;
	vecGroups.resize(groupRes.GetGroups().Size());
	for (size_t i = 0; i < groupRes.GetGroups().Size(); ++i)
	{
		for (size_t j = 0; j < groupRes.GetGroups()[i].ComSigs.Size(); ++j)
		{
			vecGroups[i].vecSigs.push_back(groupRes.GetGroups()[i].ComSigs[j]);
		}
		vecGroups[i].mergeDfaId = groupRes.GetGroups()[i].mergeDfaId;
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

	ClearUpHashRes(vecGroups, result, HashResMap);
}
