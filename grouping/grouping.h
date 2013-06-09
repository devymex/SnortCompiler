#pragma once
#include "../common/common.h"
#include "../compilernew/compilernew.h"

#ifndef GROUPING_H_
#define GROUPINGSC __declspec(dllimport)
#else
#define GROUPINGSC __declspec(dllexport)
#endif

struct DFAINFO
{
	std::vector<SIGNATURE> Sigs;
};

struct ONEGROUP
{
	CVectorUnsigned DfaIds;
	CSignatures ComSigs;
	SIGNATURE currSig;
	std::size_t mergeDfaId;
};

class GROUPINGSC CGROUPS
{
public:
	CGROUPS();
	CGROUPS(const CGROUPS& other);
	const CGROUPS &operator=(const CGROUPS &other);
	~CGROUPS();
	const size_t Size() const;
	void Resize(size_t nSize);
	void PushBack(ONEGROUP oneGroup);
	ONEGROUP& Back();
	ONEGROUP &operator[](size_t nIdx);
	const ONEGROUP &operator[](size_t nIdx) const;
	void Clear();
	void Erase(size_t nIdx);
protected:
	std::vector<ONEGROUP> *m_pGroups;
};

class GROUPINGSC CGROUPRes
{
protected:
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

GROUPINGSC void grouping(CResNew &res, CGROUPRes &groupRes);