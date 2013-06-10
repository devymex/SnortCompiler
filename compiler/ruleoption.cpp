#include "stdafx.h"
#include "ruleoption.h"

CRuleOption::CRuleOption()
	: m_nFlag(0)
{
	m_pPattern = new std::string;
}

CRuleOption::CRuleOption(const CRuleOption &other)
{
	m_pPattern = new std::string;
	*this = other;
}

CRuleOption::~CRuleOption()
{
	delete m_pPattern;
}

const CRuleOption& CRuleOption::operator=(const CRuleOption &other)
{
	*m_pPattern = *other.m_pPattern;
	m_nFlag = other.m_nFlag;
	return *this;
}

ULONG CRuleOption::GetPattern(LPSTR lpStr, ULONG nLen) const
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

void CRuleOption::SetPattern(LPCSTR lpStr)
{
	*m_pPattern = lpStr;
}

ULONG CRuleOption::GetFlag() const
{
	return m_nFlag;
}

void CRuleOption::SetFlag(ULONG nFlag)
{
	m_nFlag = nFlag;
}

void CRuleOption::AddFlag(ULONG nFlag)
{
	m_nFlag |= nFlag;
}

BOOL CRuleOption::TestFlag(ULONG nFlag) const
{
	return ((m_nFlag & nFlag) != 0);
}
