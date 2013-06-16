/*
**	@file		regrule.h
**
**	@author		Lab 435, Xidian University
**
**	@brief		Declaration of the CRegRule class
**
**	A rule includes multiple CRegChain, all store in CRegRule
**
*/

#pragma once

#include <hwprj\regchain.h>

#ifndef REGRULEHDR_DS
class CHAINVEC;
#endif

class REGRULEHDR CRegRule
{
public:
	CRegRule();
	CRegRule(const CRegRule &other);
	virtual ~CRegRule();

	CRegRule&			operator = (const CRegRule &other);
	CRegChain&			operator[](ulong nIdx);
	const CRegChain&	operator[](ulong nIdx) const;

	ulong				Size() const;
	void				Reserve(ulong nCount);
	void				Resize(ulong nSize);
	CRegChain&			Back() const;
	void				PushBack(const CRegChain &nRegChain);
	void				PopBack();

protected:
	CHAINVEC*			m_pRegVec;
};
