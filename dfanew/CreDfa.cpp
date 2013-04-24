#include "stdafx.h"
#include "CreDfa.h"
#include "dfanew.h"

/* Following codes is a testing for unordered_map */
/* DONT REMOVE */
//std::vector<size_t> v1, v2, v3;
//v1.push_back(0);
//v2.push_back(1);
////v2.push_back(2);

//int n = ssh.size();
//ssh.insert(std::make_pair(v1, 20));
//n = ssh.size();
//ssh.insert(std::make_pair(v2, 30));
//n = ssh.size();

//STATESETHASH::iterator xx1 = ssh.find(v1);
//n = xx1->second;
//STATESETHASH::iterator xx2 = ssh.find(v2);
//n = xx2->second;

bool ColumnEqual(std::vector<CStateSet*> &c1, std::vector<CStateSet*>&c2)
{
	if (c1.size() != c2.size())
	{
		return false;
	}
	for (size_t i = 0; i < c1.size(); ++i)
	{
		if (c1[i]->Size() != c2[i]->Size())
		{
			return false;
		}
		if (!(*c1[i] == *c2[i]))
		{
			return false;
		}
	}
	return true;
}

void AvaiEdges(CNfa &oneNfaTab, STATEID *group)
{
	std::vector<std::vector<size_t>> charGroups;
	std::vector<CStateSet*> column[DFACOLSIZE];
	for(size_t charNum = 0; charNum < DFACOLSIZE; ++charNum)
	{
		column[charNum].reserve(20000);
		for(size_t i = 0; i < oneNfaTab.Size(); ++i)
		{
			CStateSet &elem = oneNfaTab[i][charNum];
			elem.Sort();
			column[charNum].push_back(&elem);
			for (size_t j = 0; j < elem.Size(); ++j)
			{
				if(elem[j] > oneNfaTab.Size())
				{
					std::cout << "overflow" << std::endl;
					return;
				}
			}
		}
	}

	charGroups.clear();
	std::vector<size_t> fullSet;

	//std::cout << "columns complete" << std::endl;

	for (size_t i = 0; i < DFACOLSIZE; ++i)
	{
		fullSet.push_back(i);
	}

	for (; !fullSet.empty();)
	{
		charGroups.push_back(std::vector<size_t>());
		std::vector<size_t> &curGroup = charGroups.back();
		curGroup.push_back(fullSet.front());
		fullSet.erase(fullSet.begin());
		for (std::vector<size_t>::iterator i = fullSet.begin(); i != fullSet.end() && !fullSet.empty();)
		{
			if (ColumnEqual(column[curGroup.front()], column[*i]))
			{
				curGroup.push_back(*i);
				i = fullSet.erase(i);
			}
			else
			{
				++i;
			}
		}
	}
	//std::cout << "grouping complete" << std::endl;

	for(STATEID i = 0; i < charGroups.size(); ++i)
	{
		for(STATEID j = 0; j < charGroups[i].size(); ++j)
		{
			group[charGroups[i][j]] = i;
		}
	}
}

void NextNfaSet(const CNfa &oneNfaTab, const std::vector<size_t> &curNfaVec, size_t edge, std::vector<size_t> &nextENfaVec, char &finFlag)
{
	if (edge >= CHARSETSIZE)
	{
		std::cout << "Fatal Error!" << std::endl;
		return;
	}
	std::vector<size_t> nextNfaVec;

	for(std::vector<size_t>::const_iterator vecIter = curNfaVec.begin(); vecIter != curNfaVec.end(); ++vecIter)
	{
		if(*vecIter == oneNfaTab.Size())
		{
			continue;
		}
		const CStateSet &nextEdges = oneNfaTab[*vecIter][edge];
		for (size_t i = 0; i < nextEdges.Size(); ++i)
		{
			nextNfaVec.push_back(nextEdges[i]);
		}
	}
	std::sort(nextNfaVec.begin(), nextNfaVec.end());
	nextNfaVec.erase(std::unique(nextNfaVec.begin(), nextNfaVec.end()), nextNfaVec.end());
	if(!nextNfaVec.empty())
	{
		EClosure(oneNfaTab, nextNfaVec, nextENfaVec, finFlag);
	}
}

