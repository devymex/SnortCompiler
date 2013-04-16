#include "stdafx.h"
#include "CreDfa.h"
#include "nfa2dfa.h"

CREDFA void NfaToDfa(CNfa &oneNfaTab, CDfa &dfaTab)
{
	std::vector<std::vector<size_t>> charGroups;
	AvaiEdges(oneNfaTab, charGroups);

	//printNfa(oneNfaTab);
	//std::vector<size_t> hashTable[HASHMODULO];
	typedef std::unordered_map<std::vector<size_t>, size_t, STATESET_HASH> STATESETHASH;

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
	ssh[startEVec] = ssh.size();

	size_t nCursize = dfaTab.Size();
	dfaTab.Resize(nCursize + 1);

	dfaTab.Back().SetFlag(dfaTab.Back().GetFlag() | dfaTab.Back().START);

	if(finFlag == 1)
	{
		dfaTab.Back().SetFlag(dfaTab.Back().GetFlag() | dfaTab.Back().TERMINAL);
		finFlag = 0;
	}
	std::vector<size_t> curNfaVec;

	while(nfaStasStack.size() > 0)
	{
		int curStaNum;
		curNfaVec = nfaStasStack.top();
		nfaStasStack.pop();

		for(std::vector<std::vector<size_t>>::iterator group = charGroups.begin();
			group != charGroups.end(); ++group)
		{
			if( dfaTab.Size() > 30000)
			{
				return;
			}
			size_t nCurChar = group->front();

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
					size_t nextSta = ssh.size();
					ssh[nextNfaVec] = nextSta;

					size_t nCursize = dfaTab.Size();
					dfaTab.Resize(nCursize + 1);
					for(std::vector<size_t>::iterator iter = group->begin(); iter != group->end(); ++iter )
					{
						dfaTab[curStaNum][*iter] = nextSta;
					}
					if(finFlag == 1)
					{
						dfaTab.Back().SetFlag(dfaTab.Back().GetFlag() | dfaTab.Back().TERMINAL);
						finFlag = 0;
					}
					nfaStasStack.push(nextNfaVec);
				}
				else
				{
					for(std::vector<size_t>::iterator iter = group->begin(); iter != group->end(); ++iter )
					{
						dfaTab[curStaNum][*iter] = ssh[nextNfaVec];
					}
				}
			}
		}
	}
}

void RemoveUselessStates(CDfa &oneDfaTab, CNfa &RevDfa, std::vector<size_t> &FinalStas, std::vector<size_t> &Reachable)
{
	std::vector<size_t> PosReachableStas;
	std::vector<size_t> RevReachableStas;
	
	std::vector<size_t> nextStas;
	std::vector<size_t> tmpReach;	
	nextStas.push_back(0);
	PosReachableStas.push_back(0);
	for (; !nextStas.empty(); )
	{
		tmpReach = PosReachableStas;
		std::sort(tmpReach.begin(), tmpReach.end());
		std::vector<size_t> tmp;
		for (std::vector<size_t>::iterator iter = nextStas.begin(); iter != nextStas.end(); ++iter)
		{
			for (size_t i = 0; i < CHARSETSIZE; ++i)
			{
				if (oneDfaTab[*iter][i] != -1)
				{
					tmp.push_back(oneDfaTab[*iter][i]);
				}
			}
		}
		std::sort(tmp.begin(), tmp.end());
		tmp.erase(std::unique(tmp.begin(), tmp.end()));
		nextStas.clear();
		std::set_difference(tmp.begin(), tmp.end(), tmpReach.begin(), tmpReach.end(), std::back_inserter(nextStas));
		std::set_union(tmpReach.begin(),tmpReach.end(), nextStas.begin(), nextStas.end(), std::back_inserter(PosReachableStas));
		tmpReach.clear();
	}

	tmpReach.clear();
	std::vector<size_t> preStas = FinalStas;
	RevReachableStas = FinalStas;
	for (; !preStas.empty(); )
	{
		tmpReach = RevReachableStas;
		std::sort(tmpReach.begin(), tmpReach.end());
		std::vector<size_t> tmp;
		for (std::vector<size_t>::iterator iter = preStas.begin(); iter != preStas.end(); ++iter)
		{
			for (size_t i = 0; i < CHARSETSIZE; ++i)	
			{
				if (!RevDfa[*iter][i].Empty())
				{
					for (size_t j = 0; j < RevDfa[*iter][i].Size(); ++j)
					{
						tmp.push_back(RevDfa[*iter][i][j]);
					}
				}
			}
		}
		std::sort(tmp.begin(), tmp.end());
		tmp.erase(std::unique(tmp.begin(), tmp.end()));
		preStas.clear();
		std::set_difference(tmp.begin(), tmp.end(), tmpReach.begin(), tmpReach.end(), std::back_inserter(preStas));
		std::set_union(tmpReach.begin(), tmpReach.end(), preStas.begin(), preStas.end(), std::back_inserter(RevReachableStas));
		tmpReach.clear();
	}

	std::set_intersection(PosReachableStas.begin(), PosReachableStas.end(), RevReachableStas.begin(), RevReachableStas.end(), std::back_inserter(Reachable));
	std::sort(Reachable.begin(), Reachable.end());
}

