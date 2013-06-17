#include "stdafx.h"
#include <hwprj\regchain.h>

REGRULEHDR CRegChain::CRegChain()
{
	try
	{
		m_pPcreVec = new PCREVEC;
	}
	catch (std::exception &e)
	{
		TTHROW(e.what());
	}
}

REGRULEHDR CRegChain::CRegChain(const CRegChain &other)
	: m_sigs(other.m_sigs)
{
	TASSERT(other.m_pPcreVec != null);
	try
	{
		m_pPcreVec = new PCREVEC(*other.m_pPcreVec);
	}
	catch (std::exception &e)
	{
		TTHROW(e.what());
	}
}

REGRULEHDR CRegChain::~CRegChain()
{
	delete m_pPcreVec;
}

REGRULEHDR CRegChain& CRegChain::operator = (const CRegChain &other)
{
	TASSERT(other.m_pPcreVec != null);
	try
	{
		*this->m_pPcreVec = *other.m_pPcreVec;
	}
	catch (std::exception &e)
	{
		TTHROW(e.what());
	}
	this->m_sigs = other.m_sigs;

	return *this;
}

REGRULEHDR CPcreOption& CRegChain::operator[](ulong nIdx)
{
	return (*m_pPcreVec)[nIdx];
}

REGRULEHDR const CPcreOption& CRegChain::operator[](ulong nIdx) const
{
	return (*m_pPcreVec)[nIdx];
}

REGRULEHDR ulong CRegChain::Size() const
{
	return m_pPcreVec->size();
}

REGRULEHDR void CRegChain::Clear()
{
	m_pPcreVec->clear();
	m_sigs.Clear();
}

REGRULEHDR CPcreOption& CRegChain::Back() const
{
	return m_pPcreVec->back();
}

REGRULEHDR void CRegChain::PushBack(const CPcreOption &pcreStr)
{
	try
	{
		m_pPcreVec->push_back(pcreStr);
	}
	catch (std::exception &e)
	{
		TTHROW(e.what());
	}
}

REGRULEHDR void CRegChain::Resize(ulong nSize)
{
	try
	{
		m_pPcreVec->resize(nSize);
	}
	catch (std::exception &e)
	{
		TTHROW(e.what());
	}
}

REGRULEHDR CSignatures& CRegChain::GetSigs()
{
	return m_sigs;
}

REGRULEHDR const CSignatures& CRegChain::GetSigs() const
{
	return m_sigs;
}
