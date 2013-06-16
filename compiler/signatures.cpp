#include "stdafx.h"
#include <hwprj\signatures.h>

SIGHDR CSignatures::CSignatures()
{
	try
	{
		m_pSigs = new SIGNATUREVEC;
	}
	catch (std::exception &e)
	{
		TTHROW(e.what());
	}
}

SIGHDR CSignatures::CSignatures(const CSignatures& other)
{
	try
	{
		m_pSigs = new SIGNATUREVEC;
	}
	catch (std::exception &e)
	{
		TTHROW(e.what());
	}
	*this = other;
}

SIGHDR CSignatures &CSignatures::operator=(const CSignatures &other)
{
	try
	{
		*m_pSigs = *other.m_pSigs;
	}
	catch (std::exception &e)
	{
		TTHROW(e.what());
	}
	return *this;
}

SIGHDR bool CSignatures::operator == (const CSignatures &other) const
{
	try
	{
		return *m_pSigs == *other.m_pSigs;
	}
	catch (std::exception &e)
	{
		TTHROW(e.what());
	}
}

SIGHDR bool CSignatures::operator <	(const CSignatures &other) const
{
	return *m_pSigs < *other.m_pSigs;
}

SIGHDR CSignatures::~CSignatures()
{
	delete m_pSigs;
}

SIGHDR const ulong CSignatures::Size() const
{
	return m_pSigs->size();
}

SIGHDR void CSignatures::Resize(ulong nSize)
{
	try
	{
		m_pSigs->resize(nSize);
	}
	catch (std::exception &e)
	{
		TTHROW(e.what());
	}
}

SIGHDR void CSignatures::PushBack(SIGNATURE Sig)
{
	try
	{
		m_pSigs->push_back(Sig);
	}
	catch (std::exception &e)
	{
		TTHROW(e.what());
	}
}

SIGHDR SIGNATURE &CSignatures::operator[](ulong nIdx)
{
	return (*m_pSigs)[nIdx];
}

SIGHDR const SIGNATURE &CSignatures::operator[](ulong nIdx) const
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

SIGHDR ulong CSignatures::Find(SIGNATURE Sig) const
{
	SIGVEC_ITER iter = std::find(m_pSigs->begin(), m_pSigs->end(), Sig);
	if (iter == m_pSigs->end())
	{
		return ulong(-1);
	}
	else
	{
		return iter - m_pSigs->begin();
	}
}
