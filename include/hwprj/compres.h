#pragma once

#include <hwprj\dfaarray.h>
#include <hwprj\siddfaids.h>
#include <hwprj\regrule.h>

#ifndef COMPRESHDR_DS
#define COMPRESHDR __declspec(dllimport)
#else
#define COMPRESHDR __declspec(dllexport)
#endif

class COMPRESHDR CCompileResults
{
public:
	CDfaArray& GetDfaTable();
	CSidDfaIds& GetSidDfaIds();
	CRegRule &GetRegexTbl();
	const CDfaArray& GetDfaTable() const;
	const CSidDfaIds& GetSidDfaIds() const;
	const CRegRule &GetRegexTbl() const;
	ULONG WriteToFile(LPCTSTR filename);
	ULONG ReadFromFile(LPCTSTR filename);
protected:
	CDfaArray m_dfaTbl;
	CSidDfaIds m_sidDfaIds;
	CRegRule m_RegexTbl;
};
