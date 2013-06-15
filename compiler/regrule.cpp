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
		throw CTrace(__FILE__, __LINE__, e.what());
	}
}

REGRULEHDR CRegRule::~CRegRule()
{
	delete m_pRegVec;
}

REGRULEHDR CRegRule::CRegRule(const CRegRule &other)
{
	try
	{
		m_pRegVec = new CHAINVEC;
	}
	catch (std::exception &e)
	{
		throw CTrace(__FILE__, __LINE__, e.what());
	}
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
	try
	{
		m_pRegVec->reserve(nCount);
	}
	catch (std::exception &e)
	{
		throw CTrace(__FILE__, __LINE__, e.what());
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
		throw CTrace(__FILE__, __LINE__, e.what());
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
		throw CTrace(__FILE__, __LINE__, e.what());
	}
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
	try
	{
		*this->m_pRegVec = *other.m_pRegVec;
	}
	catch (std::exception &e)
	{
		throw CTrace(__FILE__, __LINE__, e.what());
	}
	return *this;
}
