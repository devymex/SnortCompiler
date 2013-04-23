#include "stdafx.h"
#include "orDfa.h"

void IncreDfaNum(CDfa &dfa, STATEID num)
{
	for(STATEID i = 0; i < dfa.Size(); ++i)
	{
		for(STATEID charNum = 0; charNum < dfa.GetColNum(); ++charNum)
		{
			if(dfa[i][charNum] != STATEID(-1))
			{
				dfa[i][charNum] += num;
			}
		}
	}
}
//termSta is the terminal state of nfa which is caculated in advance
void InsertDfa(CDfa &dfa, CNfa &nfa, STATEID termSta)
{
	STATEID temp = (STATEID)nfa.Size();
	nfa.Resize(nfa.Size() + dfa.Size());
	for(STATEID i = 0; i < dfa.Size(); ++i)
	{
		if((dfa[i].GetFlag() & CDfaRow::TERMINAL) != 0)
		{
			nfa[temp + i][EMPTYEDGE].PushBack(termSta);
		}
		for(STATEID charNum = 0; charNum < CHARSETSIZE; ++charNum)
		{
			STATEID group = dfa.GetGroup(charNum);
			if(dfa[i][group] != STATEID(-1))
			{
				nfa[i + temp][charNum].PushBack(dfa[i][charNum]);
			}
		}
	}
}
