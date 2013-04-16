#include "stdafx.h"
#include "../rule2nfa/rule2nfa.h"

struct RULETRAITS
{
	enum {TRAITS_LEN = 16};
	char szTraits[TRAITS_LEN];
};

bool operator == (const RULETRAITS &r1, const RULETRAITS &r2)
{
	__int64 *p1 = (__int64*)r1.szTraits;
	__int64 *p2 = (__int64*)r2.szTraits;
	for (size_t i = 0; i < RULETRAITS::TRAITS_LEN / sizeof(__int64); ++i)
	{
		if (p1[i] != p2[i])
		{
			return false;
		}
	}
	return true;
}

struct RULETRAITSHASH
{
	enum {HASH_SIZE = 100000};
	size_t operator()(const RULETRAITS &rt)
	{
		const size_t *p = (size_t*)rt.szTraits;
		size_t n = 1;
		for (size_t i = 0; i < RULETRAITS::TRAITS_LEN / sizeof(size_t); ++i)
		{
			n *= p[i];
		}

		return n % HASH_SIZE;
	}
};

typedef std::unordered_map<RULETRAITS, std::vector<size_t>, RULETRAITSHASH> TRAITSMAP;

struct MYRESULT
{
	TRAITSMAP myMap;
	std::vector<CSnortRule> mySet;
};

void CALLBACK RuleReciever(const CSnortRule &rule, LPVOID lpParam)
{
	RULETRAITS rt;
	TRAITSMAP &mymap = ((MYRESULT*)lpParam)->myMap;

	((MYRESULT*)lpParam)->mySet.push_back(rule);

	for (size_t i = 0; i < rule.Size(); ++i)
	{
		size_t nLen = rule[i]->GetPattern(NULL, 0);
		std::string strPat;
		strPat.resize(nLen + 1);
		rule[i]->GetPattern(&strPat[0], nLen);

		for (size_t i = RULETRAITS::TRAITS_LEN; i < nLen; ++i)
		{
			CopyMemory(rt.szTraits, &strPat[i - RULETRAITS::TRAITS_LEN], RULETRAITS::TRAITS_LEN);
			TRAITSMAP::iterator irt = mymap.find(rt);
			if (irt == mymap.end())
			{
				std::vector<size_t> newVec;
				newVec.push_back(rule.GetSid());
				mymap.insert(std::make_pair(rt, newVec));
			}
			else
			{
				irt->second.push_back(rule.GetSid());
			}
		}
	}
}

int main(void)
{
	std::ofstream fs("..\\out.tmp");


	MYRESULT myResult;
	myResult.myMap.rehash(RULETRAITSHASH::HASH_SIZE);
	CompileRuleSet(_T("..\\allrules.rule"), RuleReciever, &myResult);


	for (TRAITSMAP::iterator i = myResult.myMap.begin(); i != myResult.myMap.end(); ++i)
	{
		std::vector<size_t> &iVec = i->second;
		std::sort(iVec.begin(), iVec.end());
		iVec.erase(std::unique(iVec.begin(), iVec.end()), iVec.end());
	}

	for (std::vector<CSnortRule>::iterator i = myResult.mySet.begin(); i != myResult.mySet.end(); ++i)
	{
		std::vector<size_t> iNeighbors;
		bool bHasSig = false;
		for (size_t j = 0; j < i->Size(); ++j)
		{
			size_t nLen = (*i)[j]->GetPattern(NULL, 0);
			std::string strPat;
			strPat.resize(nLen + 1);
			(*i)[j]->GetPattern(&strPat[0], nLen);

			RULETRAITS rt;
			for (size_t k = RULETRAITS::TRAITS_LEN; k < nLen; ++k)
			{
				CopyMemory(rt.szTraits, &strPat[k - RULETRAITS::TRAITS_LEN], RULETRAITS::TRAITS_LEN);
				std::vector<size_t> &nei = myResult.myMap[rt];
				iNeighbors.insert(iNeighbors.end(), nei.begin(), nei.end());
				bHasSig = true;
			}
		}
		std::sort(iNeighbors.begin(), iNeighbors.end());
		iNeighbors.erase(std::unique(iNeighbors.begin(), iNeighbors.end()), iNeighbors.end());

		if (bHasSig)
		{
			fs << std::endl << "sid=" << i->GetSid() << ": " << iNeighbors.size();
		}
	}
	system("pause");
	return 0;
}