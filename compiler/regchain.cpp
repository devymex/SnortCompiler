#include "stdafx.h"
#include <hwprj\regchain.h>

REGRULEHDR CRegChain::CRegChain()
{
	m_pRegList = new STRINGVEC;
	CHECKALLOC(m_pRegList);
}

REGRULEHDR CRegChain::~CRegChain()
{
	delete m_pRegList;
}

REGRULEHDR CRegChain::CRegChain(const CRegChain &other)
{
	m_pRegList = new STRINGVEC;
	CHECKALLOC(m_pRegList);
	*this = other;
}

REGRULEHDR ulong CRegChain::Size() const
{
	return m_pRegList->size();
}

REGRULEHDR CDllString& CRegChain::Back() const
{
	return m_pRegList->back();
}

REGRULEHDR void CRegChain::PushBack(const CDllString &pcreStr)
{
	m_pRegList->push_back(pcreStr);
}

REGRULEHDR CDllString& CRegChain::operator[](ulong nIdx)
{
	return (*m_pRegList)[nIdx];
}

REGRULEHDR const CDllString& CRegChain::operator[](ulong nIdx) const
{
	return (*m_pRegList)[nIdx];
}

REGRULEHDR const CRegChain& CRegChain::operator = (const CRegChain &other)
{
	*this->m_pRegList = *other.m_pRegList;
	this->m_sigs = other.m_sigs;
	return *this;
}

REGRULEHDR void CRegChain::Resize(ulong nSize)
{
	m_pRegList->resize(nSize);
}

REGRULEHDR CSignatures& CRegChain::GetSigs()
{
	return m_sigs;
}

REGRULEHDR const CSignatures& CRegChain::GetSigs() const
{
	return m_sigs;
}
