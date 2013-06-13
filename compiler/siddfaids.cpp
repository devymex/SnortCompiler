#include "stdafx.h"
#include <hwprj\siddfaids.h>

SIDDFAHDR CSidDfaIds::CSidDfaIds()
{
	m_ruleResult = new std::vector<COMPILEDRULENEW>;
}

SIDDFAHDR CSidDfaIds::CSidDfaIds(const CSidDfaIds& other)
{
	m_ruleResult = new std::vector<COMPILEDRULENEW>;
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

SIDDFAHDR COMPILEDRULENEW& CSidDfaIds::operator[](ulong index)
{
	return (*m_ruleResult)[index];
}

SIDDFAHDR const COMPILEDRULENEW& CSidDfaIds::operator[](ulong index) const
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

SIDDFAHDR void CSidDfaIds::PushBack(const COMPILEDRULENEW &ruleResult)
{
	m_ruleResult->push_back(ruleResult);
}

SIDDFAHDR COMPILEDRULENEW& CSidDfaIds::Back()
{
	return m_ruleResult->back();
}
