#include "stdafx.h"
#include <hwprj\regrule.h>

SNORTRULEHDR CRegRule::CRegRule()
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

SNORTRULEHDR CRegRule::CRegRule(const CRegRule &other)
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

SNORTRULEHDR CRegRule::~CRegRule()
{
	delete m_pRegVec;
}

SNORTRULEHDR CPcreChain& CRegRule::operator[](ulong nIdx)
{
	return (*m_pRegVec)[nIdx];
}

SNORTRULEHDR const CPcreChain& CRegRule::operator[](ulong nIdx) const
{
	return (*m_pRegVec)[nIdx];
}

SNORTRULEHDR CRegRule& CRegRule::operator = (const CRegRule &other)
{
	TASSERT(other.m_pRegVec != null);
	if (this != &other)
	{
		try
		{
			*this->m_pRegVec = *other.m_pRegVec;
		}
		catch (std::exception &e)
		{
			TTHROW(e.what());
		}
	}
	return *this;
}

SNORTRULEHDR ulong CRegRule::Size() const
{
	return m_pRegVec->size();
}

SNORTRULEHDR void CRegRule::Erase(ulong ulIdx)
{
	m_pRegVec->erase(m_pRegVec->begin() + ulIdx);
}

SNORTRULEHDR CPcreChain& CRegRule::Back()
{
	return m_pRegVec->back();
}

SNORTRULEHDR const CPcreChain& CRegRule::Back() const
{
	return m_pRegVec->back();
}

SNORTRULEHDR void CRegRule::Reserve(ulong nCount)
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

SNORTRULEHDR void CRegRule::Resize(ulong nSize)
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

SNORTRULEHDR void CRegRule::PushBack(const CPcreChain &nRegChain)
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

SNORTRULEHDR void CRegRule::PopBack()
{
	m_pRegVec->pop_back();
}
