#include "stdafx.h"
#include <hwprj\regrule.h>

REGRULEHDR CRegRule::CRegRule()
{
	try
	{
		m_pRegVec = new CHAINVEC;
	}
	catch (std::exception &e)
	{
		TTHROW(e.what());
	}
}

REGRULEHDR CRegRule::CRegRule(const CRegRule &other)
{
	TASSERT(other.m_pRegVec != null);
	try
	{
		m_pRegVec = new CHAINVEC(*other.m_pRegVec);
	}
	catch (std::exception &e)
	{
		TTHROW(e.what());
	}
}

REGRULEHDR CRegRule::~CRegRule()
{
	delete m_pRegVec;
}

REGRULEHDR CRegChain& CRegRule::operator[](ulong nIdx)
{
	return (*m_pRegVec)[nIdx];
}

REGRULEHDR const CRegChain& CRegRule::operator[](ulong nIdx) const
{
	return (*m_pRegVec)[nIdx];
}

REGRULEHDR CRegRule& CRegRule::operator = (const CRegRule &other)
{
	TASSERT(other.m_pRegVec != null);
	try
	{
		*this->m_pRegVec = *other.m_pRegVec;
	}
	catch (std::exception &e)
	{
		TTHROW(e.what());
	}
	return *this;
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
	try
	{
		m_pRegVec->reserve(nCount);
	}
	catch (std::exception &e)
	{
		TTHROW(e.what());
	}
}

REGRULEHDR void CRegRule::Resize(ulong nSize)
{
	try
	{
		m_pRegVec->resize(nSize);
	}
	catch (std::exception &e)
	{
		TTHROW(e.what());
	}
}

REGRULEHDR void CRegRule::PushBack(const CRegChain &nRegChain)
{
	try
	{
		m_pRegVec->push_back(nRegChain);
	}
	catch (std::exception &e)
	{
		TTHROW(e.what());
	}
}

REGRULEHDR void CRegRule::PopBack()
{
	m_pRegVec->pop_back();
}
