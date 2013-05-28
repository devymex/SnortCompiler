#include "stdafx.h"
#include "OrDfanew.h"

//void NIncreDfaNum(CDfanew &dfa, size_t num)
//{
//	for(size_t i = 0; i < dfa.Size(); ++i)
//	{
//		for(size_t charNum = 0; charNum < dfa.GetGroupCount(); ++charNum)
//		{
//			STATEID sta = dfa[i][charNum];
//			if(sta != STATEID(-1))
//			{
//				dfa[i][charNum] += num;
//			}
//			else
//			{
//				dfa[i][charNum] = (size_t)-1;
//			}
//		}
//	}
//}
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

			nfalog[count].dfaId = dfa.GetId();
			nfalog[count].nfaStateId = temp + i;
			++count;
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
