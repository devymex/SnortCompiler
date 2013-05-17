#include "stdafx.h"
#include "../common/common.h"
#include "CombineTree.h"



void IncreNfaStaNum(int increNum, CNfa &curNfa)
{
	if(increNum == 0)
	{
		return;
	}

	for (size_t i = 0; i < curNfa.Size(); ++i)
	{
		CNfaRow &row = curNfa[i];
		for(size_t charNum = 0; charNum < CHARSETSIZE - 3; ++charNum)
		{
			size_t nCnt = row.DestCnt(charNum);
			for (size_t j = 0; j < nCnt; ++j)
			{
				row.GetDest(charNum, j) += increNum;
			}
		}
	}
}

//
//CNfa IntersectNfa(CNfa &firNfa, CNfa &secNfa)
//{
//	CNfa newNfa;
//	std::map<_int64, size_t> newIdMap;
//	//初始化该map,
//
//	for(size_t fir = 0; fir <= firNfa.size(); ++fir)
//	{
//		for(size_t sec = 0; sec <= secNfa.size(); ++sec)
//		{
//			_int64 temp = (_int64)fir << 32 | sec;
//			newIdMap[temp] = newIdMap.size();
//		}
//	}
//
//	for(size_t fir = 0; fir <= firNfa.size(); ++fir)
//	{
//		for(size_t sec = 0; sec <= secNfa.size(); ++sec)
//		{
//			CNfaRow newSta;
//			if((fir == firNfa.size()) || (sec == secNfa.size()))
//			{
//				std::vector<size_t> fTempVec;
//				CNfaRow tempSta;
//				fTempVec.push_back(firNfa.size());
//				for(size_t charNum = 0; charNum < CHARSETSIZE - 4; ++charNum)
//				{
//					tempSta.CharSet[charNum] = fTempVec;
//				}
//
//				if((fir == firNfa.size()) && (sec != secNfa.size()))
//				{
//					RowIntersection(fir, sec, tempSta, secNfa[sec],  firNfa.size(), secNfa.size(), newIdMap, newSta);
//					newNfa.push_back(CNfaRow());
//					newNfa.back() = newSta;
//				}
//				else if((fir != firNfa.size()) && (sec == secNfa.size()))
//				{
//					RowIntersection(fir, sec, firNfa[fir], tempSta, firNfa.size(), secNfa.size(), newIdMap, newSta);
//					newNfa.push_back(CNfaRow());
//					newNfa.back() = newSta;
//				}
//				else if((fir == firNfa.size()) && (sec == secNfa.size()))
//				{
//					break;
//				}
//			}
//			else
//			{
//				RowIntersection(fir, sec, firNfa[fir], secNfa[sec], firNfa.size(), secNfa.size(), newIdMap, newSta);
//					newNfa.push_back(CNfaRow());
//					newNfa.back() = newSta;
//			}
//
//		}
//	}
//
//	return newNfa;
//}

//处理某个CNfa已经到达终态，另一个没有到达终态的情况。fORs == false 时，表示第一个处于终态， fORs == true 时，表示第二个处于终态。
//void HalfRowIntersection(bool fORs, CNfaRow &newSta, CNfaRow fORSSta, size_t fORsSize, std::map<_int64, size_t> newIdMap)
//{
//	for(size_t charNum = 0; charNum < CHARSETSIZE - 3; ++charNum)
//	{
//		if(!fORSSta.CharSet[charNum].empty())
//		{
//			for(std::vector<size_t>::iterator iter = fORSSta.CharSet[charNum].begin(); iter != fORSSta.CharSet[charNum].end(); ++iter)
//			{
//				if(fORs == false)
//				{
//					newSta.CharSet[charNum].push_back(newIdMap[(_int64)fORsSize << 32 | *iter]);
//				}
//				else
//				{
//					newSta.CharSet[charNum].push_back(newIdMap[(_int64)*iter << 32 | fORsSize]);
//				}
//			}
//		}
//	}
//
//}
//两个CNfaRow的笛卡尔乘积
//void  RowIntersection(size_t fStaNum, size_t sStaNum, CNfaRow &firSta, CNfaRow &secSta, size_t firSize, size_t secSize, std::map<_int64, size_t> &newIdMap, CNfaRow &newSta)
//{
//	for(size_t charNum = 0; charNum < EMPTY; ++charNum)
//	{
//	    std::vector<size_t> nextStas;
//		InterSection(firSta.CharSet[charNum], secSta.CharSet[charNum], firSize, secSize, newIdMap, nextStas);
//		newSta.CharSet[charNum] = nextStas;
//	}
//
//	if((!firSta.CharSet[EMPTY].empty()) && secSta.CharSet[EMPTY].empty())
//	{
//		for(size_t num = 0; num < firSta.CharSet[EMPTY].size(); ++num)
//		{
//			_int64 temp = firSta.CharSet[EMPTY][num] << 32 | sStaNum;
//			newSta.CharSet[EMPTY].push_back(newIdMap[(_int64)firSta.CharSet[EMPTY][num] << 32 | sStaNum]);
//		}
//	}
//	else if(firSta.CharSet[EMPTY].empty() && (!secSta.CharSet[EMPTY].empty()))
//	{
//		for(size_t num = 0; num < secSta.CharSet[EMPTY].size(); ++num)
//		{
//			newSta.CharSet[EMPTY].push_back(newIdMap[(_int64)fStaNum << 32 | secSta.CharSet[EMPTY][num]]);
//		}
//	}
//	else if((!firSta.CharSet[EMPTY].empty()) && (!secSta.CharSet[EMPTY].empty()))
//	{
//		std::vector<size_t> nextStas;
//		firSta.CharSet[EMPTY].push_back(fStaNum);
//		secSta.CharSet[EMPTY].push_back(sStaNum);
//		InterSection(firSta.CharSet[EMPTY], secSta.CharSet[EMPTY], firSize, secSize, newIdMap, nextStas);
//		newSta.CharSet[EMPTY] = nextStas;
//	}
//}
//将两个 size_t的集合进行笛卡尔乘积
//void InterSection(std::vector<size_t> &firVec, std::vector<size_t> &secVec, size_t firNfaSize, size_t secNfaSize,std::map<_int64,size_t> &newIdMap, std::vector<size_t> &nextStas)
//{
//	if((!firVec.empty()) && (!secVec.empty()))
//	{
//		for(std::vector<size_t>::iterator firIter = firVec.begin(); firIter != firVec.end(); ++firIter)
//			for(std::vector<size_t>::iterator secIter = secVec.begin(); secIter != secVec.end(); ++secIter)
//			{
//				nextStas.push_back(newIdMap[((_int64)*firIter << 32) | (*secIter)]);
//			}
//	}
//}
////
