#include "stdafx.h"
#include <hwprj\regrule.h>

REGRULEHDR CRegRule::CRegRule()
{
	m_pRegVec = new std::vector<CRegChain>;
}

REGRULEHDR CRegRule::~CRegRule()
{
	delete m_pRegVec;
}

REGRULEHDR CRegRule::CRegRule(const CRegRule &other)
{
	m_pRegVec = new std::vector<CRegChain>;
	*this = other;
}

REGRULEHDR ulong CRegRule::Size() const
{
	return m_pRegVec->size();
}

REGRULEHDR CRegChain& CRegRule::Back() const
{
	return m_pRegVec->back();
}

REGRULEHDR void CRegRule::Reserve(ulong nCount)
{
	m_pRegVec->reserve(nCount);
}
REGRULEHDR void CRegRule::Resize(ulong nSize)
{
	m_pRegVec->resize(nSize);
}

REGRULEHDR void CRegRule::PushBack(const CRegChain &nRegChain)
{
	m_pRegVec->push_back(nRegChain);
}

REGRULEHDR CRegChain& CRegRule::operator[](ulong nIdx)
{
	return (*m_pRegVec)[nIdx];
}

REGRULEHDR const CRegChain& CRegRule::operator[](ulong nIdx) const
{
	return (*m_pRegVec)[nIdx];
}

REGRULEHDR const CRegRule& CRegRule::operator = (const CRegRule &other)
{
	*this->m_pRegVec = *other.m_pRegVec;
	return *this;
}
