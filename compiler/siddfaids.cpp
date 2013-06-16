#include "stdafx.h"
#include <hwprj\siddfaids.h>

SIDDFAHDR CSidDfaIds::CSidDfaIds()
{
	try
	{
		m_ruleResult = new std::vector<COMPILEDINFO>;
	}
	catch (std::exception &e)
	{
		TTHROW(e.what());
	}
}

SIDDFAHDR CSidDfaIds::CSidDfaIds(const CSidDfaIds& other)
{
	try
	{
		m_ruleResult = new std::vector<COMPILEDINFO>;
	}
	catch (std::exception &e)
	{
		TTHROW(e.what());
	}
	*this = other;
}

SIDDFAHDR CSidDfaIds &CSidDfaIds::operator=(const CSidDfaIds &other)
{
	try
	{
		*m_ruleResult = *other.m_ruleResult;
	}
	catch (std::exception &e)
	{
		TTHROW(e.what());
	}
	return *this;
}
SIDDFAHDR CSidDfaIds::~CSidDfaIds()
{
	delete m_ruleResult;
}

SIDDFAHDR COMPILEDINFO& CSidDfaIds::operator[](ulong index)
{
	return (*m_ruleResult)[index];
}

SIDDFAHDR const COMPILEDINFO& CSidDfaIds::operator[](ulong index) const
{
	return (*m_ruleResult)[index];
}

SIDDFAHDR void CSidDfaIds::Reserve(ulong nCount)
{
	try
	{
		m_ruleResult->reserve(nCount);
	}
	catch (std::exception &e)
	{
		TTHROW(e.what());
	}
}

SIDDFAHDR void CSidDfaIds::Resize(ulong nSize)
{
	try
	{
		m_ruleResult->resize(nSize);
	}
	catch (std::exception &e)
	{
		TTHROW(e.what());
	}
}

SIDDFAHDR const ulong CSidDfaIds::Size() const
{
	return m_ruleResult->size();
}

SIDDFAHDR void CSidDfaIds::PushBack(const COMPILEDINFO &ruleResult)
{
	try
	{
		m_ruleResult->push_back(ruleResult);
	}
	catch (std::exception &e)
	{
		TTHROW(e.what());
	}
}

SIDDFAHDR COMPILEDINFO& CSidDfaIds::Back()
{
	return m_ruleResult->back();
}
