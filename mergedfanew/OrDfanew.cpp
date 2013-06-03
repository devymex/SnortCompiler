#include "stdafx.h"
#include "OrDfanew.h"

//根据other的sta查找termset，将找到的TERMSET插入到nfalog的staNum中，同时增加count
void AddDIdToNFALog(STATEID sta, const CDfanew &other, NFALOG *nfalog, size_t &count, size_t staNum)
{
	for(size_t i = 0; i < other.GetTermCnt(); ++i)
	{
		if(other.GetTerm(i).dfaSta == sta)
		{
			nfalog[count].dfaId = other.GetTerm(i).dfaId;
			nfalog[count].nfaStateId = staNum;
			++count;
		}
	}
}

//termSta is the terminal state of nfa which is caculated in advance
void NInsertDfa(CDfanew &dfa, CNfa &nfa, size_t nTermSta, NFALOG *nfalog, size_t &count)
{
	size_t temp = nfa.Size();
	nfa.Resize(nfa.Size() + dfa.Size());
	for(size_t i = 0; i < dfa.Size(); ++i)
	{
		CNfaRow &nfaRow = nfa[temp + i];
		CDfaRow &dfaRow = dfa[i];
		if((dfaRow.GetFlag() & CDfaRow::TERMINAL) != 0)
		{
			nfaRow.AddDest(EMPTYEDGE, nTermSta);
			size_t staNum = temp + i;
			AddDIdToNFALog(i, dfa, nfalog, count, staNum);
		}
		for(size_t charNum = 0; charNum < DFACOLSIZE; ++charNum)
		{
			size_t group = dfa.Char2Group(charNum);
			STATEID add = dfaRow[group];		
			if (add != STATEID(-1))
			{
				nfaRow.AddDest(charNum, size_t(add + temp));
			}
		}
	}
}
