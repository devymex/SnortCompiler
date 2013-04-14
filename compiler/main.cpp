#include "stdafx.h"
#include "../common/common.h"
#include "../ruleldr/parserule.h"
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
};

void CALLBACK Process(const CSnortRule &rule, LPVOID lpVoid)
{
	RES &result = *(RES*)lpVoid;
	CNfaTree nfatree;
	size_t flag = InterpretRule(rule, nfatree);
	if (flag == SC_ERROR)
	{
		result.m_errorIds.push_back();
	}
	else if (flag == SC_EXCEED)
	{
		
	}
	else
	{
		for (size_t i = 0; i < nfatree.Size(); ++i)
		{
			CNfa nfa;
			SerializeNfa(nfatree[i], nfa);
			CDfa dfa;
			NfaToDfa(nfa, dfa);
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