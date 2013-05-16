#include "stdafx.h"
#include "OrDfanew.h"
#include "MergeDfanew.h"


MERDFANEW bool NOrMerge(std::vector<CDfanew> &dfas, CDfanew &lastDfa)
{
#undef max

	size_t nTermSta = 0;//合并后总状态数
	for(STATEID i = 0; i < dfas.size(); ++i)
	{
		if(dfas[i].Size() > std::numeric_limits<STATEID>::max())
		{
			return false;
		}
		nTermSta += dfas[i].Size();
	}
	++nTermSta;

	NFALOG nfalog[DFACOLSIZE];//nfa终态与dfaID的对应
	size_t count = 0;

	std::vector<CNfa> nfas;
	
	//STATEID nTermSta = 0;//合并后总状态数
	CNfa oneNfa;//合并后的nfa
	oneNfa.Reserve(1000);
	oneNfa.Resize(1);

	//for(STATEID i = 0; i < dfas.size(); ++i)
	//{
	//	nTermSta += dfas[i].Size();
	//}
	//nTermSta += (dfas.size() - 1);

	for(STATEID i = 0; i < dfas.size(); ++i)
	{
		oneNfa[0][EMPTYEDGE].PushBack(oneNfa.Size());
		NIncreDfaNum(dfas[i], (STATEID)oneNfa.Size());
		NInsertDfa(dfas[i], oneNfa,nTermSta, nfalog, count);
	}

	if(lastDfa.FromNFA(oneNfa, nfalog, count, true) == 0)
	{
		return true;
	}
	else
	{
		return false;
	}
}