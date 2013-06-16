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

#include <hwprj\dllstring.h>
#include <hwprj\signatures.h>

#ifndef REGRULEHDR_DS
class STRINGVEC;
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

	CRegChain& operator = (const CRegChain &other);
	CDllString& operator[](ulong nIdx);
	const CDllString& operator[](ulong nIdx) const;

	ulong Size() const;
	CDllString& Back() const;
	void PushBack(const CDllString &pcreStr);
	void Resize(ulong nSize);

	CSignatures& GetSigs();
	const CSignatures& GetSigs() const;

protected:
	STRINGVEC *m_pRegList;
	CSignatures m_sigs;
};
