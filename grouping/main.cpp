#include "stdafx.h"
#include "../compilernew/compilernew.h"
#include "../mergedfanew/MergeDfanew.h"

#define THRESHOLD 8

struct CHAINTRAIT
{
	enum {TRAIT_LEN = 64};
	char szTrait[TRAIT_LEN];
};

bool operator == (const CHAINTRAIT &r1, const CHAINTRAIT &r2)
{
	__int64 *p1 = (__int64*)r1.szTrait;
	__int64 *p2 = (__int64*)r2.szTrait;
	for (size_t i = 0; i < CHAINTRAIT::TRAIT_LEN / sizeof(__int64); ++i)
	{
		if (p1[i] != p2[i])
		{
			return false;
		}
	}
	return true;
}

struct CHAINTRAITHASH
{
	enum {HASH_SIZE = 1000000};
	size_t operator()(const CHAINTRAIT &rt)
	{
		const size_t *p = (size_t*)rt.szTrait;
		size_t n = 1;
		for (size_t i = 0; i < CHAINTRAIT::TRAIT_LEN / sizeof(size_t); ++i)
		{
			n *= p[i];
		}

		return n % HASH_SIZE;
	}
};

typedef std::unordered_map<CHAINTRAIT, std::vector<size_t>, CHAINTRAITHASH> TRAITMAP;

struct RULECHAIN
{
	CDfanew dfa;
	std::string strChain;
	std::vector<SIGNATURE> sigs;
};

void BuildSidToDfaIdsMap(CResNew &res, std::map<size_t, std::vector<size_t>> &sidToDfaIdsMap)
{
	for (size_t i = 0; i < res.GetSidDfaIds().Size(); ++i)
	{
		std::vector<size_t> vecDfaIds;
		for (size_t j = 0; j < res.GetSidDfaIds()[i].m_dfaIds.Size(); ++j)
		{
			vecDfaIds.push_back(res.GetSidDfaIds()[i].m_dfaIds[j]);
		}
		sidToDfaIdsMap[res.GetSidDfaIds()[i].m_nSid] = vecDfaIds;
	}
}

void BuildDfaIdToSidMap(CResNew &res, std::map<size_t, size_t> &dfaIdToSidMap)
{
	for (size_t i = 0; i < res.GetSidDfaIds().Size(); ++i)
	{
		for (size_t j = 0; j < res.GetSidDfaIds()[i].m_dfaIds.Size(); ++j)
		{
			dfaIdToSidMap[res.GetSidDfaIds()[i].m_dfaIds[j]] = res.GetSidDfaIds()[i].m_nSid;
		}
	}
}

void BuildChainSet(CResNew &res, std::vector<RULECHAIN> &chainSet)
{
	res.GetRegexTbl();
	chainSet.reserve(10000);
	std::string strTmp;
	size_t nBeg, nEnd;
	//size_t nPos;
	for (size_t i = 0; i < res.GetRegexTbl().Size(); ++i)
	{
		chainSet.push_back(RULECHAIN());
		RULECHAIN &cr = chainSet.back();
		for (size_t j = 0; j < res.GetRegexTbl()[i].Size(); ++j)
		{
			strTmp = res.GetRegexTbl()[i][j].C_Str();
			nBeg = strTmp.find_first_of('/');
			nEnd = strTmp.find_last_of('/');
			cr.strChain += strTmp.substr(nBeg + 1, nEnd - nBeg - 1);
		}
		//while ((nPos = cr.strList.find("\\x", 0)) != std::string::npos)
		//{
		//	cr.strList.erase(nPos, 2);
		//}
		for (size_t j = 0; j < res.GetRegexTbl()[i].GetSigCnt(); ++j)
		{
			cr.sigs.push_back(res.GetRegexTbl()[i].GetSig(j));
		}
	}
	for (size_t i = 0; i < res.GetDfaTable().Size(); ++i)
	{
		chainSet[i].dfa = res.GetDfaTable()[i];
	}
}

bool ExtractTraits(const RULECHAIN &rulechain, CHAINTRAIT &trait)
{
	bool flag = false;
	size_t nLen = rulechain.strChain.length();
	if (nLen > CHAINTRAIT::TRAIT_LEN)
	{
		flag = true;
		CopyMemory(trait.szTrait, &rulechain.strChain[0], CHAINTRAIT::TRAIT_LEN);
	}
	return flag;
}

