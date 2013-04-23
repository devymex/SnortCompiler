#include "stdafx.h"
#include "andDfa.h"
#include "orDfa.h"
#include "mergedfas.h"
#include "../nfa2dfa/nfa2dfa.h"
#include "../common/common.h"

//MERDFA void AndMerge(CDfa &dfa1, CDfa &dfa2, CDfa &andDfa, CVectorNumber &termFlag)
//{
//	std::unordered_map<_int64, STATEID, _HASH> idMap;
//	std::vector<std::pair<std::pair<size_t, size_t>, size_t>> idVec;
//	idMap.rehash(_HASH::MAX_SIZE);
//	dfa1.Resize(dfa1.Size() + 1);
//	dfa2.Resize(dfa2.Size() + 1);
//
//	//initial idMap
//	for(size_t fir = 0; fir < dfa1.Size(); ++fir)
//	{
//		for(size_t sec = 0; sec < dfa2.Size(); ++sec)
//		{
//			if(fir != dfa1.Size() - 1 && sec != dfa2.Size() - 1)
//			{
//				idMap[_int64(fir) << 32 | sec] = (STATEID)idMap.size();
//			}
//			else if(fir == dfa1.Size() - 1 && sec != dfa2.Size() - 1)
//			{
//				idMap[_int64(size_t(-1)) << 32 | sec] = (STATEID)idMap.size();
//			}
//			else if(fir != dfa1.Size() - 1 && sec == dfa2.Size() - 1)
//			{
//				idMap[_int64(fir) << 32 | size_t(-1)] = (STATEID)idMap.size();
//			}
//			else if(fir == dfa1.Size() - 1 && sec == dfa2.Size() - 1)
//			{
//				idMap[_int64(size_t(-1)) << 32 | size_t(-1)] = (STATEID)idMap.size();
//			}
//
//			idVec.push_back(std::make_pair(std::make_pair(fir, sec), idMap.size() - 1));
//		}
//	}
//
//	termFlag.Resize(dfa1.Size() * dfa2.Size());
//	andDfa.Resize(dfa1.Size() * dfa2.Size());
//
//	for(size_t d = 0; d < andDfa.Size(); ++d)
//	{
//		size_t fir = idVec[d].first.first;
//		size_t sec = idVec[d].first.second;
//		for(size_t charNum = 0; charNum < CHARSETSIZE; ++charNum)
//		{
//			if(dfa1[fir][charNum] != size_t(-1) || dfa2[sec][charNum] != size_t(-1))
//			{
//				_int64 temp = _int64(dfa1[fir][charNum]) << 32 | dfa2[sec][charNum];
//				andDfa[d][charNum] = idMap[temp];
//			}
//			
//			size_t firFlag = dfa1[fir].GetFlag();
//			size_t secFlag = dfa2[sec].GetFlag();
//			if((firFlag & CDfaRow::TERMINAL) != 0 && (secFlag & CDfaRow::TERMINAL) != 0)
//			{
//				termFlag[d] = 3;
//			}
//			else
//				if((firFlag & CDfaRow::TERMINAL) != 0 && (secFlag & CDfaRow::TERMINAL) == 0)
//				{
//					termFlag[d] = 1;
//				}
//				else
//					if((firFlag & CDfaRow::TERMINAL) == 0 && (secFlag & CDfaRow::TERMINAL) != 0)
//					{
//						termFlag[d] = 2;
//					}
//					else
//					{
//						termFlag[d] = 0;
//					}
//		}
//	}
//}

MERDFA bool OrMerge(std::vector<CDfa> &dfas, CDfa &lastDfa)
{
#undef max
	for(STATEID i = 0; i < dfas.size(); ++i)
	{
		if(dfas[i].Size() > std::numeric_limits<STATEID>::max())
		{
			return false;
		}
	}
	std::vector<CNfa> nfas;
	STATEID termSta = 0;
	CNfa oneNfa;
	oneNfa.Resize(1);

	for(STATEID i = 0; i < dfas.size(); ++i)
	{
		termSta += dfas[i].Size();
	}
	termSta += 1;

	for(STATEID i = 0; i < dfas.size(); ++i)
	{
		oneNfa[0][EMPTYEDGE].PushBack(oneNfa.Size());
		IncreDfaNum(dfas[i], (STATEID)oneNfa.Size());
		InsertDfa(dfas[i], oneNfa, termSta);
	}
	if(NfaToDfa(oneNfa, lastDfa) == 0)
	{
		return true;
	}
	else
	{
		return false;
	}
}