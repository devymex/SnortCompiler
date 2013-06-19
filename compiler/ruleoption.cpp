#include "stdafx.h"
#include <hwprj\ruleoption.h>
#include "comprule.h"

SNORTRULEHDR CRuleOption::CRuleOption()
	: m_nFlags(NOFLAG)
{
}

SNORTRULEHDR CRuleOption::CRuleOption(const CRuleOption &other)
	: m_nFlags(other.m_nFlags)
{
}

SNORTRULEHDR CRuleOption::~CRuleOption()
{
}

SNORTRULEHDR CRuleOption& CRuleOption::operator = (const CRuleOption &other)
{
	if (this != &other)
	{
		m_nFlags = other.m_nFlags;
	}
	return *this;
}

SNORTRULEHDR CRuleOption::OPTIONFLAG CRuleOption::GetFlags() const
{
	return m_nFlags;
}

SNORTRULEHDR void CRuleOption::SetFlags(OPTIONFLAG nFlags)
{
	m_nFlags = nFlags;
}

SNORTRULEHDR void CRuleOption::AddFlags(OPTIONFLAG nFlags)
{
	m_nFlags |= nFlags;
}

SNORTRULEHDR bool CRuleOption::HasFlags(OPTIONFLAG nFlags) const
{
	return ((m_nFlags & nFlags) != 0);
}

SNORTRULEHDR void CRuleOption::DelFlags(OPTIONFLAG nFlags)
{
	m_nFlags &= (~nFlags);
}

SNORTRULEHDR void CRuleOption::FormatPattern(CDllString &out)
{
	m_nFlags = CRuleOption::NOFLAG;
	STRING str = out.Data();
	STRING_ITER iBeg = str.begin(), iEnd = str.end();
	if (*std::find_if_not(iBeg, iEnd, ISSPACE()) == '!')
	{
		m_nFlags |= CRuleOption::HASNOT;
	}
	QuotedContext(iBeg, iEnd);
	out.Assign(STRING(iBeg, iEnd).c_str());
}