void BuildChainTraitMap(const std::vector<RULECHAIN> &chainSet, TRAITMAP &traitMap)
{
	//traitMap.rehash(CHAINTRAITHASH::HASH_SIZE);
	for (std::vector<RULECHAIN>::const_iterator i = chainSet.begin(); i != chainSet.end(); ++i)
	{
		CHAINTRAIT trait;
		if (ExtractTraits(*i, trait))
		{
			TRAITMAP::iterator irt = traitMap.find(trait);
			if (irt == traitMap.end())
			{
				std::vector<size_t> newVec;
				newVec.push_back(i - chainSet.begin());
				traitMap.insert(std::make_pair(trait, newVec));
			}
			else
			{
				irt->second.push_back(i - chainSet.begin());
			}
		}
	}
}

struct CHAINGROUP
{
	std::vector<size_t> chainIds;
	std::string comStr;
	std::vector<SIGNATURE> comSigs;
	std::size_t mergeDfaId;
};

//计算两个字符串的最长公共前缀
std::string LongestCommonPrefix(const std::string &str1, const std::string &str2)
{
	size_t count = 0;
	size_t min = str1.length() < str2.length() ? str1.length() : str2.length();
	while (count < min && str1[count] == str2[count])
	{
		++count;
	}
	return str1.substr(0, count);
}

//初步分组
void PreGroup(const std::vector<RULECHAIN> &chainSet, const TRAITMAP &traitMap, std::vector<CHAINGROUP> &vecChainGroups, std::vector<size_t> &vecWaitForGroup)
{
	std::vector<size_t> vecDistributed;//记录分配出去的规则链
	for (TRAITMAP::const_iterator i = traitMap.begin(); i != traitMap.end(); ++i)
	{
		if (i->second.size() > 1)
		{
			vecChainGroups.push_back(CHAINGROUP());
			CHAINGROUP &oneGroup = vecChainGroups.back();
			oneGroup.chainIds = i->second;
			vecDistributed.insert(vecDistributed.begin(), i->second.begin(), i->second.end());
		}
	}

	//计算每个分组的最长公共前缀
	for (std::vector<CHAINGROUP>::iterator i = vecChainGroups.begin(); i != vecChainGroups.end(); ++i)
	{
		i->comStr = chainSet[i->chainIds[0]].strChain;
		for (std::vector<size_t>::const_iterator j = i->chainIds.begin() + 1; j != i->chainIds.end(); ++j)
		{
			i->comStr = LongestCommonPrefix(i->comStr, chainSet[*j].strChain);
		}
	}

	//记录还未分配出去的规则链
	for (size_t i = 0; i < chainSet.size(); ++i)
	{
		if (std::find(vecDistributed.begin(), vecDistributed.end(), i) == vecDistributed.end())
		{
			vecWaitForGroup.push_back(i);
		}
	}
}

//进一步分组
void FartherGroup(const std::vector<RULECHAIN> &chainSet, std::vector<CHAINGROUP> &vecChainGroups, std::vector<size_t> &vecWaitForGroup)
{
	size_t max;
	size_t tmp;
	size_t index;
	bool flag;
	for (std::vector<size_t>::const_iterator i = vecWaitForGroup.begin(); i != vecWaitForGroup.end(); ++i)
	{
		max = 0;
		index = 0;
		flag = false;
		for (size_t j = 0; j < vecChainGroups.size(); ++j)
		{
			tmp = LongestCommonPrefix(vecChainGroups[j].comStr, chainSet[*i].strChain).length();
			if (tmp == chainSet[*i].strChain.length() && tmp == vecChainGroups[j].comStr.length())
			{
				flag = true;
				max = tmp;
				index = j;
				break;
			}
			if (max < tmp)
			{
				max = tmp;
				index = j;
			}
		}
		if (max >= THRESHOLD || flag)
		{
			vecChainGroups[index].chainIds.push_back(*i);
			vecChainGroups[index].comStr = vecChainGroups[index].comStr.substr(0, max);
		}
		else
		{
			vecChainGroups.push_back(CHAINGROUP());
			CHAINGROUP &group = vecChainGroups.back();
			group.chainIds.push_back(*i);
			group.comStr = chainSet[*i].strChain;
		}
	}

	vecWaitForGroup.clear();
	for (std::vector<CHAINGROUP>::iterator i = vecChainGroups.begin(); i != vecChainGroups.end();)
	{
		if (i->chainIds.size() == 1)
		{
			vecWaitForGroup.push_back(i->chainIds[0]);
			i = vecChainGroups.erase(i);
		}
		else
		{
			++i;
		}
	}
}

