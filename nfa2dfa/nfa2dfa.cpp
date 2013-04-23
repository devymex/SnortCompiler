#include "stdafx.h"
#include "CreDfa.h"
#include "nfa2dfa.h"

typedef std::list<STATEID> STALIST;
typedef std::list<STATEID>::iterator STALIST_ITER;
typedef std::list<std::list<STATEID>> SETLIST;
typedef std::list<std::list<STATEID>>::iterator SETLIST_ITER;

CREDFA size_t NfaToDfa(CNfa &oneNfaTab, CDfa &dfaTab)
{
	BYTE groups[DFACOLSIZE];
	AvaiEdges(oneNfaTab, groups);
	dfaTab.SetGroup(groups);

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

	size_t nCursize = dfaTab.Size();
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
		int curStaNum;
		curNfaVec = nfaStasStack.top();
		nfaStasStack.pop();

		for(size_t nCurChar = 0; nCurChar < CHARSETSIZE - 4; ++nCurChar)
		{
			if( dfaTab.Size() > SC_STATELIMIT)
			{
				return (size_t)-1;
			}

			size_t curGroup = dfaTab.GetGroup(nCurChar);
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
					STATEID nextSta = static_cast<STATEID>(ssh.size());
					ssh[nextNfaVec] = nextSta;

					size_t nCursize = dfaTab.Size();
					dfaTab.Resize(nCursize + 1);

					dfaTab[curStaNum][curGroup] = nextSta;

					if(finFlag == 1)
					{
						dfaTab.Back().SetFlag(dfaTab.Back().GetFlag() | dfaTab.Back().TERMINAL);
						finFlag = 0;
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

	return 0;
}

void RemoveUnreachable(const std::vector<STATEID> *Tab, const STALIST &begs, const size_t &col, std::vector<BYTE> &reachable)
{
	size_t stas = reachable.size();
	std::vector<BYTE> staFlags(stas, 0);

	std::vector<size_t> staStack;
	staStack.reserve(stas);

	staStack.assign(begs.begin(), begs.end());
	for (; !staStack.empty(); )
	{
		size_t nSta = staStack.back();
		staStack.pop_back();
		if (staFlags[nSta] == 0)
		{
			staFlags[nSta] = 1;
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
	size_t nRcbCnt = std::count(reachable.begin(), reachable.end(), 2);
	tmpDfa.Resize(nRcbCnt);

	STATEID nNewIdx = 0, nColNum = tmpDfa.GetColNum();
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
			*iter = -1;
		}
	}

	for (size_t i = 0; i < nRcbCnt; ++i)
	{
		CDfaRow &curRow = tmpDfa[i];
		for (size_t j = 0; j < nColNum; ++j)
		{
			curRow[j] = reachable[curRow[j]];
		}
	}
}

void PartitionNonDisState(CDfa &tmpDfa, std::vector<STATEID> *pRevTbl, SETLIST &pSets)
{

	size_t nColNum = tmpDfa.GetColNum();

	std::list<SETLIST_ITER> wSets;
	SETLIST_ITER iLast = pSets.end();
	--iLast;
	
	for (SETLIST_ITER iCurSet = pSets.begin(); iCurSet != iLast; ++iCurSet)
	{
		wSets.push_back(iCurSet);
	}

	std::vector<BYTE> ableToW(tmpDfa.Size(), 0);
	bool bAllZero = true;
	for (; !wSets.empty(); )
	{
		STALIST curWSet = *wSets.front();
		curWSet.pop_front();
		for (BYTE byChar = 0; byChar < nColNum; ++byChar)
		{
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
					if (nUnableCnt <= iPSet->size() / 2)
					{
						iCutEnd = iCutBeg;
						iCutBeg = iPSet->begin();
					}
					if (iCutBeg != iCutEnd)
					{
						iPSet = pSets.insert(++iPSet, STALIST());
						iPSet->splice(iPSet->begin(), *iPSet, iCutBeg, iCutEnd);
						wSets.push_back(iPSet);
					}
				}
				ZeroMemory(&ableToW[0], ableToW.size());
				bAllZero = true;
			}
		}
	}
}

struct COMPARE
{
	bool operator()(std::vector<size_t> &x, std::vector<size_t> &y)
	{
		 return x.front() < y.front();
	}
};


void MergeNonDisStates(CDfa &tmpDfa, SETLIST &Partition, CDfa &minDfaTab)
{
	std::vector<STATEID> sta2Part(tmpDfa.Size());

	size_t nSetIdx = 0, nStartIdx = 0;
	for (SETLIST_ITER iSet = Partition.begin(); iSet != Partition.end(); ++iSet)
	{
		for (STALIST_ITER iSta = iSet->begin(); iSta != iSet->end(); ++iSet)
		{
			sta2Part[*iSta] = nSetIdx;
		}
		++nSetIdx;
	}
	minDfaTab.SetGroup(tmpDfa.GetGroup());

	minDfaTab.Resize(Partition.size());

	nSetIdx = 0;
	for (SETLIST_ITER iSet = Partition.begin(); iSet != Partition.end(); ++iSet)
	{
		for (BYTE iCol = 0; iCol != minDfaTab.GetColNum(); ++iCol)
		{
			CDfaRow &orgRow = tmpDfa[iSet->front()];
			STATEID nDest = -1;
			if (orgRow[iCol] != -1)
			{
				nDest = sta2Part[orgRow[iCol]];
			}
			minDfaTab[nSetIdx][iCol] = nDest;
		}
		++nSetIdx;
	}
}


CREDFA size_t DfaMin(CDfa &oneDfaTab, CDfa &minDfaTab)
{
	if (oneDfaTab.Size() == 0)
	{
		return size_t(-1);
	}

	// Extract final states from a dfa;
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

	if (Partition.size() == 1 || Partition.back().empty())
	{
		return size_t(-2);
	}

	size_t row = oneDfaTab.Size();
	size_t col = oneDfaTab.GetColNum();
	std::vector<STATEID> *pPosTab = new std::vector<STATEID>[row * col];
	for (size_t i = 0; i < row; ++i)
	{
		for (size_t j = 0; j < col; ++j)
		{
			STATEID nDest = oneDfaTab[i][oneDfaTab.GetGroup(j)];
			if (nDest != -1)
			{
				pPosTab[i * col + j].push_back(nDest);
			}
		}
	}

	std::vector<BYTE> reachable(oneDfaTab.Size(), 0);
	std::list<STATEID> StartStas;
	StartStas.push_back(0);

	RemoveUnreachable(pPosTab, StartStas, col, reachable);
	delete []pPosTab;

	std::vector<STATEID> *pRevTab = new std::vector<STATEID>[row * col];
	for (size_t i = 0; i < row; ++i)
	{
		for (size_t j = 0; j < col; ++j)
		{
			STATEID nDest = oneDfaTab[i][oneDfaTab.GetGroup(j)];
			if (nDest != -1)
			{
				pRevTab[nDest * col + j].push_back(i);
			}
		}
	}

	RemoveUnreachable(pRevTab, FinalStas, col, reachable);

	CDfa tmpDfa;
	MergeReachable(oneDfaTab, reachable, tmpDfa);

	PartitionNonDisState(tmpDfa, pRevTab, Partition);
	MergeNonDisStates(tmpDfa, Partition, minDfaTab);

	//minDfaTab.SetId(oneDfaTab.GetId());


	delete []pRevTab;
	return 0;
}