void EClosure(const CNfa &oneNfaTab, const std::vector<size_t> &curNfaVec, std::vector<size_t> &eNfaVec, char &finFlag)
{
	std::vector<size_t> eStack;
	char caledStat[20000] = {0};
	const size_t cEmptyEdge = 256;

	if (eNfaVec.capacity() < 1000)
	{
		eNfaVec.reserve(1000);
	}

	for(std::vector<size_t>::const_iterator vecIter = curNfaVec.begin(); vecIter != curNfaVec.end(); ++vecIter)
	{
		eStack.push_back(*vecIter);
		eNfaVec.push_back(*vecIter);
		if(*vecIter == oneNfaTab.Size())
		{
			continue;
		}

		while(!eStack.empty())
		{
			size_t curSta;
			curSta = eStack.back();
			eStack.pop_back();
			if(caledStat[curSta] == 0)
			{
				caledStat[curSta] = 1;
				if(curSta < oneNfaTab.Size())
				{
					const CStateSet &eTempVec = oneNfaTab[curSta][cEmptyEdge];
					if(eTempVec.Size() != 0)
					{
						for (size_t i = 0; i < eTempVec.Size(); ++i)
						{
							eNfaVec.push_back(eTempVec[i]);
							eStack.push_back(eTempVec[i]);
						}
					}
				}
			}
		}
	}

	std::sort(eNfaVec.begin(), eNfaVec.end());
	eNfaVec.erase(std::unique(eNfaVec.begin(), eNfaVec.end()), eNfaVec.end());

	for(std::vector<size_t>::iterator iter = eNfaVec.begin(); iter != eNfaVec.end(); ++iter)
	{
		if(*iter == oneNfaTab.Size())
		{
			finFlag = 1;
			break;
		}
	}
}

void RemoveUnreachable(const std::vector<STATEID> *Tab, const STALIST &begs, const size_t &col, std::vector<BYTE> &reachable)
{
	//mark state after traversal, 0 is unreachable and 1 is reachable
	size_t stas = reachable.size();
	std::vector<BYTE> staFlags(stas, 0);

	//reserve current visited states
	std::vector<size_t> staStack;
	staStack.reserve(stas);

	staStack.assign(begs.begin(), begs.end());
	//extract each state in staStack until all states have visited
	for (; !staStack.empty(); )
	{
		size_t nSta = staStack.back();
		staStack.pop_back();
		if (staFlags[nSta] == 0)
		{
			staFlags[nSta] = 1;
			//according to each of transitions add states to staStack 
			for (size_t i = 0; i < col; ++i)
			{
				for (size_t j = 0; j < Tab[nSta * col + i].size(); ++j)
				{
					staStack.push_back(Tab[nSta * col + i][j]);
				}
			}
		}
	}

	for (std::vector<BYTE>::iterator i = staFlags.begin(); i != staFlags.end(); ++i)
	{
		reachable[i - staFlags.begin()] += *i;
	}
}

void MergeReachable(CDfanew &oneDfaTab, std::vector<BYTE> &reachable)
{
	//统计可达状态的数目
	size_t nRcbCnt = std::count(reachable.begin(), reachable.end(), 2);

	size_t nColNum = oneDfaTab.GetGroupCount();

	//定义一个同CDfanew中成员变量m_pDfa类型相同的变量，用于存储删除多余状态后的DFA跳转表
	std::vector<CDfaRow> tmpDfa;
	tmpDfa.resize(nRcbCnt, nColNum);

	STATEID nNewIdx = 0;
	//将原DFA跳转表中的可达状态复制到tmpDfa中，并修改可达状态的编号
	for (std::vector<BYTE>::iterator iter = reachable.begin(); iter != reachable.end(); ++iter)
	{
		if (2 == *iter)
		{
			STATEID nStaId = STATEID(iter - reachable.begin());
			tmpDfa[nNewIdx] = oneDfaTab[nStaId];
			*iter = nNewIdx;
			++nNewIdx;
		}
		else
		{
			*iter = STATEID(-1);
		}
	}

	//将新跳转表中的状态编号修改为新编号
	for (STATEID i = 0; i < nRcbCnt; ++i)
	{
		CDfaRow &curRow = tmpDfa[i];
		for (STATEID j = 0; j < nColNum; ++j)
		{
			if (curRow[j] != STATEID(-1))
			{
				curRow[j] = reachable[curRow[j]];
			}
		}
	}

	//复制新的跳转表给原DFA
	oneDfaTab.m_pDfaClear();
	nNewIdx = 0;
	for (STATEID idx = 0; idx < nRcbCnt; ++idx)
	{
		oneDfaTab[nNewIdx] = tmpDfa[idx];
		++nNewIdx;
	}
}