void SortChainId(std::vector<CHAINGROUP> &vecChainGroups, const std::vector<RULECHAIN> &chainSet)
{
	struct COMP
	{
		const std::vector<RULECHAIN> *pchainSet;
		COMP(const std::vector<RULECHAIN> *p) : pchainSet(p) {}
		bool operator()(const size_t &id1, const size_t &id2)
		{
			return (*pchainSet)[id1].dfa.Size() < (*pchainSet)[id2].dfa.Size();
		}
	};
	for (std::vector<CHAINGROUP>::iterator i = vecChainGroups.begin(); i != vecChainGroups.end(); ++i)
	{
		std::sort(i->chainIds.begin(), i->chainIds.end(), COMP(&chainSet));
	}
}

void ExtractExplosion(const std::vector<RULECHAIN> &chainSet, std::vector<CHAINGROUP> &vecChainGroups, std::vector<size_t> &vecWaitForGroup, std::vector<size_t> &vecExplosion)
{
	for (std::vector<CHAINGROUP>::iterator i = vecChainGroups.begin(); i != vecChainGroups.end();)
	{
		for (std::vector<size_t>::iterator j = i->chainIds.begin(); j != i->chainIds.end();)
		{
			if (chainSet[*j].dfa.Size() == 0)
			{
				vecExplosion.push_back(*j);
				j = i->chainIds.erase(j);
			}
			else
			{
				break;
			}
		}
		if (i->chainIds.size() == 1 || i->chainIds.size() == 0)
		{
			if (i->chainIds.size() == 1)
			{
				vecWaitForGroup.push_back(i->chainIds[0]);
			}
			i = vecChainGroups.erase(i);
		}
		else
		{
			++i;
		}
	}

	for (std::vector<size_t>::iterator i = vecWaitForGroup.begin(); i != vecWaitForGroup.end();)
	{
		if (chainSet[*i].dfa.Size() == 0)
		{
			vecExplosion.push_back(*i);
			i = vecWaitForGroup.erase(i);
		}
		else
		{
			++i;
		}
	}
}

void MergeMore(std::vector<CHAINGROUP> &vecChainGroups, CResNew &res, std::vector<size_t> &vecWaitForGroup)
{
	bool mergeFlag;
	std::vector<CDfanew> vecDfas(2);
	CDfanew MergeDfa;
	for (std::vector<CHAINGROUP>::iterator i = vecChainGroups.begin(); i != vecChainGroups.end();)
	{
		std::cout << "More" << std::endl;
		std::cout << "NO: " << i - vecChainGroups.begin() << std::endl;
		std::cout << "Total: " << vecChainGroups.size() << std::endl << std::endl;
		std::vector<CDfanew> vecDfas(2);
		vecDfas[0] = res.GetDfaTable()[i->chainIds[0]];
		vecDfas[1] = res.GetDfaTable()[i->chainIds[1]];
		if (!NOrMerge(vecDfas, MergeDfa))
		{
			for (size_t j = 0; j < i->chainIds.size(); ++j)
			{
				vecWaitForGroup.push_back(i->chainIds[j]);
			}
			i = vecChainGroups.erase(i);
			continue;
		}
		mergeFlag = true;
		vecDfas[0] = MergeDfa;
		for (size_t j = 2; j < i->chainIds.size(); ++j)
		{
			vecDfas[1] = res.GetDfaTable()[i->chainIds[j]];
			if (!NOrMerge(vecDfas, MergeDfa))
			{
				mergeFlag = false;
				res.GetDfaTable().PushBack(vecDfas[0]);
				if (i->chainIds.size() - j == 1)
				{
					vecWaitForGroup.push_back(i->chainIds[j]);
				}
				else
				{
					vecChainGroups.resize(vecChainGroups.size() + 1);
					CHAINGROUP &oneGroup = vecChainGroups.back();
					oneGroup.chainIds.insert(oneGroup.chainIds.begin(), i->chainIds.begin() + j, i->chainIds.end());
				}
				i->chainIds.erase(i->chainIds.begin() + j, i->chainIds.end());
				i->mergeDfaId = res.GetDfaTable().Size() - 1;
				break;
			}
			else
			{
				vecDfas[0] = MergeDfa;
			}
		}
		if (mergeFlag)
		{
			res.GetDfaTable().PushBack(vecDfas[0]);
			i->mergeDfaId = res.GetDfaTable().Size() - 1;
		}
		++i;
	}
}

