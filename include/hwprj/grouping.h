#pragma once

#include <hwprj\compiler.h>

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
	CDllArray DfaIds;
	CSignatures ComSigs;
	SIGNATURE currSig;
	ULONG mergeDfaId;
};

class GROUPINGSC CGROUPS
{
public:
	CGROUPS();
	CGROUPS(const CGROUPS& other);
	const CGROUPS &operator=(const CGROUPS &other);
	~CGROUPS();
	const ULONG Size() const;
	void Resize(ULONG nSize);
	void PushBack(ONEGROUP oneGroup);
	ONEGROUP& Back();
	ONEGROUP &operator[](ULONG nIdx);
	const ONEGROUP &operator[](ULONG nIdx) const;
	void Clear();
	void Erase(ULONG nIdx);
protected:
	std::vector<ONEGROUP> *m_pGroups;
};

class GROUPINGSC CGROUPRes
{
protected:
	CDfaArray m_dfaTbl;
	CSidDfaIds m_sidDfaIds; 
	CGROUPS m_groups;
public:
	CDfaArray &GetDfaTable();
	CSidDfaIds &GetSidDfaIds();
	CGROUPS & GetGroups();
	const CDfaArray &GetDfaTable() const;
	const CSidDfaIds &GetSidDfaIds() const;
	const CGROUPS &GetGroups() const;
	ULONG WriteToFile(LPCTSTR filename);
	ULONG ReadFromFile(LPCTSTR filename);
};

GROUPINGSC void Grouping(CCompileResults &res, CGROUPRes &groupRes);
