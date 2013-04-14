#include "stdafx.h"
#include "rule2nfa.h"
#include "CombineTree.h"

CRECHANFA size_t InterpretRule(const CSnortRule &rule, CNfaTree &outTree)
{
	return size_t(-1);
}

CRECHANFA void SerializeNfa(CNfaChain &nfaChain, CNfa &seriaNfa)
{
	const size_t cEmptyEdge = 256;
	CNfaRow oneSta;
	size_t staNum = seriaNfa.Size();
	for(size_t n = 0; n < nfaChain.Size(); ++n)
	{

		IncreNfaStaNum(seriaNfa.Size(), nfaChain[n]);
		for (size_t i = 0; i < nfaChain[n].Size(); ++i)
		{
			seriaNfa.PushBack(nfaChain[n][i]);
		}

		if(n != nfaChain.Size() - 1)
		{
			seriaNfa.Back()[cEmptyEdge].PushBack(seriaNfa.Size());
			seriaNfa.PushBack(oneSta);
			seriaNfa.Back()[cEmptyEdge].PushBack(seriaNfa.Size());
		}		

		staNum = seriaNfa.Size();
	}
}
