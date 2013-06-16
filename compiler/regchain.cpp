#include "stdafx.h"
#include <hwprj\regchain.h>

REGRULEHDR CRegChain::CRegChain()
{
	try
	{
		m_pRegList = new STRINGVEC;
	}
	catch (std::exception &e)
	{
		TTHROW(e.what());
	}
}

REGRULEHDR CRegChain::CRegChain(const CRegChain &other)
{
	try
	{
		m_pRegList = new STRINGVEC;
	}
	catch (std::exception &e)
	{
		TTHROW(e.what());
	}
	*this = other;
}

REGRULEHDR CRegChain::~CRegChain()
{
	delete m_pRegList;
}

REGRULEHDR CRegChain& CRegChain::operator = (const CRegChain &other)
{
	try
	{
		*this->m_pRegList = *other.m_pRegList;
	}
	catch (std::exception &e)
	{
		TTHROW(e.what());
	}
	this->m_sigs = other.m_sigs;
	return *this;
}

REGRULEHDR CDllString& CRegChain::operator[](ulong nIdx)
{
	return (*m_pRegList)[nIdx];
}

REGRULEHDR const CDllString& CRegChain::operator[](ulong nIdx) const
{
	return (*m_pRegList)[nIdx];
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
	try
	{
		m_pRegList->push_back(pcreStr);
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
		m_pRegList->resize(nSize);
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
