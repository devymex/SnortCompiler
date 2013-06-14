#pragma once

#include <hwprj\common.h>
#include <hwprj\dfaarray.h>
#include <hwprj\siddfaids.h>
#include <hwprj\groups.h>

#ifndef GROUPRESHDR_DS
#define GROUPRESHDR __declspec(dllimport)
#else
#define GROUPRESHDR __declspec(dllexport)
#endif

class GROUPRESHDR CGroupRes
{
protected:
	CDfaArray m_dfaTbl;
	CSidDfaIds m_sidDfaIds; 
	CGroups m_groups;
public:
	CDfaArray &GetDfaTable();
	CSidDfaIds &GetSidDfaIds();
	CGroups & GetGroups();
	const CDfaArray &GetDfaTable() const;
	const CSidDfaIds &GetSidDfaIds() const;
	const CGroups &GetGroups() const;
	ulong WriteToFile(const char *filename);
	ulong ReadFromFile(const char *filename);
};