void ExtractSigs(const std::vector<RULECHAIN> &chainSet, std::vector<CHAINGROUP> &vecChainGroups)
{
	for (std::vector<CHAINGROUP>::iterator i = vecChainGroups.begin(); i != vecChainGroups.end(); ++i)
	{
		std::map<SIGNATURE, size_t> SigMap;
		for (std::vector<size_t>::iterator j = i->chainIds.begin(); j != i->chainIds.end(); ++j)
		{
			for (std::vector<SIGNATURE>::const_iterator k = chainSet[*j].sigs.begin(); k != chainSet[*j].sigs.end(); ++k)
			{
				++SigMap[*k];
			};
		}

		i->comSigs.clear();
		for (std::map<SIGNATURE, size_t>::iterator j = SigMap.begin(); j != SigMap.end(); ++j)
		{
			if (j->second == i->chainIds.size())
			{
				i->comSigs.push_back(j->first);
			}
		}
	}
}

void PutInByRule(std::map<size_t, size_t> &dfaIdToSidMap, std::map<size_t, std::vector<size_t>> &sidToDfaIdsMap, std::vector<CHAINGROUP> &vecChainGroups, CResNew &res, std::vector<size_t> &vecWaitForGroup)
{
	std::map<size_t, size_t> dfaIdToGroupId;
	for (std::vector<CHAINGROUP>::iterator i = vecChainGroups.begin(); i != vecChainGroups.end(); ++i)
	{
		for (std::vector<size_t>::iterator j = i->chainIds.begin(); j != i->chainIds.end(); ++j)
		{
			dfaIdToGroupId[*j] = i - vecChainGroups.begin();
		}
	}
	std::vector<size_t> vecWaitForGroupCopy(vecWaitForGroup.begin(), vecWaitForGroup.end());
	vecWaitForGroup.clear();
	for (std::vector<size_t>::iterator i = vecWaitForGroupCopy.begin(); i != vecWaitForGroupCopy.end(); ++i)
	{
		std::cout << "PutInByRule" << std::endl;
		std::cout << "One" << std::endl;
		std::cout << "NO: " << i - vecWaitForGroupCopy.begin() << std::endl;
		std::cout << "Total: " << vecWaitForGroupCopy.size() << std::endl << std::endl;
		vecWaitForGroup.push_back(*i);
		for (std::vector<size_t>::iterator j = sidToDfaIdsMap[dfaIdToSidMap[*i]].begin(); j != sidToDfaIdsMap[dfaIdToSidMap[*i]].end(); ++j)
		{
			std::map<size_t, size_t>::iterator k = dfaIdToGroupId.find(*j);
			if (k != dfaIdToGroupId.end())
			{
				std::vector<CDfanew> vecDfas;
				vecDfas.push_back(res.GetDfaTable()[vecChainGroups[k->second].mergeDfaId]);
				vecDfas.push_back(res.GetDfaTable()[*i]);
				CDfanew MergeDfa;
				if (NOrMerge(vecDfas, MergeDfa))
				{
					vecChainGroups[k->second].chainIds.push_back(*i);
					res.GetDfaTable().PushBack(MergeDfa);
					vecChainGroups[k->second].mergeDfaId = res.GetDfaTable().Size() - 1;
					vecWaitForGroup.pop_back();
					break;
				}
			}
		}
	}
}

void UpdateComSigs(std::vector<SIGNATURE> &oldSigs, const std::vector<SIGNATURE> &newSigs)
{
	std::map<SIGNATURE, size_t> sigToCountMap;
	for (std::vector<SIGNATURE>::iterator i = oldSigs.begin(); i != oldSigs.end(); ++i)
	{
		++sigToCountMap[*i];
	}
	oldSigs.clear();
	for (std::vector<SIGNATURE>::const_iterator i = newSigs.begin(); i != newSigs.end(); ++i)
	{
		++sigToCountMap[*i];
	}
	for (std::map<SIGNATURE, size_t>::iterator i = sigToCountMap.begin(); i != sigToCountMap.end(); ++i)
	{
		if (i->second == 2)
		{
			oldSigs.push_back(i->first);
		}
	}
}

