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
	std::string strChain;
	std::vector<SIGNATURE> sigs;
};

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

size_t LongestCommonString(const std::string &str1, const std::string &str2)
{
	size_t count = 0;
	size_t min = str1.length() < str2.length() ? str1.length() : str2.length();
	while (count < min && str1[count] == str2[count])
	{
		++count;
	}
	return count;
}

struct CHAINGROUP
{
	std::vector<size_t> chainIds;
	std::string comStr;
};

struct DFASIZE
{
	std::size_t dfaId;
	std::size_t dfaSize;
};

struct DFAGROUP
{
	std::vector<size_t> dfaIds;
	std::size_t mergeDfaId;
};

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

void BuildSimilarityMatrix(const std::vector<RULECHAIN> &chainSet, const TRAITMAP &traitMap, size_t *pChainMat, size_t nChainMatDem)
{
	for (std::vector<RULECHAIN>::const_iterator i = chainSet.begin(); i != chainSet.end(); ++i)
	{
		CHAINTRAIT trait;
		if (!ExtractTraits(*i, trait))
		{
			continue;
		}

		// Compute LCS of every pair of correlative lists
		TRAITMAP::const_iterator irt = traitMap.find(trait);
		for (std::vector<size_t>::const_iterator j = irt->second.begin(); j != irt->second.end(); ++j)
		{
			size_t y = i - chainSet.begin(), x = *j;
			if (y < x)
			{
				//if (std::find_first_of(i->sigs.begin(), i->sigs.end(), listSet[*j].sigs.begin(), listSet[*j].sigs.end()) != i->sigs.end())
				{
					pChainMat[x * nChainMatDem + y] = pChainMat[y * nChainMatDem + x] = LongestCommonString(chainSet[x].strChain, chainSet[y].strChain);
				}
			}
		}
	}
}
void PreGroup(const std::vector<RULECHAIN> &chainSet, size_t* pChainMat, const size_t &nChainMatDem, std::vector<CHAINGROUP> &vecChainGroups, std::vector<size_t> &vecWaitingForGroup)
{
	std::vector<size_t> vecDistributed;
	size_t min;
	for (size_t i = 0; i < nChainMatDem; ++i)
	{
		vecChainGroups.push_back(CHAINGROUP());
		CHAINGROUP &group = vecChainGroups.back();
		group.chainIds.push_back(i);
		min = chainSet[i].strChain.length();
		for (size_t j = 0; j < nChainMatDem; ++j)
		{
			if (pChainMat[i * nChainMatDem + j] != 0)
			{
				group.chainIds.push_back(j);
				if (pChainMat[i * nChainMatDem + j] < min)
				{
					min = pChainMat[i * nChainMatDem + j];
				}
			}
		}
		if (group.chainIds.size() == 1)
		{
			vecChainGroups.pop_back();
		}
		if (group.chainIds.size() > 1)
		{
			group.comStr = chainSet[i].strChain.substr(0, min);
			for (size_t j = 0; j < group.chainIds.size(); ++j)
			{
				vecDistributed.push_back(group.chainIds[j]);
				for (size_t k = 0; k < nChainMatDem; ++k)
				{
					pChainMat[group.chainIds[j] * nChainMatDem + k] = 0;
					pChainMat[k * nChainMatDem + group.chainIds[j]] = 0;
				}
			}
		}
	}
	for (size_t i = 0; i < nChainMatDem; ++i)
	{
		if (std::find(vecDistributed.begin(), vecDistributed.end(), i) == vecDistributed.end())
		{
			vecWaitingForGroup.push_back(i);
		}
	}
}

