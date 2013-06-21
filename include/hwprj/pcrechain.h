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

#include <hwprj\signatures.h>
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

	CPcreChain&			operator = (const CPcreChain &other);
	CPcreOption&		operator[](ulong nIdx);
	const CPcreOption&	operator[](ulong nIdx) const;

	ulong				Size() const;
	void				Clear();
	CPcreOption&		Back() const;
	void				PushBack(const CPcreOption &pcreOpt);
	void				Resize(ulong nSize);

	/*	函数名：GetSigs
	**	功能：获取一个PCRE链的特征字符串signature
	*/
	CSignatures&		GetSigs();
	const CSignatures&	GetSigs() const;

protected:
	PCREVEC*			m_pPcreVec;
	CSignatures			m_sigs;
};
