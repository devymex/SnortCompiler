#pragma once

#include <hwprj\regchain.h>

class REGRULEHDR CRegRule
{
public:
	CRegRule();
	~CRegRule();
	CRegRule(const CRegRule &other);
	const CRegRule& operator = (const CRegRule &other);

	CRegChain& operator[](ULONG nIdx);
	const CRegChain& operator[](ULONG nIdx) const;

	ULONG Size() const;
	void Reserve(ULONG nCount);
	void Resize(ULONG nSize);
	CRegChain& Back() const;
	void PushBack(const CRegChain &nRegChain);

protected:
	std::vector<CRegChain> *m_pRegVec;
};
