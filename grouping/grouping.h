#pragma once
#include <unordered_map>
#include "../compilernew/compilernew.h"

#ifndef GROUPING_H_
#define GROUPINGSC __declspec(dllimport)
#else
#define GROUPINGSC __declspec(dllexport)
#endif

#define THRESHOLD 8
#define NUMOFCOMSIGS 2

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

struct CHAINGROUP
{
	std::vector<size_t> chainIds;
	std::string comStr;
	std::vector<SIGNATURE> comSigs;
	std::size_t mergeDfaId;
};

struct GROUP
{
	std::vector<SIGNATURE> vecSigs;
	std::size_t dfaId;
};

class GROUPINGSC CGROUPS
{
public:
	CGROUPS();
	CGROUPS(const CGROUPS& other);
	const CGROUPS &operator=(const CGROUPS &other);
	~CGROUPS();
	GROUP &operator[](size_t index);
	const GROUP &operator[](size_t index) const;
	void Reserve(size_t nCount);
	void Resize(size_t nSize);
	const size_t Size() const;
	void PushBack(const GROUP &oneGroup);
	GROUP &Back();
	void Sort();
	void Clear();
private:
	std::vector<GROUP> *m_pGroups;
};

class GROUPINGSC CGROUPRes
{
private:
	CDfaTblNew m_dfaTbl;
	CSidDfaIdsNew m_sidDfaIds; 
	CGROUPS m_groups;
public:
	CDfaTblNew &GetDfaTable();
	CSidDfaIdsNew &GetSidDfaIds();
	CGROUPS & GetGroups();
	const CDfaTblNew &GetDfaTable() const;
	const CSidDfaIdsNew &GetSidDfaIds() const;
	const CGROUPS &GetGroups() const;
	size_t WriteToFile(LPCTSTR filename);
	size_t ReadFromFile(LPCTSTR filename);
};

GROUPINGSC void UpdateComSigs(std::vector<SIGNATURE> &oldSigs, const std::vector<SIGNATURE> &newSigs);

GROUPINGSC void grouping(CResNew &res, CGROUPRes &groupRes);
