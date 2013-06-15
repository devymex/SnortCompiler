#include "stdafx.h"
#include <hwprj\snortrule.h>


SNORTRULEHDR CSnortRule::CSnortRule()
	: m_nSid(0), m_nFlag(0)
{
	try
	{
		m_pOptions = new std::vector<CRuleOption*>;
	}
	catch (std::exception &e)
	{
		throw CTrace(__FILE__, __LINE__, e.what());
	}
}

SNORTRULEHDR CSnortRule::CSnortRule(const CSnortRule &other)
{
	try
	{
		m_pOptions = new std::vector<CRuleOption*>;
	}
	catch (std::exception &e)
	{
		throw CTrace(__FILE__, __LINE__, e.what());
	}
	*this = other;
}

SNORTRULEHDR const CSnortRule& CSnortRule::operator = (const CSnortRule &other)
{
	m_nSid = other.m_nSid;
	m_nFlag = other.m_nFlag;
	try
	{
		*m_pOptions = *other.m_pOptions;
	}
	catch (std::exception &e)
	{
		throw CTrace(__FILE__, __LINE__, e.what());
	}
	return *this;
}

SNORTRULEHDR CSnortRule::~CSnortRule()
{
	std::vector<CRuleOption*> &opts = *m_pOptions;
	for (std::vector<CRuleOption*>::iterator i = opts.begin(); i != opts.end(); ++i)
	{
		delete *i;
	}
	opts.clear();
	delete m_pOptions;
}

SNORTRULEHDR ulong CSnortRule::Size() const
{
	return m_pOptions->size();
}

SNORTRULEHDR void CSnortRule::SetSid(ulong sid)
{
	m_nSid = sid;
}

SNORTRULEHDR ulong CSnortRule::GetSid() const
{
	return m_nSid;
}

SNORTRULEHDR void CSnortRule::SetFlag(PARSE_INFO flag)
{
	m_nFlag = flag;
}

SNORTRULEHDR CSnortRule::PARSE_INFO CSnortRule::GetFlag() const
{
	return m_nFlag;
}

SNORTRULEHDR void CSnortRule::PushBack(CRuleOption* ruleoption)
{
	try
	{
		m_pOptions->push_back(ruleoption);
	}
	catch (std::exception &e)
	{
		throw CTrace(__FILE__, __LINE__, e.what());
	}
}

SNORTRULEHDR void CSnortRule::PopBack()
{
	m_pOptions->pop_back();
}

SNORTRULEHDR CRuleOption* CSnortRule::operator[](ulong nIdx) const
{
	return (*m_pOptions)[nIdx];
}
