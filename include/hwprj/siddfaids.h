/*
**	@file		siddfas.h
**
**	@author		Lab 435, Xidian University
**
**	@brief		Declaration of the CSidDfaIds class
**
**	Store the mapping between SID and DFAID
**
*/

#pragma once

#include <hwprj\unsary.h>

#ifndef SIDDFAHDR_DS
#define SIDDFAHDR __declspec(dllimport)
#else
#define SIDDFAHDR __declspec(dllexport)
#endif

struct COMPILEDINFO
{
	typedef ulong COMPRETCODE;
	static const COMPRETCODE RES_SUCCESS		= 0;
	static const COMPRETCODE RES_PCREERROR		= (1 << 0); 
	static const COMPRETCODE RES_OPTIONERROR	= (1 << 1); 
	static const COMPRETCODE RES_HASBYTE		= (1 << 2); 
	static const COMPRETCODE RES_HASNOT			= (1 << 3); 
	static const COMPRETCODE RES_EMPTY			= (1 << 4);
	static const COMPRETCODE RES_HASNOSIG		= (1 << 5);
	static const COMPRETCODE RES_EXCEEDLIMIT	= (1 << 6);
	ulong m_nSid;
	ulong m_nResult;
	CUnsignedArray m_dfaIds;
};

class SIDDFAHDR CSidDfaIds
{
public:
	CSidDfaIds();
	CSidDfaIds(const CSidDfaIds& other);
	CSidDfaIds &operator=(const CSidDfaIds &other);
	virtual ~CSidDfaIds();

	COMPILEDINFO& operator[](ulong index);
	const COMPILEDINFO& operator[](ulong index) const;
	void Reserve(ulong nCount);
	void Resize(ulong nSize);
	const ulong Size() const;
	void PushBack(const COMPILEDINFO &sidDfaIds);
	COMPILEDINFO& Back();
protected:
	std::vector<COMPILEDINFO> *m_ruleResult;
};
