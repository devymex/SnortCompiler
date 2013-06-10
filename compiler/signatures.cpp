#include "stdafx.h"
#include <hwprj\signatures.h>

SIGHDR CSignatures::CSignatures()
{
	m_pSigs = new std::vector<SIGNATURE>;
}

SIGHDR CSignatures::CSignatures(const CSignatures& other)
{
	m_pSigs = new std::vector<SIGNATURE>;
	*this = other;
}

SIGHDR const CSignatures &CSignatures::operator=(const CSignatures &other)
{
	*m_pSigs = *other.m_pSigs;
	return *this;
}

SIGHDR CSignatures::~CSignatures()
{
	delete m_pSigs;
}

SIGHDR const ULONG CSignatures::Size() const
{
	return m_pSigs->size();
}

SIGHDR void CSignatures::Resize(ULONG nSize)
{
	m_pSigs->resize(nSize);
}

SIGHDR void CSignatures::PushBack(SIGNATURE Sig)
{
	m_pSigs->push_back(Sig);
}

SIGHDR SIGNATURE &CSignatures::operator[](ULONG nIdx)
{
	return (*m_pSigs)[nIdx];
}

SIGHDR const SIGNATURE &CSignatures::operator[](ULONG nIdx) const
{
	return (*m_pSigs)[nIdx];
}

SIGHDR void CSignatures::Clear()
{
	m_pSigs->clear();
}

SIGHDR void CSignatures::Unique()
{
	std::sort(m_pSigs->begin(), m_pSigs->end());
	m_pSigs->erase(std::unique(m_pSigs->begin(),
		m_pSigs->end()), m_pSigs->end());
}