void PartitionNonDisState(const size_t &groupnum, std::vector<STATEID> *pRevTbl, SETLIST &pSets)
{
	//store the iterator of patition will be visited into wSets
	std::list<SETLIST_ITER> wSets;
	SETLIST_ITER iLast = pSets.end();
	--iLast;

	//initialize wSets with all final states
	for (SETLIST_ITER iCurSet = pSets.begin(); iCurSet != iLast; ++iCurSet)
	{
		wSets.push_back(iCurSet);
	}

	//each element in ableToW present a property of according state of tmpDfa,
	//and has two labels, 0 and 1. 1 indicates the according state has the specific
	//transition to one state of curWSet, 0 otherwise
	STATEID size = sizeof(pRevTbl) / (STATEID)groupnum;
	std::vector<BYTE> ableToW(size, 0);
	bool bAllZero = true;
	for (; !wSets.empty(); )
	{
		STALIST curWSet = *wSets.front();
		wSets.pop_front();
		for (BYTE byChar = 0; byChar < groupnum; ++byChar)
		{
			//initialize the ableToW
			//for each state in curWSet find source states of it in pRecTbl
			//if exist at last one source state to curWSet, the bAllZero is set to false
			for (STALIST_ITER iSta = curWSet.begin(); iSta != curWSet.end(); ++iSta)
			{
				std::vector<STATEID> &ableToI = pRevTbl[*iSta * groupnum + byChar];
				for (std::vector<STATEID>::iterator i = ableToI.begin(); i != ableToI.end(); ++i)
				{
					ableToW[*i] = 1;
					bAllZero = false;
				}
			}
			if (!bAllZero)
			{
				for (SETLIST_ITER iPSet = pSets.begin(); iPSet != pSets.end(); ++iPSet)
				{
					//each partition in pSets,according to the label of a state in partition adjust position 
					//all unvisited states lie in the front of list, visited states locate
					//at the rear end of list 
					for (STALIST_ITER iCurSta = iPSet->begin(); iCurSta != iPSet->end(); )
					{
						if (ableToW[*iCurSta] == 0)
						{
							STATEID tmp = *iCurSta;
							iCurSta = iPSet->erase(iCurSta);
							iPSet->insert(iPSet->begin(), tmp);
						}
						else
						{
							++iCurSta;
						}
					} 

					//mark the position of two new partition
					size_t nUnableCnt = 0;
					STALIST_ITER iCutBeg = iPSet->begin(), iCutEnd = iPSet->end();
					for (; iCutBeg != iPSet->end(); ++iCutBeg)
					{
						if (ableToW[*iCutBeg] == 1)
						{
							break;
						}
						++nUnableCnt;
					}

					//record start position of the less partition 
					if (nUnableCnt <= iPSet->size() / 2)
					{
						iCutEnd = iCutBeg;
						iCutBeg = iPSet->begin();
					}

					//the less partition insert into pSets, its iterator insert into wSets
					if (iCutBeg != iCutEnd)
					{
						SETLIST_ITER iOldSet = iPSet;
						iPSet = pSets.insert(++iPSet, STALIST());
						iPSet->splice(iPSet->begin(), *iOldSet, iCutBeg, iCutEnd);
						wSets.push_back(iPSet);
					}
				}

				//initialize ableToW and bAllZero before read the next char, 
				ZeroMemory(&ableToW[0], ableToW.size());
				bAllZero = true;
			}
		}
	}
}

void MergeNonDisStates(CDfanew &oneDfaTab, SETLIST &Partition)
{
	std::vector<STATEID> sta2Part(oneDfaTab.Size());
	
	STATEID nCol = (STATEID)oneDfaTab.GetGroupCount();

	//定义一个同CDfanew中成员变量m_pDfa类型相同的变量，用于存储删除多余状态后的DFA跳转表
	std::vector<CDfaRow> tmpDfa;
	tmpDfa.resize((STATEID)Partition.size(), nCol);

	//等价的状态存于同一个partition中，标记原来的状态存在哪一个新的partition中，并修改新的起始状态编号
	STATEID nSetIdx = 0;
	for (SETLIST_ITER iSet = Partition.begin(); iSet != Partition.end(); ++iSet)
	{
		for (STALIST_ITER iSta = iSet->begin(); iSta != iSet->end(); ++iSta)
		{
			sta2Part[*iSta] = nSetIdx;
			if ((oneDfaTab[*iSta].GetFlag() & oneDfaTab[*iSta].START) != 0)
			{
				oneDfaTab.SetStartId(nSetIdx);
			}
		}
		++nSetIdx;
	}

	//set new DFA and modify new number
	nSetIdx = 0;
	for (SETLIST_ITER iSet = Partition.begin(); iSet != Partition.end(); ++iSet)
	{
		for (BYTE iCol = 0; iCol != nCol; ++iCol)
		{
			CDfaRow &orgRow = oneDfaTab[iSet->front()];
			STATEID nDest = STATEID(-1);
			if (orgRow[iCol] != STATEID(-1))
			{
				nDest = sta2Part[orgRow[iCol]];
			}
			tmpDfa[nSetIdx][iCol] = nDest;
		}

		//set a state attribute
		tmpDfa[nSetIdx].SetFlag(oneDfaTab[iSet->front()].GetFlag());
		++nSetIdx;
	}

	//复制新的跳转表给原DFA
	oneDfaTab.m_pDfaClear();
	nSetIdx = 0;
	for (STATEID idx = 0; idx < nCol; ++idx)
	{
		oneDfaTab[nSetIdx] = tmpDfa[idx];
		++nSetIdx;
	}
}

