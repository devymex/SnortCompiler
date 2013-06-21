/*
**	@file		pcrechain.h
**
**	@author		Lab 435, Xidian University
**
**	@brief		Construct pcre chain with attribute of snort rules
**
**	Declaration of the CPcreChain class
**
*/

#pragma once

#include <hwprj\pcreopt.h>

#ifndef SNORTRULEHDR_DS
class PCREVEC;
#endif

class SNORTRULEHDR CPcreChain
{
public:
	CPcreChain();
	CPcreChain(const CPcreChain &other);
	virtual ~CPcreChain();

	CPcreChain&				operator = (const CPcreChain &other);
	CPcreOption&			operator[](ulong nIdx);
	const CPcreOption&		operator[](ulong nIdx) const;

	ulong					Size() const;
	void					Clear();
	CPcreOption&			Back() const;
	void					PushBack(const CPcreOption &pcreOpt);
	void					Resize(ulong nSize);

	CUnsignedArray&			GetSigs();
	const CUnsignedArray&	GetSigs() const;

protected:
	PCREVEC*				m_pPcreVec;
	CUnsignedArray			m_sigs;
};
