#include "stdafx.h"
#include "CombineTree.h"
#include "CreDfa.h"

//void combineNfaTree(std::vector<std::vector<CNfa>> &nfaTree, CNfa &lastNfa)
CREDFA void combineNfaTree(CNfaTree &nfaTree, std::vector<CDfa> &dfaVec)
{
	std::vector<CNfa> nfaVec;

	for (size_t i = 0; i < nfaTree.Size(); ++i)
	{
		std::vector<CNfa*> NfaChain;
		for (size_t j = 0; j < nfaTree.ChainSize(i); ++j)
		{
			NfaChain.push_back(&(nfaTree.GetNfa(i, j)));
		}
		CNfa seriaNfa;
		CDfa seriaDfa;
		SerializeNfa(NfaChain, seriaNfa);
		NfaToDfa(seriaNfa, seriaDfa);
		dfaVec.push_back(seriaDfa);
	}
}


void SerializeNfa(std::vector<CNfa*> &braNfaList, CNfa &seriaNfa)
{
	CNfaRow oneSta;
	size_t staNum = seriaNfa.size();
	for(std::vector<CNfa*>::iterator iter = braNfaList.begin(); iter != braNfaList.end(); ++iter)
	{

		IncreNfaStaNum(seriaNfa.size(), **iter);
		for (size_t i = 0; i < (*iter)->size(); ++i)
		{
			seriaNfa.push_back((**iter)[i]);
		}

		if(iter != braNfaList.end() - 1)
		{
			seriaNfa.back()[EMPTY].PushBack(seriaNfa.size());
			seriaNfa.push_back(oneSta);
			seriaNfa.back()[EMPTY].PushBack(seriaNfa.size());
		}		

		staNum = seriaNfa.size();
	}
}

void IncreNfaStaNum(int increNum, CNfa &curNfa)
{
	if(increNum == 0)
	{
		return;
	}
//	for(std::vector<CNfaRow>::iterator nfaIter = curNfa.begin(); nfaIter != curNfa.end(); ++nfaIter)
	for (size_t i = 0; i < curNfa.size(); ++i)
	{
		for(size_t charNum = 0; charNum < CHARSETSIZE -3; ++charNum)
		{
			for (size_t i = 0; i < curNfa[i][charNum].Size(); ++i)
			{
				curNfa[i][charNum][i] += increNum;
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
//void InitialNfaTree(NFATREE &nfaTree)
//{
//	CNfaRow oneSta;
//	CNfa newNfa1, newNfa2, newNfa3;
//	std::vector<CNfa> nfaList1, nfaList2, nfaList3;
//
//	newNfa1.push_back(oneSta);
//	//std::cout << newNfa1.size() << std::endl;
//	for(size_t charNum = 0; charNum < CHARSETSIZE - 4; ++charNum)
//	{
//		newNfa1[0].CharSet[charNum].push_back(0);
//	}
//	newNfa1[0].CharSet[91].push_back(1);
//	newNfa1[0].CharSet[92].push_back(1);
//	newNfa1[0].CharSet[EMPTY].push_back(1);
//
//	newNfa1.push_back(oneSta);
//	newNfa1[1].CharSet[92].push_back(1);
//	newNfa1[1].CharSet[91].push_back(2);
//
//	newNfa2.push_back(oneSta);
//	std::cout << newNfa1.size() << std::endl;
//	for(size_t charNum = 0; charNum < CHARSETSIZE - 4; ++charNum)
//	{
//		newNfa2[0].CharSet[charNum].push_back(0);
//	}
//	newNfa2[0].CharSet[91].push_back(1);
//	newNfa2[0].CharSet[92].push_back(1);
//
//	newNfa2.push_back(oneSta);
//	newNfa2[1].CharSet[91].push_back(1);
//	newNfa2[1].CharSet[92].push_back(2);
//	newNfa2[1].CharSet[EMPTY].push_back(2);
//
//
//	nfaList1.push_back(newNfa1);
//	nfaList1.push_back(newNfa2);
//
//	//printNfa(newNfa1);
//	std::cout << std::endl;
//	printNfa(newNfa2);
//	std::cout << std::endl;
//	//printNfa(newNfa3);
//	std::cout << std::endl;
//

	//newNfa1.push_back(oneSta);
	//std::cout << newNfa1.size() << std::endl;
	//for(size_t charNum = 0; charNum < CHARSETSIZE - 4; ++charNum)
	//{
	//	newNfa1[0].CharSet[charNum].push_back(0);
	//}
	//newNfa1[0].CharSet[91].push_back(1);

	//newNfa1.push_back(oneSta);
	//newNfa1[1].CharSet[92].push_back(2);

	//newNfa2.push_back(oneSta);
	//std::cout << newNfa1.size() << std::endl;
	//for(size_t charNum = 0; charNum < CHARSETSIZE - 4; ++charNum)
	//{
	//	newNfa2[0].CharSet[charNum].push_back(0);
	//}
	//newNfa2[0].CharSet[93].push_back(1);

	//newNfa2.push_back(oneSta);
	//newNfa2[1].CharSet[94].push_back(2);

	//newNfa3.push_back(oneSta);
	//newNfa3[0].CharSet[96].push_back(1);

	//newNfa3.push_back(oneSta);
	//newNfa3[1].CharSet[97].push_back(2);


	//nfaList1.push_back(newNfa1);
	//nfaList2.push_back(newNfa2);
	//nfaList3.push_back(newNfa3);



//	nfaTree.push_back(nfaList1);
//    nfaTree.push_back(nfaList2);
// //   nfaTree.push_back(nfaList3);
//
//}
