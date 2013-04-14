#include "stdafx.h"
#include "../common/common.h"
#include "../rule2nfa/rule2nfa.h"
#include "../nfa2dfa/nfa2dfa.h"
#include "../pcre2nfa/pcre2nfa.h"

struct SNORTIDDFAIDS
{
	size_t m_nSid;
	std::vector<size_t> m_dfaIds;
};

struct RES
{
	std::vector<CDfa> m_dfaTbl;
	std::vector<SNORTIDDFAIDS> m_sidDfaIds;
	std::vector<size_t> m_errorIds;
	std::vector<size_t> m_exceedIds;
	std::vector<size_t> m_emptyIds;
};

void CALLBACK Process(const CSnortRule &rule, LPVOID lpVoid)
{
	RES &result = *(RES*)lpVoid;
	CNfaTree nfatree;
	size_t flag = InterpretRule(rule, nfatree);
	size_t nSid = rule.GetSid();
	if (flag == SC_ERROR)
	{
		result.m_errorIds.push_back(nSid);
		return;
	}
	else if (flag == SC_EXCEED)
	{
		result.m_exceedIds.push_back(nSid);
		return;
	}
	else if (flag == SC_EMPTY)
	{
		result.m_emptyIds.push_back(nSid);
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
			sidDfaIds.m_dfaIds.push_back(nId);
		}
	}
}

void _tmain(int nArgs, TCHAR **pArgs)
{
	if (nArgs != 2)
	{
		return;
	}

	CString filename(pArgs[1]);
	
	RES result;
	ParseRule(filename, Process, &result);

	system("pause");
}