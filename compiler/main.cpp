#include "stdafx.h"
#include "../common/common.h"
#include "../ruleldr/parserule.h"
#include "../rule2nfa/rule2nfa.h"
#include "../nfa2dfa/nfa2dfa.h"

void CALLBACK Process(const CSnortRule &rule)
{
	CNfaTree nfatree;
	//
	for (size_t i = 0; i < nfatree.Size(); ++i)
	{
		CNfa nfa;
		SerializeNfa(nfatree[i], nfa);
		CDfa dfa;
		NfaToDfa(nfa, dfa);

	}
}

void _tmain(int nArgs, TCHAR **pArgs)
{
	if (nArgs != 2)
	{
		return;
	}

	CString filename(pArgs[1]);

	ParseRule(filename, Process);

	system("pause");
}