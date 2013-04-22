#include "stdafx.h"
#include "orDfa.h"

void IncreDfaNum(CDfa &dfa, STATEID num)
{
	for(size_t i = 0; i < dfa.Size(); ++i)
	{
		for(size_t charNum = 0; charNum < CHARSETSIZE; ++charNum)
		{
			if(dfa[i][charNum] != size_t(-1))
			{
				dfa[i][charNum] += num;
			}
		}
	}
}
//termSta is the terminal state of nfa which is caculated in advance
void InsertDfa(CDfa &dfa, CNfa &nfa, size_t termSta)
{
	size_t temp = nfa.Size();
	nfa.Resize(nfa.Size() + dfa.Size());
	for(size_t i = 0; i < dfa.Size(); ++i)
	{
		if((dfa[i].GetFlag() & CDfaRow::TERMINAL) != 0)
		{
			nfa[temp + i][EMPTYEDGE].PushBack(termSta);
		}
		for(size_t charNum = 0; charNum < CHARSETSIZE; ++charNum)
		{
			if(dfa[i][charNum] != size_t(-1))
			{
				nfa[i + temp][charNum].PushBack(dfa[i][charNum]);
			}
		}
	}
}
