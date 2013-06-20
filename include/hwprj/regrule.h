/*
**	@file		regrule.h
**
**	@author		Lab 435, Xidian University
**
**	@brief		Declaration of the CRegRule class
**
**	A rule includes multiple CPcreChain, all store in CRegRule
**
*/

#pragma once

#include <hwprj\pcrechain.h>

#ifndef SNORTRULEHDR_DS
class CHAINVEC;
#endif

class SNORTRULEHDR CRegRule
{
public:
	CRegRule();
	CRegRule(const CRegRule &other);
	virtual ~CRegRule();

	CRegRule&			operator = (const CRegRule &other);
	CPcreChain&			operator[](ulong nIdx);
	const CPcreChain&	operator[](ulong nIdx) const;

	ulong				Size() const;
	void				Erase(ulong ulIdx);
	void				Reserve(ulong nCount);
	void				Resize(ulong nSize);
	CPcreChain&			Back() const;
	void				PushBack(const CPcreChain &nRegChain);
	void				PopBack();

protected:
	CHAINVEC*			m_pRegVec;
};
