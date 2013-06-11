/**
**  @file        common.h
**
**  @author      Lab 435, Xidian University
**
**  @brief       Common classes declaration
**
**  Include CUnsignedArray, CDllString
**
*/

#pragma once

#include <vector>
#include <hwprj\common.h>

#ifndef SIGHDR_DS
#define SIGHDR __declspec(dllimport)
#else
#define SIGHDR __declspec(dllexport)
#endif

typedef unsigned long SIGNATURE;

class SIGHDR CSignatures
{
public:
	CSignatures();
	CSignatures(const CSignatures& other);
	const CSignatures &operator=(const CSignatures &other);
	~CSignatures();

	const ulong Size() const;
	void Resize(ulong nSize);
	void PushBack(SIGNATURE Sig);
	SIGNATURE &operator[](ulong nIdx);
	const SIGNATURE &operator[](ulong nIdx) const;
	void Clear();
	void Unique();
protected:
	std::vector<SIGNATURE> *m_pSigs;
};
