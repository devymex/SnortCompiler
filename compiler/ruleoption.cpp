#include "stdafx.h"
#include <hwprj\ruleoption.h>

SNORTRULEHDR CRuleOption::CRuleOption()
	: m_nFlags(NOFLAG)
{
	try
	{
		m_pPat = new std::string;
	}
	catch (std::exception &e)
	{
		TTHROW(e.what());
	}
}

SNORTRULEHDR CRuleOption::CRuleOption(const CRuleOption &other)
	: m_nFlags(other.m_nFlags)
{
	TASSERT(other.m_pPat != null);
	try
	{
		m_pPat = new std::string(*other.m_pPat);
	}
	catch (std::exception &e)
	{
		TTHROW(e.what());
	}
}

SNORTRULEHDR CRuleOption::~CRuleOption()
{
	delete m_pPat;
}

SNORTRULEHDR CRuleOption& CRuleOption::operator = (const CRuleOption &other)
{
	TASSERT(other.m_pPat != null);
	try
	{
		*m_pPat = *other.m_pPat;
	}
	catch (std::exception &e)
	{
		TTHROW(e.what());
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

SNORTRULEHDR void CRuleOption::GetPattern(CDllString &out) const
{
	out.Assign(m_pPat->c_str());
}

SNORTRULEHDR void CRuleOption::FromPattern(pcstr pPat)
{
	try
	{
		m_pPat->assign(pPat);
	}
	catch (std::exception &e)
	{
		TTHROW(e.what());
	}
}

SNORTRULEHDR CRuleOption* CRuleOption::Clone() const
{
	CRuleOption *pNew = null;
	try
	{
		pNew = new CRuleOption(*this);
	}
	catch (std::exception &e)
	{
		TTHROW(e.what());
	}
	return pNew;
}