void MergeReachable(CDfa &oneDfaTab, std::vector<size_t> &Reachable, CDfa &tmpDfa)
{
	std::vector<size_t> full;
	std::vector<size_t> difference;
	for (size_t i = 0; i < oneDfaTab.Size(); ++i)
	{
		full.push_back(i);
	}
	std::set_difference(full.begin(), full.end(), Reachable.begin(), Reachable.end(), std::back_inserter(difference));
	tmpDfa.Resize(Reachable.size());
	size_t k = 0;
	for (size_t i = 0, j = 0; i < difference.size() && j < oneDfaTab.Size(); )
	{
		if (j != difference[i])
		{
			tmpDfa[k++] = oneDfaTab[j];
			++j;
		}
		else
		{
			++i;
			++j;
		}
	}
	for (std::vector<size_t>::iterator iter = difference.begin(); iter != difference.end(); ++iter)
	{
		for (size_t i = 0; i < tmpDfa.Size(); ++i)
		{
			for (size_t j = 0; j < CHARSETSIZE; ++j)
			{
				if (tmpDfa[i][j] == *iter)
				{
					tmpDfa[i][j] == size_t(-1); 
				}
			}
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


void PartitionNonDisState(CDfa &tmpDfa, CNfa &RevTab, std::vector<std::vector<size_t>> &partition, std::vector<size_t> &FinalStas)
{
	std::vector<std::vector<size_t>> NewSplit;
	CDfa tmp = tmpDfa;
	
	for (NewSplit.push_back(FinalStas); !NewSplit.empty(); )
	{
		std::vector<size_t> oneset;
		oneset = NewSplit.front();
		NewSplit.erase(NewSplit.begin());

		for (size_t character = 0; character < CHARSETSIZE; ++character)
		{
			std::vector<size_t> equstates;
			for (std::vector<size_t>::iterator iState = oneset.begin(); iState != oneset.end(); ++iState)
			{
				if (!RevTab[*iState][character].Empty)
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
				SubPartition(partition, NewSplit, equstates, NextPartition);
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
		size_t tmp = iP - Partition.begin();
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
	if (oneDfaTab.Empty())
	{
		return size_t(-1);
	}

	CNfa RevTab;
	RevTab.Resize(oneDfaTab.Size());
	for (size_t i = 0; i < oneDfaTab.Size(); ++i)
	{
		for (size_t j = 0; j < CHARSETSIZE; ++j)
		{
			if (oneDfaTab[i][j] != -1)
			{
				RevTab[oneDfaTab[i][j]][j].PushBack(i);
			}
		}
	}

	std::vector<size_t> FinalStas;
	std::vector<size_t> NormalStas;
	for (size_t i = 0; i < oneDfaTab.Size(); ++i)
	{
		if ((oneDfaTab[i].GetFlag & oneDfaTab[i].TERMINAL) != 0)
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

	CDfa tmpDfa;
	std::vector<size_t> Reachable;
	RemoveUselessStates(oneDfaTab, RevTab, FinalStas, Reachable);

	std::vector<std::vector<size_t>> Partition;
	Partition.push_back(FinalStas);
	Partition.push_back(NormalStas);

	PartitionNonDisState(tmpDfa, RevTab, Partition, FinalStas);

	return 0;
}
