/**
**	@file		signatures.h
**
**	@author		Lab 435, Xidian University
**
**	@brief		Declaration of CSignatures class
**
**	Extract signature from "content" and "pcre"
**	Store mapping between signature and rule
**
*/

#pragma once

#include <hwprj\common.h>

#ifndef SIGHDR_DS
#define SIGHDR __declspec(dllimport)
#else
#define SIGHDR __declspec(dllexport)
#endif

#ifndef SIGHDR_DS
class SIGNATUREVEC;
#endif

class SIGHDR CSignatures
{
public:
	CSignatures();
	CSignatures(const CSignatures& other);
	CSignatures &operator=(const CSignatures &other);

	bool operator == (const CSignatures &other) const;
	bool operator <	(const CSignatures &other) const;

	virtual ~CSignatures();

	const ulong Size() const;
	void Resize(ulong nSize);
	void PushBack(SIGNATURE Sig);
	SIGNATURE &operator[](ulong nIdx);
	const SIGNATURE &operator[](ulong nIdx) const;
	void Clear();
	void Unique();
	ulong Find(SIGNATURE Sig) const;
protected:
	SIGNATUREVEC *m_pSigs;
};