void FartherGroup(const std::vector<RULECHAIN> &chainSet, std::vector<CHAINGROUP> &vecChainGroups, const std::vector<size_t> &vecWaitingForGroup)
{
	size_t max;
	size_t tmp;
	size_t index;
	for (std::vector<size_t>::const_iterator i = vecWaitingForGroup.begin(); i != vecWaitingForGroup.end(); ++i)
	{
		max = 0;
		index = 0;
		for (size_t j = 0; j < vecChainGroups.size(); ++j)
		{
			tmp = LongestCommonString(vecChainGroups[j].comStr, chainSet[*i].strChain);
			if (max < tmp || (max == tmp && tmp == vecChainGroups[j].comStr.length()))
			{
				max = tmp;
				index = j;
			}
		}
		if (max >= THRESHOLD || (max == chainSet[*i].strChain.length() && max == vecChainGroups[index].comStr.length()))
		{
			vecChainGroups[index].chainIds.push_back(*i);
			vecChainGroups[index].comStr.substr(0, max);
		}
		else
		{
			vecChainGroups.push_back(CHAINGROUP());
			CHAINGROUP &group = vecChainGroups.back();
			group.chainIds.push_back(*i);
			group.comStr = chainSet[*i].strChain;
		}
	}
}

void ChainIdTODfaId(const std::vector<CHAINGROUP> &vecChainGroups, const CResNew &res, std::vector<std::vector<DFASIZE>> &vecDfaSize)
{
	for (std::vector<CHAINGROUP>::const_iterator i = vecChainGroups.begin(); i != vecChainGroups.end(); ++i)
	{
		std::vector<DFASIZE> &dfaIds = vecDfaSize[i - vecChainGroups.begin()];
		for (std::vector<size_t>::const_iterator j = i->chainIds.begin(); j != i->chainIds.end(); ++j)
		{
			for (size_t k = 0; k < res.GetDfasInfo().Size(); ++k)
			{
				if (res.GetDfasInfo()[k].chainId == *j)
				{
					dfaIds.push_back(DFASIZE());
					dfaIds.back().dfaId = res.GetDfasInfo()[k].dfaId;
					dfaIds.back().dfaSize = res.GetDfaTable()[dfaIds.back().dfaId].Size() * res.GetDfaTable()[dfaIds.back().dfaId].GetGroupCount();
				}
			}
		}
	}
	struct COMP
	{
		bool operator()(const DFASIZE &id1, const DFASIZE &id2)
		{
			return id1.dfaSize < id2.dfaSize;
		}
	};
	for (size_t i = 0; i < vecDfaSize.size(); ++i)
	{
		std::sort(vecDfaSize[i].begin(), vecDfaSize[i].end(), COMP());
	}
}

void Extract(const std::vector<std::vector<DFASIZE>> &vecDfaSize, std::vector<size_t> &vecExplosion, std::vector<size_t> &vecOne, std::vector<std::vector<size_t>> &vecMore)
{
	for (std::vector<std::vector<DFASIZE>>::const_iterator i = vecDfaSize.begin(); i != vecDfaSize.end(); ++i)
	{
		for (std::vector<DFASIZE>::const_iterator j = i->begin(); j != i->end(); ++j)
		{
			if (j->dfaSize == 0)
			{
				vecExplosion.push_back(j->dfaId);
			}
			else if (i->end() - j == 1)
			{
				vecOne.push_back(j->dfaId);
			}
			else
			{
				vecMore.push_back(std::vector<size_t>());
				while (j != i->end())
				{
					vecMore.back().push_back(j->dfaId);
					++j;
				}
				break;
			}
		}
	}
}

