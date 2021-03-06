#include "stdafx.h"
#include <hwprj\snortrule.h>

typedef std::vector<CRuleOption*>::iterator OPTVEC_ITER;

SNORTRULEHDR CSnortRule::CSnortRule()
	: m_nSid(ulong(-1)), m_nFlags(NORMAL)
{
	try
	{
		m_pOptions = new std::vector<CRuleOption*>;
	}
	catch (std::exception &e)
	{
		TTHROW(e.what());
	}
}

SNORTRULEHDR CSnortRule::CSnortRule(const CSnortRule &other)
	: m_nSid(other.m_nSid), m_nFlags(other.m_nFlags)
{
	TASSERT(other.m_pOptions != null);
	try
	{
		m_pOptions = new std::vector<CRuleOption*>;
		m_pOptions->reserve((other.m_pOptions->size()));
		for (OPTVEC_ITER i = other.m_pOptions->begin();
			i != other.m_pOptions->end(); ++i)
		{
			m_pOptions->push_back((*i)->Clone());
		}
	}
	catch (std::exception &e)
	{
		TTHROW(e.what());
	}
}

SNORTRULEHDR CSnortRule::~CSnortRule()
{
	Clear();
	delete m_pOptions;
}

SNORTRULEHDR CSnortRule& CSnortRule::operator = (const CSnortRule &other)
{
	TASSERT(other.m_pOptions != null);
	if (this != &other)
	{
		m_nSid = other.m_nSid;
		m_nFlags = other.m_nFlags;
		try
		{
			Clear();
			for (OPTVEC_ITER i = other.m_pOptions->begin();
				i != other.m_pOptions->end(); ++i)
			{
				m_pOptions->push_back((*i)->Clone());
			}
		}
		catch (std::exception &e)
		{
			TTHROW(e.what());
		}
	}
	return *this;
}

SNORTRULEHDR CRuleOption* CSnortRule::operator[](ulong nIdx) const
{
	return (*m_pOptions)[nIdx];
}

SNORTRULEHDR ulong CSnortRule::Size() const
{
	return m_pOptions->size();
}

SNORTRULEHDR void CSnortRule::Clear()
{
	for (OPTVEC_ITER i = m_pOptions->begin(); i != m_pOptions->end(); ++i)
	{
		delete *i;
	}
	m_pOptions->clear();
}

SNORTRULEHDR CRuleOption* CSnortRule::Back()
{
	return m_pOptions->back();
}

SNORTRULEHDR void CSnortRule::SetSid(ulong sid)
{
	m_nSid = sid;
}

SNORTRULEHDR ulong CSnortRule::GetSid() const
{
	return m_nSid;
}

SNORTRULEHDR void CSnortRule::AddFlags(PARSE_INFO nFlags)
{
	m_nFlags |= nFlags;
}

SNORTRULEHDR void CSnortRule::SetFlags(PARSE_INFO flag)
{
	m_nFlags = flag;
}

SNORTRULEHDR CSnortRule::PARSE_INFO CSnortRule::GetFlags() const
{
	return m_nFlags;
}

SNORTRULEHDR bool CSnortRule::HasFlags(PARSE_INFO nFlags) const
{
	return (m_nFlags & nFlags) != 0;
}

SNORTRULEHDR void CSnortRule::PushBack(CRuleOption* pRuleOpt)
{
	try
	{
		m_pOptions->push_back(pRuleOpt->Clone());
	}
	catch (std::exception &e)
	{
		TTHROW(e.what());
	}
}

SNORTRULEHDR void CSnortRule::PopBack()
{
	delete m_pOptions->back();
	m_pOptions->pop_back();
}
