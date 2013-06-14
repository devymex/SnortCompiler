#include "stdafx.h"
#include <hwprj\siddfaids.h>

SIDDFAHDR CSidDfaIds::CSidDfaIds()
{
	m_ruleResult = new std::vector<COMPILEDINFO>;
}

SIDDFAHDR CSidDfaIds::CSidDfaIds(const CSidDfaIds& other)
{
	m_ruleResult = new std::vector<COMPILEDINFO>;
	*this = other;
}

SIDDFAHDR const CSidDfaIds &CSidDfaIds::operator=(const CSidDfaIds &other)
{
	*m_ruleResult = *other.m_ruleResult;
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
	m_ruleResult->reserve(nCount);
}

SIDDFAHDR void CSidDfaIds::Resize(ulong nSize)
{
	m_ruleResult->resize(nSize);
}

SIDDFAHDR const ulong CSidDfaIds::Size() const
{
	return m_ruleResult->size();
}

SIDDFAHDR void CSidDfaIds::PushBack(const COMPILEDINFO &ruleResult)
{
	m_ruleResult->push_back(ruleResult);
}

SIDDFAHDR COMPILEDINFO& CSidDfaIds::Back()
{
	return m_ruleResult->back();
}
