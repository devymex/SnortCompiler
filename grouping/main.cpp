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
	traitMap.rehash(CHAINTRAITHASH::HASH_SIZE);
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

	std::cout << "Total time: " << tAll.Reset() << " Sec." << std::endl;

	system("pause");
	return 0;
}