void Merge(std::vector<DFAGROUP> &vecGroupRes, CResNew &res, std::vector<size_t> &vecOne, std::vector<std::vector<size_t>> &vecMore)
{
	vecGroupRes.reserve(10000);
	bool mergeFlag;
	std::vector<CDfanew> vecDfas(2);
	CDfanew MergeDfa;
	for (size_t i = 0; i < vecMore.size(); ++i)
	{
		std::cout << "NO: " << i << std::endl;
		std::cout << "Total: " << vecMore.size() << std::endl << std::endl;
		std::vector<CDfanew> vecDfas(2);
		vecDfas[0] = res.GetDfaTable()[vecMore[i][0]];
		vecDfas[1] = res.GetDfaTable()[vecMore[i][1]];
		if (!NOrMerge(vecDfas, MergeDfa))
		{
			for (size_t j = 0; j < vecMore[i].size(); ++j)
			{
				vecOne.push_back(vecMore[i][j]);
			}
			vecMore.erase(vecMore.begin() + i);
			continue;
		}
		mergeFlag = true;
		vecDfas[0] = MergeDfa;
		for (size_t j = 2; j < vecMore[i].size(); ++j)
		{
			vecDfas[1] = res.GetDfaTable()[vecMore[i][j]];
			if (!NOrMerge(vecDfas, MergeDfa))
			{
				mergeFlag = false;
				res.GetDfaTable().PushBack(vecDfas[0]);
				vecGroupRes.resize(vecGroupRes.size() + 1);
				DFAGROUP &oneGroup = vecGroupRes.back();
				for (size_t k = 0; k < j; ++k)
				{
					oneGroup.dfaIds.push_back(k);
				}
				oneGroup.mergeDfaId = res.GetDfaTable().Size() - 1;
				if (vecMore[i].size() - j == 1)
				{
					vecOne.push_back(vecMore[i][j]);
					vecMore[i].erase(vecMore[i].begin() + j);
				}
				else
				{
					vecMore.push_back(std::vector<size_t>());
					size_t nId = j;
					while (j < vecMore[i].size())
					{
						vecMore.back().push_back(vecMore[i][j]);
						++j;
					}
					vecMore[i].erase(vecMore[i].begin() + nId, vecMore[i].end());
				}
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
			vecGroupRes.resize(vecGroupRes.size() + 1);
			DFAGROUP &oneGroup = vecGroupRes.back();
			oneGroup.dfaIds.insert(oneGroup.dfaIds.begin(), vecMore[i].begin(), vecMore[i].end());
			oneGroup.mergeDfaId = res.GetDfaTable().Size() - 1;
		}
	}
	for (size_t i = 0; i < vecOne.size();)
	{
		vecDfas[0] = res.GetDfaTable()[vecOne[i]];
		mergeFlag = true;
		for (size_t j = i + 1; j < vecOne.size(); ++j)
		{
			vecDfas[1] = res.GetDfaTable()[vecOne[j]];
			if (!NOrMerge(vecDfas, MergeDfa))
			{
				mergeFlag = false;
				if (j - i == 1)
				{
					vecGroupRes.resize(vecGroupRes.size() + 1);
					DFAGROUP &oneGroup = vecGroupRes.back();
					oneGroup.dfaIds.push_back(i);
					oneGroup.mergeDfaId = vecOne[i];
				}
				else
				{
					res.GetDfaTable().PushBack(vecDfas[0]);
					vecGroupRes.resize(vecGroupRes.size() + 1);
					DFAGROUP &oneGroup = vecGroupRes.back();
					for (size_t k = i; k < j; ++k)
					{
						oneGroup.dfaIds.push_back(k);
					}
					oneGroup.mergeDfaId = res.GetDfaTable().Size() - 1;
				}
				i = j;
				break;
			}
			else
			{
				vecDfas[0] = MergeDfa;
			}
		}
		if (mergeFlag)
		{
			if (i == vecOne.size() - 1)
			{
				vecGroupRes.resize(vecGroupRes.size() + 1);
				DFAGROUP &oneGroup = vecGroupRes.back();
				oneGroup.dfaIds.push_back(i);
				oneGroup.mergeDfaId = vecOne[i];
			}
			else
			{
				res.GetDfaTable().PushBack(vecDfas[0]);
				vecGroupRes.resize(vecGroupRes.size() + 1);
				DFAGROUP &oneGroup = vecGroupRes.back();
				for (size_t k = i; k < vecOne.size(); ++k)
				{
					oneGroup.dfaIds.push_back(k);
				}
				oneGroup.mergeDfaId = res.GetDfaTable().Size() - 1;
			}
		}
	}
}

int main(void)
{
	//Load all rules chains from file
	CTimer t1, tAll;

	std::cout << "Loading results from file..." << std::endl;
	CResNew res;
	res.ReadFromFile(_T("..\\..\\output\\result(back_up).cdt"));
	std::cout << "Completed in " << t1.Reset() << " Sec. chains: " << res.GetRegexTbl().Size() << std::endl << std::endl;

	std::cout << "Build chainSet from result..." << std::endl;
	std::vector<RULECHAIN> chainSet;
	BuildChainSet(res, chainSet);
	std::cout << "Completed in " << t1.Reset() << " Sec." << std::endl << std::endl;

	// Build chain trait map
	std::cout << "Building a trait map for a trait to a vector of chain index numbers..." << std::endl;
	TRAITMAP traitMap;
	BuildChainTraitMap(chainSet, traitMap);
	std::cout << "Completed in " << t1.Reset() << " Sec. Traits: " << traitMap.size() << std::endl << std::endl;

	// Alloc memory for chain similar matrix
	std::cout << "Allocating a simillar matrix of chains (SMC)." << std::endl;
	size_t nChainMatDem = chainSet.size();
	size_t nChainMatSize = nChainMatDem * nChainMatDem;
	size_t *pChainMat = new size_t[nChainMatSize];
	ZeroMemory(pChainMat, nChainMatSize * sizeof(size_t));
	std::cout << "Completed in " << t1.Reset() << " Sec. SMC size (bytes): " << nChainMatSize * sizeof(size_t) << std::endl << std::endl;
	
	// Compute the LCS of every pair of correlative chains;
	std::cout << "Computer similarity of every pair of correlative chains..." << std::endl;
	BuildSimilarityMatrix(chainSet, traitMap, pChainMat, nChainMatDem);
	std::cout << "Completed in " << t1.Reset() << " Sec. " << std::endl << std::endl;

	//Start preliminary grouping...
	std::cout << "Start preliminary grouping..." << std::endl;
	std::vector<CHAINGROUP> vecChainGroups;
	std::vector<size_t> vecWaitingForGroup;
	PreGroup(chainSet, pChainMat, nChainMatDem, vecChainGroups, vecWaitingForGroup);
	std::cout << "Completed in " << t1.Reset() << " Sec." << std::endl << std::endl;

	//Start farther grouping...
	std::cout << "Start farther grouping..." << std::endl;
	FartherGroup(chainSet, vecChainGroups, vecWaitingForGroup);
	std::cout << "Completed in " << t1.Reset() << " Sec." << std::endl << std::endl;

	//Change chainid to dfaid and sort dfaid by dfa size...
	std::cout << "Change chainid to dfaid..." << std::endl;
	std::vector<std::vector<DFASIZE>> vecDfaSize(vecChainGroups.size());
	ChainIdTODfaId(vecChainGroups, res, vecDfaSize);
	std::cout << "Completed in " << t1.Reset() << " Sec." << std::endl << std::endl;

	//Extract explosion, one and more chains in a group...
	std::cout << "Extract explosion, one and more chains in a group..." << std::endl;
	std::vector<size_t> vecExplosion;
	std::vector<size_t> vecOne;
	std::vector<std::vector<size_t>> vecMore;
	Extract(vecDfaSize, vecExplosion, vecOne, vecMore);
	std::cout << "Completed in " << t1.Reset() << " Sec." << std::endl << std::endl;

	//Merge dfa in a group...
	std::cout << "Merge dfa in a group..." << std::endl;
	std::vector<DFAGROUP> vecGroupRes;
	Merge(vecGroupRes, res, vecOne, vecMore);
	std::cout << "Completed in " << t1.Reset() << " Sec." << std::endl << std::endl;

	std::cout << "Total time: " << tAll.Reset() << " Sec." << std::endl;

	system("pause");
	return 0;
}
