/*
**	@file		regchain.h
**
**	@author		Lab 435, Xidian University
**
**	@brief		Construct regex chain with attribute of snort rules
**
**	Declaration of the CRegChain class
**
*/

#pragma once

#include <hwprj\signatures.h>
#include <hwprj\pcreopt.h>

#ifndef REGRULEHDR_DS
class PCREVEC;
#define REGRULEHDR __declspec(dllimport)
#else
#define REGRULEHDR __declspec(dllexport)
#endif

class REGRULEHDR CRegChain
{
public:
	CRegChain();
	CRegChain(const CRegChain &other);
	virtual ~CRegChain();

	CRegChain&			operator = (const CRegChain &other);
	CPcreOption&		operator[](ulong nIdx);
	const CPcreOption&	operator[](ulong nIdx) const;

	ulong				Size() const;
	void				Clear();
	CPcreOption&		Back() const;
	void				PushBack(const CPcreOption &pcreOpt);
	void				Resize(ulong nSize);

	CSignatures&		GetSigs();
	const CSignatures&	GetSigs() const;

protected:
	PCREVEC*			m_pPcreVec;
	CSignatures			m_sigs;
};
