//#include "stdafx.h"
//#include "OrDfanew.h"
//#include "MergeDfanew.h"
//#include "../nfa2dfa/nfa2dfa.h"
//#include "../common/common.h"
//
//MERDFA bool OrMerge(std::vector<CDfanew> &dfas, CDfanew &lastDfa)
//{
//#undef max
//	for(STATEID i = 0; i < dfas.size(); ++i)
//	{
//		if(dfas[i].Size() > std::numeric_limits<STATEID>::max())
//		{
//			return false;
//		}
//	}
//	std::vector<CNfa> nfas;
//	STATEID nTermSta = 0;
//	CNfa oneNfa;
//	oneNfa.Resize(1);
//
//	for(STATEID i = 0; i < dfas.size(); ++i)
//	{
//		nTermSta += dfas[i].Size();
//	}
//	nTermSta += 1;
//
//	for(STATEID i = 0; i < dfas.size(); ++i)
//	{
//		oneNfa[0][EMPTYEDGE].PushBack(oneNfa.Size());
//		IncreDfaNum(dfas[i], (STATEID)oneNfa.Size());
//		InsertDfa(dfas[i], oneNfa,nTermSta);
//	}
//	CDfa dfalast;
//	if(NfaToDfa(oneNfa, dfalast, true) == 0)
//	{
//		return true;
//	}
//	else
//	{
//		return false;
//	}
//}