void PutInBySig(const std::vector<RULECHAIN> &chainSet, std::vector<CHAINGROUP> &vecChainGroups, CResNew &res, std::vector<size_t> &vecWaitForGroup)
{
	std::vector<size_t> vecWaitForGroupCopy(vecWaitForGroup.begin(), vecWaitForGroup.end());
	vecWaitForGroup.clear();
	for (std::vector<size_t>::iterator i = vecWaitForGroupCopy.begin(); i != vecWaitForGroupCopy.end(); ++i)
	{
		std::cout << "PutInBySig" << std::endl;
		std::cout << "One" << std::endl;
		std::cout << "NO: " << i - vecWaitForGroupCopy.begin() << std::endl;
		std::cout << "Total: " << vecWaitForGroupCopy.size() << std::endl << std::endl;
		vecWaitForGroup.push_back(*i);
		for (std::vector<CHAINGROUP>::iterator j = vecChainGroups.begin(); j != vecChainGroups.end(); ++j)
		{
			std::vector<SIGNATURE>::iterator k = std::find_first_of(j->comSigs.begin(), j->comSigs.end(), chainSet[*i].sigs.begin(), chainSet[*i].sigs.end());
			if (k != j->comSigs.end())
			{
				std::vector<CDfanew> vecDfas;
				vecDfas.push_back(res.GetDfaTable()[j->mergeDfaId]);
				vecDfas.push_back(res.GetDfaTable()[*i]);
				CDfanew MergeDfa;
				if (NOrMerge(vecDfas, MergeDfa))
				{
					j->chainIds.push_back(*i);
					res.GetDfaTable().PushBack(MergeDfa);
					j->mergeDfaId = res.GetDfaTable().Size() - 1;
					vecWaitForGroup.pop_back();
					UpdateComSigs(j->comSigs, chainSet[*i].sigs);
					break;
				}
			}
		}
	}
}

void BuildGroupBySig(const std::vector<RULECHAIN> &chainSet, std::vector<CHAINGROUP> &vecChainGroups, CResNew &res, std::vector<size_t> &vecWaitForGroup)
{
	std::vector<CHAINGROUP> newGroups;
	std::vector<size_t> visited(vecWaitForGroup.size(), 0);
	size_t idx = 0;
	for (std::vector<size_t>::iterator i = vecWaitForGroup.begin(); i != vecWaitForGroup.end(); ++i, ++idx)
	{
		if (visited[idx])
		{
			continue;
		}
		visited[idx] = 1;
		newGroups.push_back(CHAINGROUP());
		CHAINGROUP &oneGroup = newGroups.back();
		oneGroup.chainIds.push_back(*i);
		oneGroup.comSigs = chainSet[*i].sigs;
		for (std::vector<size_t>::iterator j = i + 1; j != vecWaitForGroup.end(); ++j)
		{
			std::vector<SIGNATURE>::iterator k = std::find_first_of(oneGroup.comSigs.begin(), oneGroup.comSigs.end(), chainSet[*j].sigs.begin(), chainSet[*j].sigs.end());
			if (k != oneGroup.comSigs.end())
			{
				visited[j - vecWaitForGroup.begin()] = 1;
				oneGroup.chainIds.push_back(*j);
				UpdateComSigs(oneGroup.comSigs, chainSet[*j].sigs);
			}
		}
	}

	vecWaitForGroup.clear();
	for (std::vector<CHAINGROUP>::iterator i = newGroups.begin(); i != newGroups.end(); )
	{
		if (i->chainIds.size() == 1)
		{
			vecWaitForGroup.push_back(i->chainIds[0]);
			i = newGroups.erase(i);
		}
		else
		{
			++i;
		}
	}
	MergeMore(newGroups, res, vecWaitForGroup);
	ExtractSigs(chainSet, newGroups);
	vecChainGroups.insert(vecChainGroups.end(), newGroups.begin(), newGroups.end());
	for (std::vector<size_t>::iterator i = vecWaitForGroup.begin(); i != vecWaitForGroup.end(); ++i)
	{
		vecChainGroups.push_back(CHAINGROUP());
		CHAINGROUP &oneGroup = vecChainGroups.back();
		oneGroup.chainIds.push_back(*i);
		oneGroup.comSigs = chainSet[*i].sigs;
		oneGroup.mergeDfaId = *i;
	}
}

