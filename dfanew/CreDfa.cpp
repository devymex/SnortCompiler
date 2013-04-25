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

//删除不可达状态或者“死”状态，Tab表示传入一个DFA的正向访问表或一个DFA的逆向访问表
//begs表示读Tab的起始位置
//reachable表示可达状态集合，初始值为0，输出结果
void RemoveUnreachable(const std::vector<STATEID> *Tab, const STALIST &begs, const size_t &col, std::vector<BYTE> &reachable)
{
	size_t stas = reachable.size();
	//mark state after traversal, 0 is unreachable and 1 is reachable
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

//groupnum表示字符集长度，pRevTbl表示逆向访问表
//pSets表示输入一个状态集的初始划分，输出一个状态集的最终划分结果
//pSets初始值为终态和非终态集合，其中最后一个为非终态集合
void PartitionNonDisState(const size_t &groupnum, const size_t &size, std::vector<STATEID> *pRevTbl, SETLIST &pSets)
{
	//将需要查找的划分的iterator存入wSets，初始化时只保存终态集合
	std::list<SETLIST_ITER> wSets;
	SETLIST_ITER iLast = pSets.end();
	--iLast;

	//initialize wSets
	for (SETLIST_ITER iCurSet = pSets.begin(); iCurSet != iLast; ++iCurSet)
	{
		wSets.push_back(iCurSet);
	}

	//each element in ableToW present a property of according state of tmpDfa,
	//and has two labels, 0 and 1. 1 indicates the according state has the specific
	//transition to one state of curWSet, 0 otherwise
	std::vector<BYTE> ableToW(size,BYTE(0));
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

