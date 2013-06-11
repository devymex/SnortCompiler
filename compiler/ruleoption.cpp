#include "stdafx.h"
#include <hwprj\ruleoption.h>

SNORTRULEHDR CRuleOption::CRuleOption()
	: m_nFlag(0)
{
	m_pPattern = new std::string;
}

SNORTRULEHDR CRuleOption::CRuleOption(const CRuleOption &other)
{
	m_pPattern = new std::string;
	*this = other;
}

SNORTRULEHDR CRuleOption::~CRuleOption()
{
	delete m_pPattern;
}

SNORTRULEHDR const CRuleOption& CRuleOption::operator=(const CRuleOption &other)
{
	*m_pPattern = *other.m_pPattern;
	m_nFlag = other.m_nFlag;
	return *this;
}

SNORTRULEHDR ulong CRuleOption::GetPattern(LPSTR lpStr, ulong nLen) const
{
	if (lpStr == NULL || nLen == 0)
	{
		return m_pPattern->length();
	}
	if (nLen > m_pPattern->length())
	{
		nLen = m_pPattern->length();
	}
	CopyMemory(lpStr, &(*m_pPattern)[0], nLen);
	return nLen;
}

SNORTRULEHDR void CRuleOption::SetPattern(LPCSTR lpStr)
{
	*m_pPattern = lpStr;
}

SNORTRULEHDR ulong CRuleOption::GetFlag() const
{
	return m_nFlag;
}

SNORTRULEHDR void CRuleOption::SetFlag(ulong nFlag)
{
	m_nFlag = nFlag;
}

SNORTRULEHDR void CRuleOption::AddFlag(ulong nFlag)
{
	m_nFlag |= nFlag;
}

SNORTRULEHDR bool CRuleOption::TestFlag(ulong nFlag) const
{
	return ((m_nFlag & nFlag) != 0);
}
