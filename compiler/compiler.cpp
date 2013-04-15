#include "stdafx.h"
#include "compiler.h"
#include "../common/common.h"
#include "../rule2nfa/rule2nfa.h"
#include "../nfa2dfa/nfa2dfa.h"
#include "../pcre2nfa/pcre2nfa.h"

void CALLBACK Process(const CSnortRule &rule, LPVOID lpVoid)
{
	RES &result = *(RES*)lpVoid;
	size_t nSid = rule.GetSid();
	size_t nFlag = rule.GetFlag();
	if (nFlag & CSnortRule::RULE_HASBYTE)
	{
		result.m_hasbyteIds.PushBack(nSid);
	}
	else if (nFlag & CSnortRule::RULE_HASNOT)
	{
		result.m_hasnotIds.PushBack(nSid);
	}
	else if (rule.Size() == 0)
	{
		result.m_emptyIds.PushBack(nSid);
	}
	else
	{
		CNfaTree nfatree;
		size_t flag = InterpretRule(rule, nfatree);
		if (flag == SC_ERROR)
		{
			result.m_errorIds.PushBack(nSid);
			return;
		}
		else if (flag == SC_EXCEED)
		{
			result.m_exceedIds.PushBack(nSid);
			return;
		}
		else
		{
			SNORTIDDFAIDS sidDfaIds;
			sidDfaIds.m_nSid = nSid;
			const size_t nCursize = result.m_dfaTbl.size();
			const size_t nIncrement = nfatree.Size();
			result.m_dfaTbl.resize(nCursize + nIncrement);
			size_t nId;
			for (size_t i = 0; i < nIncrement; ++i)
			{
				CNfa nfa;
				SerializeNfa(nfatree[i], nfa);
				nId = nCursize + i;
				CDfa &dfa = result.m_dfaTbl[nId];
				NfaToDfa(nfa, dfa);
				sidDfaIds.m_dfaIds.PushBack(nId);
			}
		}
	}
}

COMPILER void compile(LPCTSTR filename, RES &result)
{
	ParseRule(filename, Process, &result);
}