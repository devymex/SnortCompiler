#include "stdafx.h"
#include "compiler.h"
#include "../common/common.h"
#include "../rule2nfa/rule2nfa.h"
#include "../nfa2dfa/nfa2dfa.h"
#include "../pcre2nfa/pcre2nfa.h"

COMPILER CDfaTbl::CDfaTbl()
{
	m_dfaTbl = new std::vector<CDfa>;	
}

COMPILER CDfaTbl::CDfaTbl(const CDfaTbl& other)
{
	m_dfaTbl = new std::vector<CDfa>;
	*this = other;
}

COMPILER const CDfaTbl &CDfaTbl::operator=(const CDfaTbl &other)
{
	*m_dfaTbl = *other.m_dfaTbl;
	return *this;
}

COMPILER CDfaTbl::~CDfaTbl()
{
	delete m_dfaTbl;
}

COMPILER CDfa& CDfaTbl::operator[](size_t index)
{
	return (*m_dfaTbl)[index];
}

COMPILER const CDfa& CDfaTbl::operator[](size_t index) const
{
	return (*m_dfaTbl)[index];
}

COMPILER void CDfaTbl::Reserve(size_t nCount)
{
	m_dfaTbl->reserve(nCount);
}

COMPILER void CDfaTbl::Resize(size_t nSize)
{
	m_dfaTbl->resize(nSize);
}

COMPILER const size_t CDfaTbl::Size() const
{
	return m_dfaTbl->size();
}

COMPILER void CDfaTbl::PushBack(const CDfa &dfa)
{
	m_dfaTbl->push_back(dfa);
}

COMPILER CSidDfaIds::CSidDfaIds()
{
	m_ruleResult = new std::vector<COMPILEDRULE>;
}

COMPILER CSidDfaIds::CSidDfaIds(const CSidDfaIds& other)
{
	m_ruleResult = new std::vector<COMPILEDRULE>;
	*this = other;
}

COMPILER const CSidDfaIds &CSidDfaIds::operator=(const CSidDfaIds &other)
{
	*m_ruleResult = *other.m_ruleResult;
	return *this;
}
COMPILER CSidDfaIds::~CSidDfaIds()
{
	delete m_ruleResult;
}

COMPILER COMPILEDRULE& CSidDfaIds::operator[](size_t index)
{
	return (*m_ruleResult)[index];
}

COMPILER const COMPILEDRULE& CSidDfaIds::operator[](size_t index) const
{
	return (*m_ruleResult)[index];
}

COMPILER void CSidDfaIds::Reserve(size_t nCount)
{
	m_ruleResult->reserve(nCount);
}

COMPILER void CSidDfaIds::Resize(size_t nSize)
{
	m_ruleResult->resize(nSize);
}

COMPILER const size_t CSidDfaIds::Size() const
{
	return m_ruleResult->size();
}

COMPILER void CSidDfaIds::PushBack(const COMPILEDRULE &ruleResult)
{
	m_ruleResult->push_back(ruleResult);
}

COMPILER COMPILEDRULE& CSidDfaIds::Back()
{
	return m_ruleResult->back();
}

COMPILER CDfaTbl& CRes::GetDfaTable()
{
	return m_dfaTbl;
}

COMPILER CSidDfaIds& CRes::GetSidDfaIds()
{
	return m_sidDfaIds;
}

void CALLBACK Process(const CSnortRule &rule, LPVOID lpVoid)
{
	CRes &result = *(CRes*)lpVoid;
	size_t nSid = rule.GetSid();
	size_t nFlag = rule.GetFlag();
	size_t nNewSize = result.GetSidDfaIds().Size() + 1;
	result.GetSidDfaIds().Resize(nNewSize);
	COMPILEDRULE &ruleResult = result.GetSidDfaIds().Back();
	if (nFlag & CSnortRule::RULE_HASBYTE)
	{
		ruleResult.m_nResult = COMPILEDRULE::RES_HASBYTE;
		return;
	}
	else if (nFlag & CSnortRule::RULE_HASNOT)
	{
		ruleResult.m_nResult = COMPILEDRULE::RES_HASNOT;
		return;
	}
	else if (rule.Size() == 0)
	{
		ruleResult.m_nResult = COMPILEDRULE::RES_EMPTY;
		return;
	}
	else
	{
		CNfaTree nfatree;
		size_t flag = InterpretRule(rule, nfatree);
		if (flag == SC_ERROR)
		{
			ruleResult.m_nResult = COMPILEDRULE::RES_ERROR;
			return;
		}
		else if (flag == SC_EXCEED)
		{
			ruleResult.m_nResult = COMPILEDRULE::RES_EXCEED;
			return;
		}
		else
		{
			ruleResult.m_nResult = COMPILEDRULE::RES_SUCCESS;
			ruleResult.m_nSid = nSid;
			const size_t nCursize = result.GetDfaTable().Size();
			const size_t nIncrement = nfatree.Size();
			result.GetDfaTable().Resize(nCursize + nIncrement);
			size_t nId;
			for (size_t i = 0; i < nIncrement; ++i)
			{
				CNfa nfa;
				SerializeNfa(nfatree[i], nfa);
				nId = nCursize + i;
				CDfa &dfa = result.GetDfaTable()[nId];
				NfaToDfa(nfa, dfa);
				ruleResult.m_dfaIds.PushBack(nId);
			}
		}
	}
}

COMPILER void compile(LPCTSTR filename, CRes &result)
{
	ParseRule(filename, Process, &result);
}