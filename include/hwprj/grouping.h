/*
**	@file		grouping.h
**
**	@author		Lab 435, Xidian University
**
**	@brief		Grouping algorithm
**
**	Declaration of CGROUPS and CGROUPRes class
**
*/

#pragma once

#include <hwprj\unsary.h>
#include <hwprj\dfaarray.h>
#include <hwprj\signatures.h>
#include <hwprj\siddfaids.h>

#ifndef GROUPHDR_DS
#define GROUPHDR __declspec(dllimport)
#else
#define GROUPHDR __declspec(dllexport)
#endif

struct DFAINFO
{
	std::vector<SIGNATURE> Sigs;
};

struct ONEGROUP
{
	CUnsignedArray DfaIds;
	CSignatures ComSigs;
	SIGNATURE currSig;
	ulong mergeDfaId;
};

class GROUPHDR CGROUPS
{
public:
	CGROUPS();
	CGROUPS(const CGROUPS& other);
	const CGROUPS &operator=(const CGROUPS &other);
	~CGROUPS();
	const ulong Size() const;
	void Resize(ulong nSize);
	void PushBack(ONEGROUP oneGroup);
	ONEGROUP& Back();
	ONEGROUP &operator[](ulong nIdx);
	const ONEGROUP &operator[](ulong nIdx) const;
	void Clear();
	void Erase(ulong nIdx);
protected:
	std::vector<ONEGROUP> *m_pGroups;
};

class GROUPHDR CGROUPRes
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
	ulong WriteToFile(const char *filename);
	ulong ReadFromFile(const char *filename);
};

GROUPHDR void Grouping(class CCompileResults &res, CGROUPRes &groupRes);
