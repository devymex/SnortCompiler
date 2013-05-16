#include "stdafx.h"
#include "OrDfanew.h"
#include "MergeDfanew.h"


MERDFANEW bool NOrMerge(std::vector<CDfanew> &dfas, CDfanew &lastDfa)
{
#undef max

	size_t nTermSta = 0;//�ϲ�����״̬��
	for(STATEID i = 0; i < dfas.size(); ++i)
	{
		if(dfas[i].Size() > std::numeric_limits<STATEID>::max())
		{
			return false;
		}
		nTermSta += dfas[i].Size();
	}
	++nTermSta;

	NFALOG nfalog[DFACOLSIZE];//nfa��̬��dfaID�Ķ�Ӧ
	size_t count = 0;

	std::vector<CNfa> nfas;
	
	//STATEID nTermSta = 0;//�ϲ�����״̬��
	CNfa oneNfa;//�ϲ����nfa
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