int main(void)
{
	//Load all rules chains from file
	CTimer t1, tAll;

	std::cout << "Loading results from file..." << std::endl;
	CResNew res;
	res.ReadFromFile(_T("..\\..\\output\\result.cdt"));
	std::cout << "Completed in " << t1.Reset() << " Sec. chains: " << res.GetRegexTbl().Size() << std::endl << std::endl;

	std::cout << "Build sidToDfaIdsMap from result..." << std::endl;
	std::map<size_t, std::vector<size_t>> sidToDfaIdsMap;
	BuildSidToDfaIdsMap(res, sidToDfaIdsMap);
	std::cout << "Completed in " << t1.Reset() << " Sec." << std::endl << std::endl;

	std::cout << "Build dfaIdToSidMap from result..." << std::endl;
	std::map<size_t, size_t> dfaIdToSidMap;
	BuildDfaIdToSidMap(res, dfaIdToSidMap);
	std::cout << "Completed in " << t1.Reset() << " Sec." << std::endl << std::endl;

	std::cout << "Build chainSet from result..." << std::endl;
	std::vector<RULECHAIN> chainSet;
	BuildChainSet(res, chainSet);
	std::cout << "Completed in " << t1.Reset() << " Sec." << std::endl << std::endl;

	// Build chain trait map
	std::cout << "Building a trait map for a trait to a vector of chain index numbers..." << std::endl;
	TRAITMAP traitMap;
	BuildChainTraitMap(chainSet, traitMap);
	std::cout << "Completed in " << t1.Reset() << " Sec. Traits: " << traitMap.size() << std::endl << std::endl;

	//Start preliminary grouping...
	std::cout << "Start preliminary grouping..." << std::endl;
	std::vector<CHAINGROUP> vecChainGroups;
	std::vector<size_t> vecWaitForGroup;
	PreGroup(chainSet, traitMap, vecChainGroups, vecWaitForGroup);
	std::cout << "Completed in " << t1.Reset() << " Sec." << std::endl << std::endl;

	//Start farther grouping...
	std::cout << "Start farther grouping..." << std::endl;
	FartherGroup(chainSet, vecChainGroups, vecWaitForGroup);
	std::cout << "Completed in " << t1.Reset() << " Sec." << std::endl << std::endl;

	//Sort chainid in each group by dfa size...
	std::cout << "Sort chainid in each group by dfa size..." << std::endl;
	SortChainId(vecChainGroups, chainSet);
	std::cout << "Completed in " << t1.Reset() << " Sec." << std::endl << std::endl;

	//Extract explosion chains...
	std::cout << "Extract explosion chains..." << std::endl;
	std::vector<size_t> vecExplosion;
	ExtractExplosion(chainSet, vecChainGroups, vecWaitForGroup, vecExplosion);
	std::cout << "Completed in " << t1.Reset() << " Sec." << std::endl << std::endl;

	//Merge dfa in a group...
	std::cout << "Merge dfa in a group..." << std::endl;
	MergeMore(vecChainGroups, res, vecWaitForGroup);
	std::cout << "Completed in " << t1.Reset() << " Sec." << std::endl << std::endl;

	//Extract common Signatures in each group...
	std::cout << "Extract common Signatures in each group..." << std::endl;
	ExtractSigs(chainSet, vecChainGroups);
	std::cout << "Completed in " << t1.Reset() << " Sec." << std::endl << std::endl;

	//Put chain in group which belong to a same rule...
	std::cout << "Put chain in group which belong to a same rule..." << std::endl;
	PutInByRule(dfaIdToSidMap, sidToDfaIdsMap, vecChainGroups, res, vecWaitForGroup);
	std::cout << "Completed in " << t1.Reset() << " Sec." << std::endl << std::endl;

	//Put chain in group which have the same signature...
	std::cout << "Put chain in group which have the same signature..." << std::endl;
	PutInBySig(chainSet, vecChainGroups, res, vecWaitForGroup);
	std::cout << "Completed in " << t1.Reset() << " Sec." << std::endl << std::endl;

	//New group which have the same signature...
	std::cout << "Put chain in group which have the same signature..." << std::endl;
	BuildGroupBySig(chainSet, vecChainGroups, res, vecWaitForGroup);
	std::cout << "Completed in " << t1.Reset() << " Sec." << std::endl << std::endl;

	std::cout << vecChainGroups.size() << std::endl;
	std::cout << "Total time: " << tAll.Reset() << " Sec." << std::endl;

	system("pause");
	return 0;
}