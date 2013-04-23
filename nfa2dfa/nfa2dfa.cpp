#include "stdafx.h"
#include "CreDfa.h"
#include "nfa2dfa.h"

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

void RemoveUnreachable(const CNfa &nfa, const std::vector<size_t> &begs, std::vector<BYTE> &reachable)
{
	std::vector<BYTE> staFlags(nfa.Size(), 0);

	std::vector<size_t> staStack;
	staStack.reserve(nfa.Size());

	staStack.assign(begs.begin(), begs.end());
	for (; !staStack.empty(); )
	{
		size_t nSta = staStack.back();
		staStack.pop_back();
		if (staFlags[nSta] == 0)
		{
			staFlags[nSta] = 1;
			for (size_t i = 0; i < CHARSETSIZE; ++i)
			{
				for (size_t j = 0; j < nfa[nSta][i].Size(); ++j)
				{
					staStack.push_back(nfa[nSta][i][j]);
				}
			}
		}
	}

	if (reachable.empty())
	{
		reachable.resize(staFlags.size(), 0);
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

int SearchInSet(std::vector<size_t>::iterator beg, std::vector<size_t>::iterator end, std::vector<std::vector<size_t>> &NewSplitStates)
{
	int post = 0;
	if (!NewSplitStates.empty())
	{
		for (std::vector<std::vector<size_t>>::iterator i = NewSplitStates.begin(); i != NewSplitStates.end(); ++i)
		{
			std::vector<size_t>::iterator pos = std::search(i->begin(), i->end(), beg, end);
			if (pos != i->end())
			{
				post = pos - i->begin();
			}
		}
	}
	return post;
}

void SubPartition(std::vector<std::vector<size_t>> &partition_states, std::vector<std::vector<size_t>> &NewSplitStates, std::vector<size_t> &equstates, std::vector<std::vector<size_t>> &NextPartition)
{
	std::vector<std::vector<size_t>> temp = NewSplitStates;
	for (std::vector<std::vector<size_t>>::iterator iPS = partition_states.begin(); iPS != partition_states.end(); ++iPS)
	{
		std::vector<size_t> intersect;
		std::vector<size_t> difference;
		std::sort(iPS->begin(),iPS->end());
		std::sort(equstates.begin(),equstates.end());
		std::set_intersection(iPS->begin(), iPS->end(), equstates.begin(), equstates.end(), std::back_inserter(intersect));
		std::set_difference(iPS->begin(), iPS->end(), equstates.begin(), equstates.end(), std::back_inserter(difference));
		if (!intersect.empty())
		{
			NextPartition.push_back(intersect);
			NextPartition.push_back(difference);
			int post = SearchInSet(iPS->begin(), iPS->end(), temp);
			if (post != 0)
			{
				NewSplitStates.erase(std::remove(NewSplitStates.begin(), NewSplitStates.end(), NewSplitStates[post]));
				NewSplitStates.push_back(intersect);
				NewSplitStates.push_back(difference);
			}
			else if (intersect.size() <= difference.size())
			{
				NewSplitStates.push_back(intersect);
			}
			else
			{
				NewSplitStates.push_back(difference);
			}
		}
		else 
		{
			NextPartition.push_back(*iPS);
		}
	}

	for (std::vector<std::vector<size_t>>::iterator i = NextPartition.begin(); i != NextPartition.end();)
	{
		if (i->size() == 0)
		{
			i = NextPartition.erase(i);
		}
		else
		{
			++i;
		}
	}

	for (std::vector<std::vector<size_t>>::iterator i = NewSplitStates.begin(); i != NewSplitStates.end();)
	{
		if (i->size() == 0)
		{
			i = NewSplitStates.erase(i);
		}
		else
		{
			++i;
		}
	}
}


void PartitionNonDisState(CDfa &tmpDfa, CNfa &RevTab, std::vector<std::vector<size_t>> &partition, std::vector<size_t> &fin)
{
	std::vector<std::vector<size_t>> staset;
	staset.push_back(fin);

	CDfa tmp = tmpDfa;

	for ( ; !staset.empty(); )
	{
		std::vector<size_t> oneset;
		oneset = staset.front();
		staset.erase(staset.begin());

		//std::vector<std::vector<BYTE>> charFlag(CHAR256);
		//for (size_t i = 0; i < CHAR256; ++i)
		//{
		//	charFlag[i].resize(tmpDfa.Size(), 0);
		//}
		//for (size_t character = 0; character < CHAR256; ++character)
		//{
		//	for (std::vector<size_t>::iterator iState = oneset.begin(); iState != oneset.end(); ++iState)
		//	{
		//		if (RevTab[*iState][character].Size() != 0)
		//		{
		//			for (size_t i = 0; i < RevTab[*iState][character].Size(); ++i)
		//			{
		//				charFlag[character][RevTab[*iState][character][i]] = 1;
		//			}
		//		}
		//	}
		//}

		for (size_t character = 0; character < CHAR256; ++character)
		{
			std::vector<size_t> equstates;
			for (std::vector<size_t>::iterator iState = oneset.begin(); iState != oneset.end(); ++iState)
			{
				if (RevTab[*iState][character].Size() != 0)
				{
					for (size_t i = 0; i < RevTab[*iState][character].Size(); ++i)
					{
						equstates.push_back(RevTab[*iState][character][i]);
					}
				}
			}

			if (!equstates.empty())
			{
				std::vector<std::vector<size_t>> NextPartition;
				SubPartition(partition, staset, equstates, NextPartition);
				partition.clear();
				partition = NextPartition;	
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


void MergeNonDisStates(CDfa &tmpDfa, std::vector<std::vector<size_t>> &Partition, CDfa &minDfaTab)
{
	std::sort(Partition.begin(), Partition.end(), COMPARE());
	minDfaTab.Resize(Partition.size());
	size_t k = 0;
	for (size_t i = 0, j = 0; i < Partition.size() && j < tmpDfa.Size(); )
	{
		if (j == Partition[i].front())
		{
			minDfaTab[k++] = tmpDfa[j];			
			minDfaTab[k-1].SetFlag(tmpDfa[j].GetFlag());
			++i;
			++j;
		}
		else
		{
			++j;
		}
	}
	for (std::vector<std::vector<size_t>>::iterator iP = Partition.begin(); iP != Partition.end(); ++iP)
	{
		STATEID tmp = (STATEID)(iP - Partition.begin());
		for (std::vector<size_t>::iterator iS = iP->begin(); iS != iP->end(); ++iS)
		{
			for (size_t i = 0; i < minDfaTab.Size(); ++i)
			{
				for (size_t j = 0; j < CHARSETSIZE; ++j)
				{
					if (minDfaTab[i][j] == *iS)
					{
						minDfaTab[i][j] = tmp;
					}
				}
			}
		}
	}
}


CREDFA size_t DfaMin(CDfa &oneDfaTab, CDfa &minDfaTab)
{
	if (oneDfaTab.Size() == 0)
	{
		return size_t(-1);
	}

	minDfaTab.SetId(oneDfaTab.GetId());



	CNfa nfaTab;
	nfaTab.FromDfa(oneDfaTab);

	std::vector<BYTE> reachable;
	std::vector<size_t> StartStas;
	StartStas.push_back(0);

	RemoveUnreachable(nfaTab, StartStas, reachable);

	CNfa revTab;
	revTab.Resize(oneDfaTab.Size());
	for (size_t i = 0; i < oneDfaTab.Size(); ++i)
	{
		for (size_t j = 0; j < CHAR256; ++j)
		{
			STATEID nDest = oneDfaTab[i][oneDfaTab.GetGroup(j)];
			if (nDest != -1)
			{
				revTab[nDest][j].PushBack(i);
			}
		}
	}

	// Extract final states from a dfa;
	std::vector<size_t> FinalStas;
	std::vector<size_t> NormalStas;
	for (size_t i = 0; i < oneDfaTab.Size(); ++i)
	{
		if ((oneDfaTab[i].GetFlag() & oneDfaTab[i].TERMINAL) != 0)
		{
			FinalStas.push_back(i);
		}
		else
		{
			NormalStas.push_back(i);
		}
	}

	if (FinalStas.empty())
	{
		return size_t(-2);
	}

	RemoveUnreachable(revTab, FinalStas, reachable);

	CDfa tmpDfa;
	MergeReachable(oneDfaTab, reachable, tmpDfa);

	std::vector<std::vector<size_t>> Partition;
	Partition.push_back(FinalStas);
	Partition.push_back(NormalStas);

	PartitionNonDisState(tmpDfa, revTab, Partition, FinalStas);
	//MergeNonDisStates(tmpDfa, Partition, minDfaTab);

	return 0;
}
