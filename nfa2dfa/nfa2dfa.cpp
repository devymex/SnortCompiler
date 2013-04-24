#include "stdafx.h"
#include "CreDfa.h"
#include "nfa2dfa.h"

typedef std::list<STATEID> STALIST;
typedef std::list<STATEID>::iterator STALIST_ITER;
typedef std::list<std::list<STATEID>> SETLIST;
typedef std::list<std::list<STATEID>>::iterator SETLIST_ITER;

CREDFA size_t NfaToDfa(CNfa &oneNfaTab, CDfa &dfaTab, bool combine)
{
	BYTE groups[DFACOLSIZE];
	AvaiEdges(oneNfaTab, groups);
	dfaTab.SetGroup(groups);

	std::vector<std::pair<std::vector<size_t>, STATEID>> termStasVec;

	typedef std::unordered_map<std::vector<size_t>, STATEID, STATESET_HASH> STATESETHASH;

	STATESETHASH ssh;
	ssh.rehash(STATESET_HASH::MAX_SIZE);

	std::stack<std::vector<size_t>> nfaStasStack;
	std::vector<size_t> startEVec;
	std::vector<size_t> startVec;

	char finFlag = 0;
	startVec.push_back(0);
	EClosure(oneNfaTab, startVec, startEVec, finFlag);


	nfaStasStack.push(startEVec);
	//ssh.insert(std::make_pair(startEVec, ssh.size()));
	ssh[startEVec] = 0;

	STATEID nCursize = dfaTab.Size();
	dfaTab.Resize(nCursize + 1);

	dfaTab.Back().SetFlag(dfaTab.Back().GetFlag() | dfaTab.Back().START);

	if(finFlag == 1)
	{
		dfaTab.Back().SetFlag(dfaTab.Back().GetFlag() | dfaTab.Back().TERMINAL);
		finFlag = 0;
	}
	std::vector<size_t> curNfaVec;
	std::vector<size_t> compuFlag;
	while(nfaStasStack.size() > 0)
	{
		compuFlag.clear();
		STATEID curStaNum;
		curNfaVec = nfaStasStack.top();
		nfaStasStack.pop();

		for(size_t nCurChar = 0; nCurChar < DFACOLSIZE; ++nCurChar)
		{
			if( dfaTab.Size() > SC_STATELIMIT)
			{
				return (size_t)-1;
			}

			STATEID curGroup = dfaTab.GetGroup(nCurChar);
			if(std::find(compuFlag.begin(), compuFlag.end(), curGroup) != compuFlag.end())
			{
				continue;
			}

			compuFlag.push_back(curGroup);

			STATESETHASH::iterator ir = ssh.find(curNfaVec);
			if (ir == ssh.end())
			{
				std::cout << "Fatal Error!" << std::endl;
				break;
			}
			curStaNum = ir->second;

			std::vector<size_t> nextNfaVec;

			NextNfaSet(oneNfaTab, curNfaVec, nCurChar, nextNfaVec, finFlag);

			if(!nextNfaVec.empty())
			{
				if(ssh.count(nextNfaVec) == 0)
				{
#undef max
					if (ssh.size() > std::numeric_limits<STATEID>::max())
					{
						std::cerr << "Fatal Error!" << std::endl;
						return (size_t)-1;
					}
					STATEID nextSta = (STATEID)ssh.size();
					ssh[nextNfaVec] = nextSta;

					STATEID nCursize = (STATEID)dfaTab.Size();
					dfaTab.Resize(nCursize + 1);

					dfaTab[curStaNum][curGroup] = nextSta;

					if(finFlag == 1)
					{
						dfaTab.Back().SetFlag(dfaTab.Back().GetFlag() | dfaTab.Back().TERMINAL);
						finFlag = 0;
						if(combine)
						{
							termStasVec.push_back(std::make_pair(nextNfaVec, nextSta));
						}

					}
					nfaStasStack.push(nextNfaVec);
				}
				else
				{
					dfaTab[curStaNum][curGroup] = ssh[nextNfaVec];
				}
			}
		}
	}

	if(combine)
	{
		if(!termStasVec.empty())
		{
			size_t tempNum = oneNfaTab.GetDfaTermsNum();
			for(size_t i = 0; i < tempNum; ++i)
			{
				for(size_t j = 0; j < termStasVec.size(); ++j)
				{
					for(size_t k = 0; k < termStasVec[j].first.size(); ++k)
					{
						CNfa::DFATERMS dfaTerm;
						dfaTerm = oneNfaTab.GetDfaTerms(i);
						if(dfaTerm.nfaSta == termStasVec[j].first[k])
						{
							CDfa::TERMSET term;
							term.dfaSta = termStasVec[j].second;
							term.dfaId = dfaTerm.dfaId;
							dfaTab.PushTermSet(term);
							break;
						}
					}
				}
			}
		}
	}
	return 0;
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

void MergeReachable(const CDfa &oneDfaTab, std::vector<BYTE> &reachable, CDfa &tmpDfa)
{
	//count reachable states 
	size_t nRcbCnt = std::count(reachable.begin(), reachable.end(), 2);
	tmpDfa.Resize((STATEID)nRcbCnt);
	tmpDfa.SetGroup(oneDfaTab.GetGroup());

	STATEID nNewIdx = 0;
	size_t nColNum = tmpDfa.GetColNum();
	//reachable states in old DFA copy to new DFA and renumber the new DFA
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

	//the new number replace the old in new DFA
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
}

void PartitionNonDisState(CDfa &tmpDfa, std::vector<STATEID> *pRevTbl, SETLIST &pSets)
{// every two states of the same partition are equivalent if they have the same behavior for all the input sequences,
 // and the partitions are reserved in pSets.

	size_t nColNum = tmpDfa.GetColNum();

	//store the iterator of patition will be visited into wSets
	std::list<SETLIST_ITER> wSets;
	SETLIST_ITER iLast = pSets.end();
	--iLast;

	//initialize wSets with all final states
	for (SETLIST_ITER iCurSet = pSets.begin(); iCurSet != iLast; ++iCurSet)
	{
		wSets.push_back(iCurSet);
	}

	std::vector<BYTE> ableToW(tmpDfa.Size(), 0);
	bool bAllZero = true;
	for (; !wSets.empty(); )
	{
		//choose and remove a partition from wSets
		STALIST curWSet = *wSets.front();
		wSets.pop_front();
		for (BYTE byChar = 0; byChar < nColNum; ++byChar)
		{
			//each char in charset,label state for which a transition on char to a state in partition,
			//1 is reachable and 0 is unreachable
			//if no state reach to a state in partition, the bAllZero is true; else, the bAllZero is false.
			for (STALIST_ITER iSta = curWSet.begin(); iSta != curWSet.end(); ++iSta)
			{
				std::vector<STATEID> &ableToI = pRevTbl[*iSta * nColNum + byChar];
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

void MergeNonDisStates(CDfa &tmpDfa, SETLIST &Partition, CDfa &minDfaTab)
{
	std::vector<STATEID> sta2Part(tmpDfa.Size());

	//set group of new DFA 
	minDfaTab.SetGroup(tmpDfa.GetGroup());
	minDfaTab.Resize((STATEID)Partition.size());

	//an old state which belongs to a new partition and set start state
	STATEID nSetIdx = 0;
	for (SETLIST_ITER iSet = Partition.begin(); iSet != Partition.end(); ++iSet)
	{
		for (STALIST_ITER iSta = iSet->begin(); iSta != iSet->end(); ++iSta)
		{
			sta2Part[*iSta] = nSetIdx;
			if ((tmpDfa[*iSta].GetFlag() & tmpDfa[*iSta].START) != 0)
			{
				minDfaTab.SetStartId(nSetIdx);
			}
		}
		++nSetIdx;
	}

	//set new DFA and modify new number
	nSetIdx = 0;
	for (SETLIST_ITER iSet = Partition.begin(); iSet != Partition.end(); ++iSet)
	{
		for (BYTE iCol = 0; iCol != minDfaTab.GetColNum(); ++iCol)
		{
			CDfaRow &orgRow = tmpDfa[iSet->front()];
			STATEID nDest = STATEID(-1);
			if (orgRow[iCol] != STATEID(-1))
			{
				nDest = sta2Part[orgRow[iCol]];
			}
			minDfaTab[nSetIdx][iCol] = nDest;
		}

		//set a state attribute
		minDfaTab[nSetIdx].SetFlag(tmpDfa[iSet->front()].GetFlag());
		++nSetIdx;
	}
}


CREDFA size_t DfaMin(CDfa &oneDfaTab, CDfa &minDfaTab)
{
	//error: DFA is empty 
	if (oneDfaTab.Size() == 0)
	{
		return size_t(-1);
	}

	// Extract terminal states from a dfa, initialize partition with terminal states and normal states
	std::list<std::list<STATEID>> Partition(1);
	std::list<STATEID> FinalStas;
	for (STATEID i = 0; i < oneDfaTab.Size(); ++i)
	{
		if ((oneDfaTab[i].GetFlag() & oneDfaTab[i].TERMINAL) != 0)
		{
			FinalStas.push_back(i);
			Partition.push_front(std::list<STATEID>());
			Partition.front().push_back(i);
		}
		else
		{
			Partition.back().push_back(i);
		}
	}

	//error: terminal states or normal states are empty
	if (Partition.size() == 1 || Partition.back().empty())
	{
		return size_t(-2);
	}

	//generate positive traverse table
	size_t row = oneDfaTab.Size();
	size_t col = oneDfaTab.GetColNum();
	std::vector<STATEID> *pPosTab = new std::vector<STATEID>[row * col];
	for (STATEID i = 0; i < row; ++i)
	{
		for (STATEID j = 0; j < col; ++j)
		{
			STATEID nDest = (STATEID)oneDfaTab[i][j];
			if (nDest != STATEID(-1))
			{
				pPosTab[i * col + j].push_back(nDest);
			}
		}
	}

	std::vector<BYTE> reachable(oneDfaTab.Size(), 0);
	std::list<STATEID> StartStas;
	StartStas.push_back(0);

	//record states that will be visited from positive traverse table
	RemoveUnreachable(pPosTab, StartStas, col, reachable);
	delete []pPosTab;

	//generate reverse traverse table	
	std::vector<STATEID> *pRevTab = new std::vector<STATEID>[row * col];
	for (STATEID i = 0; i < row; ++i)
	{
		for (STATEID j = 0; j < col; ++j)
		{
			STATEID nDest = (STATEID)oneDfaTab[i][j];
			if (nDest != STATEID(-1))
			{
				pRevTab[nDest * col + j].push_back(STATEID(i));
			}
		}
	}

	//record states that will be visited from reverse traverse table
	RemoveUnreachable(pRevTab, FinalStas, col, reachable);

	//remove unreachable states, generate new DFA
	CDfa tmpDfa;
	MergeReachable(oneDfaTab, reachable, tmpDfa);

	//divide nondistinguishable states
	PartitionNonDisState(tmpDfa, pRevTab, Partition);

	//DFA minization
	MergeNonDisStates(tmpDfa, Partition, minDfaTab);

	minDfaTab.SetId(oneDfaTab.GetId());

	for (STATEID j = 0; j < minDfaTab.Size(); ++j)
	{
		std::cout << (int)j << ": ";
		for (STATEID k = 0; k < minDfaTab.GetColNum(); ++k)
		{
			if(minDfaTab[j][k] != STATEID(-1))
			{
				std::cout << "(" << (int)k << "," << (int)minDfaTab[j][k]<< ")";
			}

		}
		std::cout << std::endl;
	}


	delete []pRevTab;
	return 0;
}

