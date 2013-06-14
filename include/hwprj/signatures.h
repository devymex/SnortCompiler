/**
**	@file		signatures.h
**
**	@author		Lab 435, Xidian University
**
**	@brief		Common classes declaration
**
**	Include CUnsignedArray, CDllString
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
	const CSignatures &operator=(const CSignatures &other);

	bool operator == (const CSignatures &other) const;
	bool operator <	(const CSignatures &other) const;

	~CSignatures();

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
