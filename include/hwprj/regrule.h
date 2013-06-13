#pragma once

#include <hwprj\regchain.h>

#ifndef REGRULEHDR_DS
class CHAINVEC;
#endif

class REGRULEHDR CRegRule
{
public:
	CRegRule();
	~CRegRule();
	CRegRule(const CRegRule &other);
	const CRegRule& operator = (const CRegRule &other);

	CRegChain& operator[](ulong nIdx);
	const CRegChain& operator[](ulong nIdx) const;

	ulong Size() const;
	void Reserve(ulong nCount);
	void Resize(ulong nSize);
	CRegChain& Back() const;
	void PushBack(const CRegChain &nRegChain);

protected:
	CHAINVEC *m_pRegVec;
};
