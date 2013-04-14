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
	size_t staNum = seriaNfa.size();
	for(size_t n = 0; n < nfaChain.Size(); ++n)
	{

		IncreNfaStaNum(seriaNfa.size(), nfaChain[n]);
		for (size_t i = 0; i < nfaChain[n].size(); ++i)
		{
			seriaNfa.push_back(nfaChain[n][i]);
		}

		if(n != nfaChain.Size() - 1)
		{
			seriaNfa.back()[cEmptyEdge].PushBack(seriaNfa.size());
			seriaNfa.push_back(oneSta);
			seriaNfa.back()[cEmptyEdge].PushBack(seriaNfa.size());
		}		

		staNum = seriaNfa.size();
	}
}
